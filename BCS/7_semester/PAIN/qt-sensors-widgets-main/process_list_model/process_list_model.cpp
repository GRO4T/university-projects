//
// Created by damiankolaska on 11/30/21.
//

#include <QTimer>
#include <QProcess>

#include "process_list_model.h"


ProcessListModel::ProcessListModel(QObject *parent) :
        QAbstractListModel(parent) {
    QTimer *timer = new QTimer(this);
    process = new QProcess(this);
    QObject::connect(timer, &QTimer::timeout, this, [this]() {
        process->start(processListCommand);
    });
    QObject::connect(process, &QProcess::readyReadStandardOutput, this, &ProcessListModel::updateModel);
    timer->start(probingDelay);
    proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(this);
    proxyModel->setFilterKeyColumn(0);
}

QVariant ProcessListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        if (section >= header.size())
            return QVariant();
        return header[section];
    }
    return QVariant();
}

QVariant ProcessListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() >= modelData.size() or index.column() >= modelData[index.row()].size())
        return QVariant();
    return modelData[index.row()][index.column()];
}

void ProcessListModel::updateModelFromRawOutput(const QString &rawOutput) {
    QStringList lines = rawOutput.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    QVector<QVector<QVariant>> newModelData;
    for (int i = 1; i < lines.size(); ++i) {
        QVector<QVariant> proc;
        for (const auto &elem : lines[i].split(QRegExp("\\s+"), QString::SkipEmptyParts)) {
            proc.push_back(elem);
        }
        newModelData.push_back(proc);
    }
    qSort(newModelData);
    {
        beginResetModel();
        modelData = newModelData;
        endResetModel();
    }
}
