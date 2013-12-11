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
 * Authors: Tiago Salem Herrmann <tiago.herrmann@canonical.com>
            Gustavo Pichorim Boiko <gustavo.boiko@canonical.com>
 */

#ifndef MOCKCALLCHANNEL_H
#define MOCKCALLCHANNEL_H

#include <QObject>

#include <TelepathyQt/Constants>
#include <TelepathyQt/BaseChannel>
#include <TelepathyQt/BaseCall>
#include <TelepathyQt/Types>

#include "connection.h"

class MockConnection;

class MockCallChannel : public QObject
{
    Q_OBJECT
public:
    MockCallChannel(MockConnection *conn, QString phoneNumber, uint targetHandle, QObject *parent = 0);
    ~MockCallChannel();
    Tp::BaseChannelPtr baseChannel();

    void onHangup(uint reason, const QString &detailedReason, const QString &message, Tp::DBusError* error);
    void onAccept(Tp::DBusError*);
    void onMuteStateChanged(const Tp::LocalMuteState &state, Tp::DBusError *error);
    void onHoldStateChanged(const Tp::LocalHoldState &state, const Tp::LocalHoldStateReason &reason, Tp::DBusError *error);
    void onDTMFStartTone(uchar event, Tp::DBusError *error);
    void onDTMFStopTone(Tp::DBusError *error);

private Q_SLOTS:
    void setCallState(const QString &state);
    void init();


    void onOfonoMuteChanged(bool mute);

private:
    QString mObjPath;
    QString mPreviousState;
    bool mIncoming;
    bool mRequestedHangup;
    Tp::BaseChannelPtr mBaseChannel;
    QString mPhoneNumber;
    MockConnection *mConnection;
    uint mTargetHandle;
    Tp::BaseChannelHoldInterfacePtr mHoldIface;
    Tp::BaseCallMuteInterfacePtr mMuteIface;
    Tp::BaseChannelCallTypePtr mCallChannel;
    Tp::BaseCallContentDTMFInterfacePtr mDTMFIface;
    Tp::BaseCallContentPtr mCallContent;

};

#endif // MOCKCALLCHANNEL_H
