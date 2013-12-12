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
 */

#include "approver.h"
#include <QDebug>

#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ChannelClassSpec>
#include <TelepathyQt/ClientRegistrar>
#include <TelepathyQt/TextChannel>

Approver::Approver(QObject* parent)
: QObject(parent), Tp::AbstractClientApprover(channelFilters())
{
}

Approver::~Approver()
{
}

Tp::ChannelClassSpecList Approver::channelFilters() const
{
    Tp::ChannelClassSpecList specList;
    specList << Tp::ChannelClassSpec::textChat();

    return specList;
}

void Approver::addDispatchOperation(const Tp::MethodInvocationContextPtr<> &context,
                                        const Tp::ChannelDispatchOperationPtr &dispatchOperation)
{
    bool willHandle = false;

    QList<Tp::ChannelPtr> channels = dispatchOperation->channels();
    Q_FOREACH (Tp::ChannelPtr channel, channels) {
        // Text Channel
        Tp::TextChannelPtr textChannel = Tp::TextChannelPtr::dynamicCast(channel);
        if (!textChannel.isNull()) {
            // right now we are not using any of the text channel's features in the approver
            // so no need to call becomeReady() on it.
            willHandle = true;
        }
    }

    if (willHandle) {
        mDispatchOps.append(dispatchOperation);
    }

    context->setFinished();

    // check if we need to approve channels already or if we should wait.
    processChannels();
}

void Approver::processChannels()
{
    Q_FOREACH (Tp::ChannelDispatchOperationPtr dispatchOperation, mDispatchOps) {
        QList<Tp::ChannelPtr> channels = dispatchOperation->channels();
        Q_FOREACH (Tp::ChannelPtr channel, channels) {
            // approve only text channels
            Tp::TextChannelPtr textChannel = Tp::TextChannelPtr::dynamicCast(channel);
            if (textChannel.isNull()) {
                continue;
            }

            if (dispatchOperation->possibleHandlers().contains(TELEPHONY_SERVICE_HANDLER)) {
                dispatchOperation->handleWith(TELEPHONY_SERVICE_HANDLER);
                mDispatchOps.removeAll(dispatchOperation);
            }
            // FIXME: this shouldn't happen, but in any case, we need to check what to do when
            // the phone app client is not available
        }
    }
}

