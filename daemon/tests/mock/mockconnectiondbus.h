/**
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3, as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranties of MERCHANTABILITY,
 * SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Gustavo Pichorim Boiko <gustavo.boiko@canonical.com>
 */

#ifndef MOCKCONNECTIONDBUS_H
#define MOCKCONNECTIONDBUS_H

#include <QDBusContext>
#include <QObject>
#include "types.h"

class MockConnectionAdaptor;
class MockConnection;

class MockConnectionDBus : public QObject, public QDBusContext
{
    Q_OBJECT
public:
    explicit MockConnectionDBus(MockConnection *parent);

    bool connectToBus();
    void PlaceIncomingMessage(const QString &message, const QVariantMap &properties);

Q_SIGNALS:
    // signals that will be relayed into the bus
    void MessageSent(const QString &mesasge, const QVariantMap &properties);

private:
    MockConnectionAdaptor *mAdaptor;
    MockConnection *mConnection;
};

#endif // MOCKCONNECTIONDBUS_H
