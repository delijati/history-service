/*
 * Copyright (C) 2015 Canonical, Ltd.
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

#include <QtCore/QObject>
#include <QtTest/QtTest>
#include <TelepathyQt/PendingAccount>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/Account>
#include "telepathytest.h"
#include "telepathyhelper_p.h"

void TelepathyTest::initialize()
{
    Tp::registerTypes();

    QSignalSpy spy(TelepathyHelper::instance(), SIGNAL(setupReady()));
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, DEFAULT_TIMEOUT);

    // just in case, remove any existing account that might be a leftover from
    // previous test runs
    Q_FOREACH(const Tp::AccountPtr &account, TelepathyHelper::instance()->accounts()) {
        QVERIFY(removeAccount(account));
    }

    // create an account manager instance to help testing
    Tp::Features accountFeatures;
    accountFeatures << Tp::Account::FeatureCore
                    << Tp::Account::FeatureProtocolInfo;
    Tp::Features contactFeatures;
    contactFeatures << Tp::Contact::FeatureAlias
                    << Tp::Contact::FeatureAvatarData
                    << Tp::Contact::FeatureAvatarToken
                    << Tp::Contact::FeatureCapabilities
                    << Tp::Contact::FeatureSimplePresence;
    Tp::Features connectionFeatures;
    connectionFeatures << Tp::Connection::FeatureCore
                       << Tp::Connection::FeatureSelfContact
                       << Tp::Connection::FeatureSimplePresence;

    Tp::ChannelFactoryPtr channelFactory = Tp::ChannelFactory::create(QDBusConnection::sessionBus());
    channelFactory->addCommonFeatures(Tp::Channel::FeatureCore);

    mAccountManager = Tp::AccountManager::create(
            Tp::AccountFactory::create(QDBusConnection::sessionBus(), accountFeatures),
            Tp::ConnectionFactory::create(QDBusConnection::sessionBus(), connectionFeatures),
            channelFactory,
            Tp::ContactFactory::create(contactFeatures));

    mReady = false;
    connect(mAccountManager->becomeReady(Tp::AccountManager::FeatureCore),
            &Tp::PendingOperation::finished, [=]{
        mReady = true;
    });

    QTRY_VERIFY(mReady);

    // give some time for telepathy stuff to settle
    QTest::qWait(1000);
}

void TelepathyTest::doCleanup()
{
    // remove all accounts on every test to prevent garbage to go from one test to another
    Q_FOREACH(const Tp::AccountPtr &account, mAccounts) {
        QVERIFY(removeAccount(account));
    }
    QVERIFY(mAccounts.isEmpty());
}


Tp::AccountPtr TelepathyTest::addAccount(const QString &manager, const QString &protocol, const QString &displayName, const QVariantMap &parameters)
{
    bool finished = false;
    Tp::AccountPtr account;
    connect(mAccountManager->createAccount(manager, protocol, displayName, parameters), &Tp::PendingOperation::finished,
            [&](Tp::PendingOperation *op) {
        Tp::PendingAccount *pa = qobject_cast<Tp::PendingAccount*>(op);
        if (op->isError() || !pa) {
            qCritical() << "Failed to create account:" << op->errorName() << op->errorMessage();
            finished = true;
            return;
        }

        account = pa->account();

        connect(account->setEnabled(true), &Tp::PendingOperation::finished, [&] {
            Tp::Presence presence(Tp::ConnectionPresenceTypeAvailable, "online", "online");
            account->setRequestedPresence(presence);
            finished = true;
        });
    });

    while (!finished) {
        QTest::qWait(100);
    }
    mAccounts << account;
    return account;
}

bool TelepathyTest::removeAccount(const Tp::AccountPtr &account)
{
    bool success = false;
    bool finished = false;

    connect(account->remove(), &Tp::PendingOperation::finished,
            [&](Tp::PendingOperation *op) {
        success = !op->isError();
        finished = true;
    });

    while (!finished) {
        QTest::qWait(100);
    }
    if (success) {
        mAccounts.removeAll(account);
    }
    return success;
}


QList<Tp::AccountPtr> TelepathyTest::accounts() const
{
    return mAccounts;
}

void TelepathyTest::cleanup()
{
    doCleanup();
}
