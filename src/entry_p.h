/*
    Copyright (C) 2009 Frederik Gladhorn <gladhorn@kde.org>

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

#ifndef KNEWSTUFF3_KNEWSTUFFENTRY_P_H
#define KNEWSTUFF3_KNEWSTUFFENTRY_P_H

#include <QStringList>
#include "core/entryinternal.h"

namespace KNS3
{

class EntryPrivate : public QSharedData
{
public:
    KNSCore::EntryInternal e;
    static Entry fromInternal(const KNSCore::EntryInternal* internal)
    {
        Entry e;
        e.d->e = *internal;
        return e;
    }
};
}

#endif

