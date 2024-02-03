//
// Created by damiankolaska on 11/19/21.
//

#ifndef QT_SENSORS_WIDGETS_SENSOR_GROUPER_FACTORY_H
#define QT_SENSORS_WIDGETS_SENSOR_GROUPER_FACTORY_H


#include "sensor_grouper.h"


class SensorGrouperFactory {
public:
    SensorGrouperFactory(SensorModel* sensorModel): sensorModel(sensorModel) {}

    SensorGrouper* create(int deviceId, QWidget* parent = nullptr) {
        auto* sensorGrouper = new SensorGrouper(sensorModel, deviceId, parent);
        return sensorGrouper;
    }

private:
    SensorModel* sensorModel;
};

#endif //QT_SENSORS_WIDGETS_SENSOR_GROUPER_FACTORY_H
