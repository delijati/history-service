#include <HistoryManager>
#include <HistoryFilter>
#include <HistoryIntersectionFilter>
#include <HistoryThread>
#include <TextItem>
#include <VoiceItem>
#include <QCoreApplication>
#include <QDebug>

void printItem(const HistoryItemPtr &item)
{
    QString extraInfo;
    TextItemPtr textItem;
    VoiceItemPtr voiceItem;

    switch (item->type()) {
    case HistoryItem::ItemTypeText:
        textItem = item.staticCast<TextItem>();
        extraInfo = QString(" message: %1").arg(textItem->message());
        break;
    case HistoryItem::ItemTypeVoice:
        voiceItem = item.staticCast<VoiceItem>();
        extraInfo = QString(" missed: %1 duration: %2").arg(voiceItem->missed() ? "yes" : "no", voiceItem->duration().toString());
        break;
    }

    qDebug() << qPrintable(QString("    * Item: accountId: %1 threadId: %2 itemId: %3 sender: %4 timestamp: %5 newItem: %6")
                .arg(item->accountId(), item->threadId(), item->itemId(), item->sender(), item->timestamp().toString(),
                     item->newItem() ? "yes" : "no"));
    qDebug() << qPrintable(QString("      %1").arg(extraInfo));
}

void printThread(const HistoryThreadPtr &thread)
{
    QString type = "Unknown";
    switch (thread->type()) {
    case HistoryItem::ItemTypeText:
        type = "Text";
        break;
    case HistoryItem::ItemTypeVoice:
        type = "Voice";
        break;
    }

    qDebug() << qPrintable(QString("%1 thread - accountId: %2 threadId: %3 count: %4 unreadCount: %5").arg(type,
                                                                                                thread->accountId(),
                                                                                                thread->threadId(),
                                                                                                QString::number(thread->count()),
                                                                                                QString::number(thread->unreadCount())));
    qDebug() << qPrintable(QString("    Participants: %1").arg(thread->participants().join(", ")));
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    HistoryManager *manager = HistoryManager::instance();

    QList<HistoryItem::ItemType> itemTypes;
    itemTypes << HistoryItem::ItemTypeText << HistoryItem::ItemTypeVoice;

    Q_FOREACH(HistoryItem::ItemType type, itemTypes) {
        Q_FOREACH(const HistoryThreadPtr &thread, manager->queryThreads(type)) {
            printThread(thread);

            // now print the items for this thread
            HistoryIntersectionFilter filter;
            filter.append(HistoryFilter("threadId", thread->threadId()));
            filter.append(HistoryFilter("accountId", thread->accountId()));
            Q_FOREACH(const HistoryItemPtr &item, manager->queryItems(type, HistorySort(), filter)) {
                printItem(item);
            }
        }
    }
}
