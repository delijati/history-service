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

#include "sqlitehistorythreadview.h"
#include "sqlitedatabase.h"
#include "sqlitehistoryplugin.h"
#include "sort.h"
#include <QDateTime>
#include <QDebug>
#include <QSqlError>

SQLiteHistoryThreadView::SQLiteHistoryThreadView(SQLiteHistoryPlugin *plugin,
                                                 History::EventType type,
                                                 const History::Sort &sort,
                                                 const History::Filter &filter)
    : History::PluginThreadView(), mPlugin(plugin), mType(type), mSort(sort),
      mFilter(filter), mPageSize(15), mQuery(SQLiteDatabase::instance()->database()), mOffset(0), mValid(true)
{
    qDebug() << __PRETTY_FUNCTION__;
    mTemporaryTable = QString("threadview%1%2").arg(QString::number((qulonglong)this), QDateTime::currentDateTimeUtc().toString("yyyyMMddhhmmsszzz"));
    mQuery.setForwardOnly(true);

    // FIXME: validate the filter
    QString condition = filter.toString();
    QString order;
    if (!sort.sortField().isNull()) {
        order = QString("ORDER BY %1 %2").arg(sort.sortField(), sort.sortOrder() == Qt::AscendingOrder ? "ASC" : "DESC");
        // FIXME: check case sensitiviy
    }

    QString queryText = QString("CREATE TEMP TABLE %1 AS ").arg(mTemporaryTable);
    queryText += mPlugin->sqlQueryForThreads(type, condition, order);

    // create the temporary table
    if (!mQuery.exec(queryText)) {
        qCritical() << "Error:" << mQuery.lastError() << mQuery.lastQuery();
        mValid = false;
        Q_EMIT Invalidated();
        return;
    }

    mQuery.exec(QString("SELECT count(*) FROM %1").arg(mTemporaryTable));
    mQuery.next();
}

SQLiteHistoryThreadView::~SQLiteHistoryThreadView()
{
    if (!mQuery.exec(QString("DROP TABLE IF EXISTS %1").arg(mTemporaryTable))) {
        qCritical() << "Error:" << mQuery.lastError() << mQuery.lastQuery();
        return;
    }
}

QList<QVariantMap> SQLiteHistoryThreadView::NextPage()
{
    qDebug() << __PRETTY_FUNCTION__;
    QList<QVariantMap> threads;

    // now prepare for selecting from it
    mQuery.prepare(QString("SELECT * FROM %1 LIMIT %2 OFFSET %3").arg(mTemporaryTable,
                                                                      QString::number(mPageSize), QString::number(mOffset)));
    if (!mQuery.exec()) {
        qCritical() << "Error:" << mQuery.lastError() << mQuery.lastQuery();
        mValid = false;
        Q_EMIT Invalidated();
        return threads;
    }

    threads = mPlugin->parseThreadResults(mType, mQuery);
    mOffset += mPageSize;
    mQuery.clear();

    return threads;
}

bool SQLiteHistoryThreadView::IsValid() const
{
    return mValid;
}
