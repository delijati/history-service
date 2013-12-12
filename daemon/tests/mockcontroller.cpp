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
 * Authors: 
 *  Tiago Salem Herrmann <tiago.herrmann@canonical.com>
 *  Gustavo Pichorim Boiko <gustavo.boiko@canonical.com>
 */

#include "mockcontroller.h"

#define MOCK_SERVICE "com.canonical.MockConnection"
#define MOCK_OBJECT "/com/canonical/MockConnection"
#define MOCK_INTERFACE "com.canonical.MockConnection"

MockController::MockController(QObject *parent) :
    QObject(parent),
    mMockInterface(MOCK_SERVICE, MOCK_OBJECT, MOCK_INTERFACE)
{
    QDBusConnection::sessionBus().connect(MOCK_SERVICE, MOCK_OBJECT, MOCK_INTERFACE, "MessageSent", this, SIGNAL(MessageSent(QString, QVariantMap)));
}

MockController *MockController::instance()
{
    static MockController *self = new MockController();
    return self;
}

void MockController::placeIncomingMessage(const QString &message, const QVariantMap &properties)
{
    mMockInterface.call("PlaceIncomingMessage", message, properties);
}

