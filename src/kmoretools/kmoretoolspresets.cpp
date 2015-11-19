/*
    Copyright 2015 by Gregor Mi <codestruct@posteo.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "kmoretoolspresets.h"

#include <QDebug>
#include <QHash>

#include <KNS3/KMoreTools>

#define _ QStringLiteral

class KmtServiceInfo
{
public:
    KmtServiceInfo(QString desktopEntryName, QString homepageUrl, int maxUrlArgCount)
        : desktopEntryName(desktopEntryName), homepageUrl(homepageUrl), maxUrlArgCount(maxUrlArgCount)
    {
    }
public:
    QString desktopEntryName;
    QString homepageUrl;
    int maxUrlArgCount;
};

//
// todo later: add a property "maturity" with values "stable" > "new" > "incubating" or similar
//
KMoreToolsService* KMoreToolsPresets::registerServiceByDesktopEntryName(KMoreTools* kmt, const QString& desktopEntryName)
{
    static QHash<QString, KmtServiceInfo> dict;

#define ADD_ENTRY(desktopEntryName, maxUrlArgCount, homepageUrl) dict.insert(desktopEntryName, KmtServiceInfo(desktopEntryName, QLatin1String(homepageUrl), maxUrlArgCount));

    //
    // definitions begin:
    //                                              .------ If one gives more URL arguments as
    //                                              |       specified here the program will not work.
    //                                              |       Note, that there are some desktop files where _too few_
    //                                              |       arguments also lead to errors. Watch the console
    //                                              v       output for messages from the program.
    //
    ADD_ENTRY("catfish",                            1, "http://www.twotoasts.de/index.php/catfish/");
    ADD_ENTRY("git-cola-folder-handler",            1, "https://git-cola.github.io");
    ADD_ENTRY("git-cola-view-history.kmt-edition",  1, "https://git-cola.github.io");
    ADD_ENTRY("gitk.kmt-edition",                   1, "http://git-scm.com/docs/gitk");
    ADD_ENTRY("qgit.kmt-edition",                   1, "http://libre.tibirna.org/projects/qgit");
    ADD_ENTRY("gitg",                               1, "https://wiki.gnome.org/action/show/Apps/Gitg?action=show&redirect=Gitg");
    ADD_ENTRY("gparted",                            0, "http://gparted.org");
    ADD_ENTRY("htop",                               0, "http://hisham.hm/htop/");
    ADD_ENTRY("org.kde.PartitionManager",           0, "https://www.kde.org/applications/system/kdepartitionmanager/");
    ADD_ENTRY("disk",                               0, "https://en.opensuse.org/YaST_Disk_Controller");
    ADD_ENTRY("kdf",                                0, "https://www.kde.org/applications/system/kdiskfree");
    ADD_ENTRY("ksystemlog",                         0, "https://www.kde.org/applications/system/ksystemlog/");
    ADD_ENTRY("org.gnome.clocks",                   0, "https://wiki.gnome.org/Apps/Clocks");
    ADD_ENTRY("org.kde.filelight",                  1, "https://utils.kde.org/projects/filelight");
    ADD_ENTRY("org.kde.kfind",                      1, "https://www.kde.org/applications/utilities/kfind/");
    ADD_ENTRY("org.kde.plasma.cuttlefish.kmt-edition", 0, "http://vizzzion.org/blog/2015/02/say-hi-to-cuttlefish/");
    ADD_ENTRY("org.kde.ksysguard",                  0, "https://userbase.kde.org/KSysGuard");
    ADD_ENTRY("org.kde.ktimer",                     0, "https://www.kde.org/applications/utilities/ktimer/");
    ADD_ENTRY("hotshots",                           1, "http://sourceforge.net/projects/hotshots/");
    ADD_ENTRY("kaption",                            0, "http://kde-apps.org/content/show.php/?content=139302");
    ADD_ENTRY("org.kde.kscreengenie",               0, "http://quickgit.kde.org/?p=kscreengenie.git");
    ADD_ENTRY("org.kde.ksnapshot",                  0, "https://www.kde.org/applications/graphics/ksnapshot/");
    ADD_ENTRY("shutter",                            0, "http://shutter-project.org");
    ADD_ENTRY("xfce4-taskmanager",                  0, "http://goodies.xfce.org/projects/applications/xfce4-taskmanager");
    //
    // ...definitions end
    //

#undef ADD_ENTRY

    auto iter = dict.find(desktopEntryName);
    if (iter != dict.end()) {
        auto kmtServiceInfo = *iter;
        const QString subdir = QStringLiteral("presets-kmoretools");
        auto serviceLocatingMode = desktopEntryName.endsWith(QLatin1String(".kmt-edition")) ?
                                   KMoreTools::ServiceLocatingMode_ByProvidedExecLine : KMoreTools::ServiceLocatingMode_Default;
        auto service = kmt->registerServiceByDesktopEntryName(desktopEntryName, subdir, serviceLocatingMode);
        service->setHomepageUrl(QUrl(kmtServiceInfo.homepageUrl));
        service->setMaxUrlArgCount(kmtServiceInfo.maxUrlArgCount);
        return service;
    } else {
        qDebug() << "KMoreToolsPresets::registerServiceByDesktopEntryName: " << desktopEntryName << "was not found. Return nullptr.";
        return nullptr;
    }
}

QList<KMoreToolsService*> KMoreToolsPresets::registerServicesByGroupingNames(KMoreTools* kmt, const QStringList& groupingNames)
{
    static QHash<QString, QList<QString>> dict;

    // The corresponding desktop files are located here:
    // 'knewstuff/data/kmoretools-desktopfiles/'

    // Use KMoreToolsTest2::testDialogForGroupingNames to see if the settings
    // here are correct.

    // NOTE that the desktopentry names must be registered in
    // registerServiceByDesktopEntryName above.

    // For special handlings about naming in the menu etc. see kmoretoolsmenufactory.cpp/addItemsForGroupingNameWithSpecialHandling

    //
    // definitions begin:
    //
    dict.insert(_("disk-usage"), { _("kdf"), _("org.kde.filelight") });
    dict.insert(_("disk-partitions"), { _("gparted"), _("org.kde.PartitionManager"), _("disk") });
    dict.insert(_("files-find"), { _("org.kde.kfind"), _("catfish") });
    dict.insert(_("git-clients-for-folder"), { _("git-cola-folder-handler"), _("gitk.kmt-edition"), _("qgit.kmt-edition"), _("gitg") });
    dict.insert(_("git-clients-and-actions"), { _("git-cola-folder-handler"), _("git-cola-view-history.kmt-edition"),
                _("gitk.kmt-edition"), _("qgit.kmt-edition"), _("gitg") });
    dict.insert(_("icon-browser"), { _("org.kde.plasma.cuttlefish.kmt-edition") });
    dict.insert(_("screenshot-take"), { _("org.kde.ksnapshot"), _("org.kde.kscreengenie"), _("shutter"), _("kaption"), _("hotshots") });
    dict.insert(_("system-monitor-processes"), { _("org.kde.ksysguard"), _("htop"), _("xfce4-taskmanager") });
    dict.insert(_("system-monitor-logs"), { _("ksystemlog") });
    dict.insert(_("time-countdown"), { _("org.gnome.clocks"), _("org.kde.ktimer") });
    //
    // ...definitions end
    //

    QList<KMoreToolsService*> resultList;

    Q_FOREACH (QString groupingName, groupingNames) {
        auto iter = dict.find(groupingName);
        if (iter != dict.end()) {
            Q_FOREACH(QString desktopEntryName, *iter) {
                resultList << registerServiceByDesktopEntryName(kmt, desktopEntryName);
            }
        }
        else {
            qDebug() << "KMoreToolsPresets::registerServicesByGroupingName: groupingName not found: " << groupingName;
        }
    }

    if (resultList.isEmpty()) {
        qDebug() << "KMoreToolsPresets::registerServicesByGroupingName: " << groupingNames << ". Nothing found in this groupings. HINT: check for invalid grouping names.";
    }

    return resultList;
}

