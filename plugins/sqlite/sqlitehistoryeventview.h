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

#ifndef SQLITEHISTORYEVENTVIEW_H
#define SQLITEHISTORYEVENTVIEW_H

#include "eventview.h"
#include "event.h"
#include "types.h"
#include <QSqlQuery>

class SQLiteHistoryReader;

class SQLiteHistoryEventView : public History::EventView
{
    Q_OBJECT

public:
    SQLiteHistoryEventView(SQLiteHistoryReader *reader,
                          History::EventType type,
                          const History::SortPtr &sort,
                          const History::FilterPtr &filter);

    History::Events nextPage();
    bool isValid() const;

protected:


private:
    History::EventType mType;
    History::SortPtr mSort;
    History::FilterPtr mFilter;
    QSqlQuery mQuery;
    int mPageSize;
    SQLiteHistoryReader *mReader;
};

#endif // SQLITEHISTORYEVENTVIEW_H