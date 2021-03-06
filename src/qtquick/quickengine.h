/*
 * Copyright (C) 2016 Dan Leinir Turthra Jensen <admin@leinir.dk>
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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

/**
 * @short Encapsulates a KNSCore::Engine for use in Qt Quick
 *
 * This class takes care of initialisation of a KNSCore::Engine when assigned a config file.
 * The actual KNSCore:Engine can be read through the Engine::engine property.
 *
 * @see ItemsModel
 */
class Engine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString configFile READ configFile WRITE setConfigFile NOTIFY configFileChanged)
    Q_PROPERTY(QObject* engine READ engine NOTIFY engineChanged)
public:
    explicit Engine(QObject* parent = nullptr);
    virtual ~Engine();

    QString configFile() const;
    void setConfigFile(const QString& newFile);
    Q_SIGNAL void configFileChanged();

    QObject* engine() const;
    Q_SIGNAL void engineChanged();

Q_SIGNALS:
    void message(const QString& message);
    void idleMessage(const QString& message);
    void busyMessage(const QString& message);
    void errorMessage(const QString& message);

private:
    class Private;
    Private* d;
};

#endif//ENGINE_H
