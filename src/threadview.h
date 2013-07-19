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

#ifndef HISTORY_THREADVIEW_H
#define HISTORY_THREADVIEW_H

#include "types.h"
#include <QObject>

namespace History
{

class ThreadView : public QObject
{
    Q_OBJECT

public:
    ThreadView() {}
    virtual ~ThreadView() {}

    virtual Threads nextPage() = 0;
    virtual bool isValid() const = 0;

Q_SIGNALS:
    void threadsAdded(const History::Threads &threads);
    void threadsModified(const History::Threads &threads);
    void threadsRemoved(const History::Threads &threads);
};

}

#endif // HISTORY_THREADVIEW_H
