//
// Created by damiankolaska on 11/24/21.
//

#ifndef QT_SENSORS_WIDGETS_PROCESS_LIST_MODEL_H
#define QT_SENSORS_WIDGETS_PROCESS_LIST_MODEL_H

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QRegExp>

#include "process_list_delegate.h"
#include "sensor_model/command_executor.h"

class ProcessListModel : public QAbstractListModel {
    Q_OBJECT
public:
    ProcessListModel(QObject* parent = nullptr);

    void init() {
        updateModelFromRawOutput(CommandExecutor::execute(processListCommand));
    }

    QSortFilterProxyModel* getProxy() { return proxyModel; }

    int rowCount(const QModelIndex &parent) const override {
        return modelData.size();
    }

    int columnCount(const QModelIndex &parent) const override {
        if (modelData.size() == 0)
            return 0;
        return modelData[0].size();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void setSearchExpression(const QString& newSearchExpression) {
        proxyModel->setFilterRegExp(QRegExp(newSearchExpression,
                                            Qt::CaseInsensitive,
                                            QRegExp::PatternSyntax::FixedString));
    };
    void updateModel() {
        updateModelFromRawOutput(process->readAllStandardOutput());
    }

private:
    QSortFilterProxyModel* proxyModel;
    QVector<QVector<QVariant>> modelData;
    QVector<QString> header {"Command", "User", "PID", "%CPU", "%MEM", "Time"};

    QProcess* process;
    const QString processListCommand = "ps axo comm,user,pid,%cpu,%mem,time";
    const float probingDelay = 5000;

    void updateModelFromRawOutput(const QString& rawOutput);
};

#endif //QT_SENSORS_WIDGETS_PROCESS_LIST_MODEL_H
