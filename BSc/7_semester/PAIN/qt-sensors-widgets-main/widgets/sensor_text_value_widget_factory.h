//
// Created by damiankolaska on 11/19/21.
//

#ifndef QT_SENSORS_WIDGETS_SENSOR_TEXT_VALUE_WIDGET_FACTORY_H
#define QT_SENSORS_WIDGETS_SENSOR_TEXT_VALUE_WIDGET_FACTORY_H

#include "sensor_model/sensor_model.h"


class SensorTextValueWidgetFactory {
public:
    SensorTextValueWidgetFactory(SensorModel* sensorModel): sensorModel(sensorModel) {}

    SensorTextValueWidget* create(QModelIndex device, int sensorId, QWidget* parent = nullptr) {
        auto* sensorValue = new SensorTextValueWidget(device, sensorId, sensorModel, parent);
        return sensorValue;
    }

private:
    SensorModel* sensorModel;
};

#endif //QT_SENSORS_WIDGETS_SENSOR_TEXT_VALUE_WIDGET_FACTORY_H
