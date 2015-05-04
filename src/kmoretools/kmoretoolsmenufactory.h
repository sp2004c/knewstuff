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

#ifndef KMORETOOLSMENUFACTORY_H
#define KMORETOOLSMENUFACTORY_H

#include <QMenu>
#include <QUrl>
#include <QString>

#include "knewstuff_export.h"

class KMoreTools;
class KMoreToolsService;

/**
 * This is the class with the highest abstraction in KMoreTools.
 *
 * Creates a QMenu from a list of grouping names,
 * see KMoreToolsPresets::registerServicesByGroupingNames
 *
 * @since 5.10
 */
class KNEWSTUFF_EXPORT KMoreToolsMenuFactory
{
public:
    /**
     * see KMoreTools::KMoreTools
     */
    KMoreToolsMenuFactory(const QString& uniqueId);

    virtual ~KMoreToolsMenuFactory();

    /**
     * First, an empty QMenu is created. Then, for each grouping name
     * menu items will be created an appended to the menu.
     *
     * For each grouping name there might be special handlings that take the
     * given @p url (may be empty) into account if needed.
     *
     * Furthermore, some selected menu items will be put into the "More"
     * menu section by default.
     *
     * "more:"
     * -------
     * There is a special grouping name "more:" (note the colon). If this name
     * is given in the list, all further groupings are put into the More
     * section by default.
     * (todo later: if needed this could be made configurable more fine-grained)
     *
     * NOTE that this method overrides a previously created QMenu* instance
     * of the same KMoreToolsMenuFactory instance. (todo: solve this or
     * rename the class?)
     *
     * @returns the created QMenu which includes a Main and (maybe) a More
     * section and an item that starts configure dialog where the user can
     * configure the menu (see KMoreTools).
     */
    QMenu* createMenuFromGroupingNames(const QStringList& groupingNames,
                                       const QUrl& url);

private:
    // Note that this object must live long enough in case the user opens
    // the "Configure..." dialog
    KMoreTools* m_kmt = nullptr;

    QMenu* m_menu = nullptr;
};

#endif