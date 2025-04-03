#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QObject>
#include <QWidget>

#include <QStyledItemDelegate>
#include <QPainter>
#include <QDate>

class CustomDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit CustomDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setHighlightedDates(const QList<QDate> &dates);

private:
    QList<QDate> highlightedDates;
};




#endif // CUSTOMDELEGATE_H
