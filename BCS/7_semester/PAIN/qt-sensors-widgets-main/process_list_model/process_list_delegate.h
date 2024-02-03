//
// Created by damiankolaska on 11/24/21.
//

#ifndef QT_SENSORS_WIDGETS_PROCESS_LIST_DELEGATE_H
#define QT_SENSORS_WIDGETS_PROCESS_LIST_DELEGATE_H

#include <QStyledItemDelegate>
#include <QApplication>
#include <QStyleOptionViewItem>
#include <QLabel>
#include <QDebug>
#include <QItemDelegate>


class ProcessListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ProcessListDelegate(QObject *parent = nullptr): QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (index.column() == 3 or index.column() == 4) {
            int progress = index.data().toFloat();
            QStyleOptionProgressBar progressBarOption;
            progressBarOption.rect = option.rect;
            progressBarOption.minimum = 0;
            progressBarOption.maximum = 100;
            progressBarOption.progress = progress;
            progressBarOption.text = QString::number(progress) + "%";
            progressBarOption.textVisible = true;

            QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                               &progressBarOption, painter);
        }
        else {
            QStyleOptionViewItem cellItem(option);
            cellItem.rect = option.rect;
            if (index.isValid())
                cellItem.text = index.data().toString();
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem,
                                               &cellItem, painter);
        }
    }

};

#endif //QT_SENSORS_WIDGETS_PROCESS_LIST_DELEGATE_H
