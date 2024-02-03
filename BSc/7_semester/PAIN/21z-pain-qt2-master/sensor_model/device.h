//
// Created by damiankolaska on 11/26/21.
//

#ifndef QT_SENSORS_WIDGETS_DEVICE_H
#define QT_SENSORS_WIDGETS_DEVICE_H

#include <QObject>
#include <QDebug>
#include "sensor.h"

class Device: public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> graphSensors READ getGraphSensors CONSTANT)
    Q_PROPERTY(QList<QObject*> listSensors READ getListSensors CONSTANT)
public:
    Device() = default;
    ~Device() = default;
    Device(const Device&) = default;
    Device(Device&&) = default;
    Device& operator=(const Device&) = default;
    Device(const QString& name, const QList<QObject*>& sensors): name(name), sensors(sensors) {
        for (QObject *obj : sensors) {
            Sensor* sensor = qobject_cast<Sensor*>(obj);
            if (sensor == nullptr) {
                qWarning() << "found nullptr when trying to cast to Sensor";
            }
            if (sensor->getDisplayMode() & Sensor::Display::List) {
                listSensors.append(sensor);
            }
            if (sensor->getDisplayMode() & Sensor::Display::Meter) {
                meterSensors.append(sensor);
            }
            if (sensor->getDisplayMode() & Sensor::Display::Graph) {
                graphSensors.append(sensor);
            }
        }
    }

    const QString& getName() const { return name; }
    void setName(const QString& name) { this->name = name; }

    const QList<QObject*>& getSensors() const { return sensors; }
    const QList<QObject*>& getListSensors() const { return listSensors; }
    const QList<QObject*> getMeters() const { return meterSensors; }
    const QList<QObject*> getGraphSensors() const { return graphSensors; }

    Sensor* getSensor(int index) {
        return (Sensor *) sensors.at(index);
    }

private:
    QString name;
    QList<QObject*> sensors;
    QList<QObject*> listSensors;
    QList<QObject*> meterSensors;
    QList<QObject*> graphSensors;
};

#endif //QT_SENSORS_WIDGETS_DEVICE_H