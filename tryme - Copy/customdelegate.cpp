#include "CustomDelegate.h"
#include <QCalendarWidget>

CustomDelegate::CustomDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void CustomDelegate::setHighlightedDates(const QList<QDate> &dates) {
    highlightedDates = dates;
}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);

    if (!index.isValid()) return;

    QDate date = index.data(Qt::UserRole).toDate();

    if (highlightedDates.contains(date)) {
        painter->fillRect(option.rect, QColor(255, 165, 0, 100)); // Light Orange color
    }
}
