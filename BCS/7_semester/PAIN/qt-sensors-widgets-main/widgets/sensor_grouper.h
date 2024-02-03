//
// Created by damiankolaska on 11/19/21.
//

#ifndef QT_SENSORS_WIDGETS_SENSOR_GROUPER_H
#define QT_SENSORS_WIDGETS_SENSOR_GROUPER_H

#include <QDataWidgetMapper>
#include <QLineEdit>
#include <QGroupBox>
#include <QScrollArea>
#include <QScrollBar>

#include "sensor_text_value_widget.h"
#include "sensor_text_value_widget_factory.h"
#include "sensor_model/device.h"


class SensorGrouper : public QGroupBox {
public:
    SensorGrouper(SensorModel* sensorModel, int deviceId, QWidget* parent = nullptr): QGroupBox(parent) {
        const auto& deviceIndex = sensorModel->index(deviceId, 0, QModelIndex());
        const auto& deviceName = sensorModel->data(deviceIndex, SensorModel::Roles::DeviceRole);
        setMinimumHeight(100);
        setMaximumHeight(500);
        setTitle(deviceName.toString());
        SensorTextValueWidgetFactory sensorValueFactory(sensorModel);
        QHBoxLayout* hbox = new QHBoxLayout(this);
        QVBoxLayout* vbox0 = new QVBoxLayout(this);
        QVBoxLayout* vbox1 = new QVBoxLayout(this);
        for (int i = 0; i < sensorModel->rowCount(deviceIndex); ++i) {
            auto* sensorWidget = sensorValueFactory.create(deviceIndex, i);
            sensorTextWidgets.push_back(sensorWidget);
            vbox0->addWidget(sensorWidget->getLabel());
            vbox1->addWidget(sensorWidget->getValue());
        }
        hbox->addLayout(vbox0);
        hbox->addLayout(vbox1);
        hbox->setContentsMargins(5, 5, 20, 5);
        QScrollArea* scrollArea = new QScrollArea(this);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));
        QFrame* frame = new QFrame(this);
        frame->setLayout(hbox);
        scrollArea->setWidget(frame);
        QVBoxLayout* box = new QVBoxLayout(this);
        box->setMargin(0);
        box->addWidget(scrollArea);
        setLayout(box);
    }

private:
    QVector<SensorTextValueWidget*> sensorTextWidgets;
};

#endif //QT_SENSORS_WIDGETS_SENSOR_GROUPER_H
