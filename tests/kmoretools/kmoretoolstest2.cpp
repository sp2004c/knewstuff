/*
 * Copyright 2014 2015 by Gregor Mi <codestruct@posteo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <../src/kmoretools/kmoretools.h>
#include <../src/kmoretools/kmoretools_p.h>
#include <../src/kmoretools/kmoretoolspresets.h>

#include <QTest>
#include <QRegularExpression>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#define _ QLatin1String

class KMoreToolsTest2 : public QObject
{
    Q_OBJECT

private Q_SLOTS: // todo: why does just "slots" not work here? (see http://qt-project.org/forums/viewthread/18432)
    void init();
    void cleanup();

    // use cases:
    void testInstalledAppStructure();
    void testInstalledAppSetInitialItemText();
    void test_buildMenu_ShowConfigureMenuItem();

    // GUI (manual / interactive):
    void testConfigDialogAllInstalled();
    void testConfigDialogSomeNotInstalled();
    void testConfigDialogNotInstalled1Service2Items();
    void test_buildMenu_WithQActions_interative1();

public:
    static const bool enableInteractiveTests = true;
};

void KMoreToolsTest2::init()
{
}

void KMoreToolsTest2::cleanup()
{
}

/**
 * NOTE: we assume kate is installed
 */
void KMoreToolsTest2::testInstalledAppStructure()
{
    KMoreTools kmt(_("unittest-kmoretools/2"));
    const auto kateApp = kmt.registerServiceByDesktopEntryName(_("kate"));
    const auto menuBuilder = kmt.menuBuilder();
    menuBuilder->addMenuItem(kateApp);
    QString s = menuBuilder->menuStructureAsString(false);
    qDebug() << s;
    QCOMPARE(s, QString(_("|main|:kate.|more|:|notinstalled|:")));
}

/**
 * NOTE: we assume kate is installed
 * and that the translated Name is "Kate"
 */
void KMoreToolsTest2::testInstalledAppSetInitialItemText()
{
    KMoreTools kmt(_("unittest-kmoretools/2"));
    const auto kateApp = kmt.registerServiceByDesktopEntryName(_("kate"));
    const auto menuBuilder = kmt.menuBuilder();
    auto kateAppItem = menuBuilder->addMenuItem(kateApp);
    kateAppItem->setInitialItemText(kateApp->formatString(_("$Name in super-user mode")));
    auto action = kateAppItem->action();
    QVERIFY(action); // because kate is installed;
    QCOMPARE(action->text(), QString(_("Kate in super-user mode")));
}


bool menuAtLeastOneActionWithText(const QMenu* menu, const QString& text)
{
    Q_FOREACH(auto a, menu->actions())
    {
        if (a->text() == text) {
            return true;
        }
    }

    return false;
}

bool menuAtLeastNoActionWithText(const QMenu* menu, const QString& text)
{
    Q_FOREACH(auto a, menu->actions())
    {
        if (a->text() == text) {
            qDebug() << a->text();
            return false;
        }
    }

    return true;
}

/**
 * NOTE: we assume kate is installed
 */
void KMoreToolsTest2::test_buildMenu_ShowConfigureMenuItem()
{
    {
        KMoreTools kmt(_("unittest-kmoretools/2"));
        const auto kateApp = kmt.registerServiceByDesktopEntryName(_("kate"));
        // porcelain: other (interactive) tests will reuse the kmt id so we make sure that
        // no user configurment disburbs this test:
        const auto menuBuilder = kmt.menuBuilder(_("porcelain"));
        menuBuilder->addMenuItem(kateApp);
        QMenu menu;
        menuBuilder->buildByAppendingToMenu(&menu); // == KMoreTools::ConfigureDialogAccessible_Always
        qDebug() << menuBuilder->menuStructureAsString(true);
        QVERIFY(menuAtLeastOneActionWithText(&menu, _("Configure..."))); // "Kate", "Separator", "Configure..."

        {
            menu.clear();
            menuBuilder->buildByAppendingToMenu(&menu, KMoreTools::ConfigureDialogAccessible_Defensive);
            QVERIFY(menuAtLeastNoActionWithText(&menu, _("Configure...")));
        }
    }

    {
        KMoreTools kmt(_("unittest-kmoretools/2"));
        const auto kateApp = kmt.registerServiceByDesktopEntryName(_("kate"));
        const auto mynotInstalledApp = kmt.registerServiceByDesktopEntryName(_("mynotinstalledapp"));
        const auto menuBuilder = kmt.menuBuilder(_("porcelain"));
        menuBuilder->addMenuItem(kateApp);
        menuBuilder->addMenuItem(mynotInstalledApp);
        QMenu menu;
        menuBuilder->buildByAppendingToMenu(&menu); // == KMoreTools::ConfigureDialogAccessible_Always

        auto doAssert = [](QMenu* menu) {
            QCOMPARE(menu->actions().count(), 3); // "Kate", "Separator", "More..."
            auto moreMenu = menu->actions()[2]->menu();
            QCOMPARE(moreMenu->actions().count(), 4); // "Not-installed-section", "Not installed app", "Separator", "Configure menu..."
            auto configureMenu = moreMenu->actions()[3];
            QCOMPARE(configureMenu->data().toString(), QString(_("configureItem")));
        };

        doAssert(&menu);

        {
            menu.clear();
            menuBuilder->buildByAppendingToMenu(&menu, KMoreTools::ConfigureDialogAccessible_Defensive);
            doAssert(&menu); // = same as _Always because there is one not-installed item
        }
    }
}

void KMoreToolsTest2::test_buildMenu_WithQActions_interative1()
{
    if (!enableInteractiveTests) {
        QSKIP("enableInteractiveTests is false");
    }

    KMoreTools kmt(_("unittest-kmoretools/qactions")); // todo: disable copy-ctor!?

    auto f = [&kmt](QString title) { // NOTE: capture by reference! see https://en.wikipedia.org/wiki/Anonymous_function
        const auto menuBuilder = kmt.menuBuilder();
        menuBuilder->clear();
        QMenu menu;
        menuBuilder->addMenuItem(new QAction(_("Hallo 1"), &menu), _("id1"));
        menuBuilder->addMenuItem(new QAction(_("Hallo 2"), &menu), _("id2"));
        menuBuilder->addMenuItem(new QAction(_("Hallo 3"), &menu), _("id3"));

        menuBuilder->buildByAppendingToMenu(&menu);
        menuBuilder->showConfigDialog(title);
    };

    f(_("test_buildMenu_WithQActions 1"));
    //f(_("test_buildMenu_WithQActions 2"));
}

QDebug operator<< (QDebug d, const KmtMenuItemDto &m) {
    d << "id:" << m.id << ", section:" << m.menuSection << ", isInstalled:" << m.isInstalled;
    return d;
}

void testConfigDialogImpl(bool withNotInstalled, bool withMultipleItemsPerNotInstalledService, const QString& description)
{
    if (KMoreToolsTest2::enableInteractiveTests) {
        KMoreTools kmt(_("unittest-kmoretools/2"));
        const auto kateApp = kmt.registerServiceByDesktopEntryName(_("kate"));
        const auto gitgApp = kmt.registerServiceByDesktopEntryName(_("gitg"));
        const auto notinstApp = kmt.registerServiceByDesktopEntryName(_("mynotinstalledapp"));
        const auto notinstApp2 = kmt.registerServiceByDesktopEntryName(_("mynotinstapp2"));
        notinstApp2->setHomepageUrl(QUrl(_("http://www.kde.org")));
        const auto menuBuilder = kmt.menuBuilder();
        menuBuilder->addMenuItem(kateApp);
        menuBuilder->addMenuItem(gitgApp);
        if (withNotInstalled) {
            auto item1 = menuBuilder->addMenuItem(notinstApp);
            item1->setInitialItemText(notinstApp->formatString(_("$Name - item 1")));

            menuBuilder->addMenuItem(notinstApp2);

            if (withMultipleItemsPerNotInstalledService) {
                auto item3 = menuBuilder->addMenuItem(notinstApp);
                item3->setInitialItemText(notinstApp->formatString(_("$Name - second item")));
            }
        }
        auto i1 = menuBuilder->addMenuItem(kateApp, KMoreTools::MenuSection_More);
        i1->setId(_("kate1"));
        i1->setInitialItemText(_("Kate more"));
        auto i2 = menuBuilder->addMenuItem(gitgApp, KMoreTools::MenuSection_More);
        i2->setId(_("gitg1"));
        i2->setInitialItemText(_("gitg more"));
        menuBuilder->showConfigDialog(description);

        // show resulting menu
        auto dlg = new QDialog();
        auto button = new QPushButton(_("Test the menu"), dlg);
        auto menu = new QMenu(dlg);
        menuBuilder->buildByAppendingToMenu(menu);
        button->setMenu(menu); // TODO: connect to the button click signal to always rebuild the menu
        auto label = new QLabel(_("Test the menu and hit Esc to exit if you are done. Note that changes made via the Configure dialog will have no immediate effect."), dlg);
        label->setWordWrap(true);
        auto layout = new QHBoxLayout();
        layout->addWidget(button);
        layout->addWidget(label);
        dlg->setLayout(layout);
        QObject::connect(dlg, &QDialog::finished, dlg, [dlg]() {
            qDebug () << "delete dlg;";
            delete dlg;
        });
        dlg->exec();
    }
}

void KMoreToolsTest2::testConfigDialogAllInstalled()
{
    if (!enableInteractiveTests) {
        QSKIP("enableInteractiveTests is false");
    }

    testConfigDialogImpl(false, false, _("TEST all installed"));
}

void KMoreToolsTest2::testConfigDialogSomeNotInstalled()
{
    if (!enableInteractiveTests) {
        QSKIP("enableInteractiveTests is false");
    }

    testConfigDialogImpl(true, false, _("TEST some not installed"));
}

void KMoreToolsTest2::testConfigDialogNotInstalled1Service2Items()
{
    if (!enableInteractiveTests) {
        QSKIP("enableInteractiveTests is false");
    }

    testConfigDialogImpl(true, true, _("TEST more than one item for one not installed service"));
}

QTEST_MAIN(KMoreToolsTest2)

#include "kmoretoolstest2.moc"