/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * Authors:
 *  Gustavo Pichorim Boiko <gustavo.boiko@canonical.com>
 *
 * This file is part of history-service.
 *
 * history-service is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * history-service is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "connection.h"
#include "mockconnectiondbus.h"
#include "mockconnectionadaptor.h"
#include "types.h"

Q_DECLARE_METATYPE(QList< QVariantMap >)

MockConnectionDBus::MockConnectionDBus(MockConnection *parent) :
    QObject(parent), mAdaptor(0), mConnection(parent)
{

    connect(mConnection,
            SIGNAL(messageSent(QString,QVariantMap)),
            SIGNAL(MessageSent(QString,QVariantMap)));
    qDBusRegisterMetaType<QList<QVariantMap> >();
    connectToBus();
}

bool MockConnectionDBus::connectToBus()
{
    bool ok = QDBusConnection::sessionBus().registerService("com.canonical.MockConnection");
    if (!ok) {
        return false;
    }

    if (!mAdaptor) {
        mAdaptor = new MockConnectionAdaptor(this);
    }

    return QDBusConnection::sessionBus().registerObject("/com/canonical/MockConnection", this);
}

void MockConnectionDBus::PlaceIncomingMessage(const QString &message, const QVariantMap &properties)
{
    mConnection->placeIncomingMessage(message, properties);
}

void MockConnectionDBus::PlaceCall(const QVariantMap &properties)
{
    mConnection->placeCall(properties);
}

void MockConnectionDBus::HangupCall(const QString &callerId)
{
    mConnection->hangupCall(callerId);
}

void MockConnectionDBus::SetCallState(const QString &phoneNumber, const QString &state)
{
    mConnection->setCallState(phoneNumber, state);
}
