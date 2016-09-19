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

Q_DECLARE_METATYPE(QList< QVariantMap >)

MockConnectionDBus::MockConnectionDBus(MockConnection *parent) :
    QObject(parent), mAdaptor(0), mConnection(parent)
{
    connect(mConnection,
            SIGNAL(messageRead(QString)),
            SIGNAL(MessageRead(QString)));
    connect(mConnection,
            SIGNAL(messageSent(QString,QVariantMap)),
            SIGNAL(MessageSent(QString,QVariantMap)));
    connect(mConnection,
            SIGNAL(callReceived(QString)),
            SIGNAL(CallReceived(QString)));
    connect(mConnection,
            SIGNAL(callEnded(QString)),
            SIGNAL(CallEnded(QString)));
    connect(mConnection,
            SIGNAL(callStateChanged(QString,QString,QString)),
            SIGNAL(CallStateChanged(QString,QString,QString)));
    connect(mConnection,
            SIGNAL(conferenceCreated(QString)),
            SIGNAL(ConferenceCreated(QString)));
    connect(mConnection,
            SIGNAL(channelMerged(QString)),
            SIGNAL(ChannelMerged(QString)));
    connect(mConnection,
            SIGNAL(channelSplitted(QString)),
            SIGNAL(ChannelSplitted(QString)));
    connect(mConnection,
            SIGNAL(disconnected()),
            SIGNAL(Disconnected()));
    connect(mConnection,
            SIGNAL(destroyed()),
            SIGNAL(Destroyed()));
    qDBusRegisterMetaType<QList<QVariantMap> >();
    mObjectPath = "/com/canonical/MockConnection/" + mConnection->protocolName();
    connectToBus();
}

MockConnectionDBus::~MockConnectionDBus()
{
    QDBusConnection::sessionBus().unregisterObject(mObjectPath, QDBusConnection::UnregisterTree);
}

bool MockConnectionDBus::connectToBus()
{
    if (!mAdaptor) {
        mAdaptor = new MockConnectionAdaptor(this);
    }

    if (!QDBusConnection::sessionBus().registerObject(mObjectPath, this)) {
        return false;
    }

    return QDBusConnection::sessionBus().registerService("com.canonical.MockConnection");
}

void MockConnectionDBus::PlaceIncomingMessage(const QString &message, const QVariantMap &properties)
{
    mConnection->placeIncomingMessage(message, properties);
}

void MockConnectionDBus::PlaceDeliveryReport(const QStringList &recipients, const QString &messageId, const QString &status)
{
    mConnection->placeDeliveryReport(recipients, messageId, status);
}

QString MockConnectionDBus::PlaceCall(const QVariantMap &properties)
{
    return mConnection->placeCall(properties);
}

void MockConnectionDBus::HangupCall(const QString &callerId)
{
    mConnection->hangupCall(callerId);
}

void MockConnectionDBus::SetCallState(const QString &phoneNumber, const QString &state)
{
    mConnection->setCallState(phoneNumber, state);
}

void MockConnectionDBus::SetOnline(bool online)
{
    mConnection->setOnline(online);
}

void MockConnectionDBus::SetPresence(const QString &status, const QString &statusMessage)
{
    Tp::DBusError error;
    mConnection->setPresence(status, statusMessage, &error);
}

void MockConnectionDBus::SetVoicemailIndicator(bool active)
{
    mConnection->setVoicemailIndicator(active);
}

void MockConnectionDBus::SetVoicemailNumber(const QString &number)
{
    mConnection->setVoicemailNumber(number);
}

void MockConnectionDBus::SetVoicemailCount(int count)
{
    mConnection->setVoicemailCount(count);
}

void MockConnectionDBus::SetEmergencyNumbers(const QStringList &numbers)
{
    mConnection->setEmergencyNumbers(numbers);
}

QString MockConnectionDBus::Serial()
{
    return mConnection->serial();
}
