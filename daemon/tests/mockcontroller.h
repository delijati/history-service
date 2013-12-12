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

#ifndef MOCKCONTROLLER_H
#define MOCKCONTROLLER_H

#include <QObject>
#include <QDBusInterface>

class MockController : public QObject
{
    Q_OBJECT
public:
    static MockController *instance();

Q_SIGNALS:
    void MessageSent(const QString &message, const QVariantMap &properties);

public Q_SLOTS:
    void placeIncomingMessage(const QString &message, const QVariantMap &properties);

private:
    explicit MockController(QObject *parent = 0);
    QDBusInterface mMockInterface;
};

#endif // MOCKCONTROLLER_H
