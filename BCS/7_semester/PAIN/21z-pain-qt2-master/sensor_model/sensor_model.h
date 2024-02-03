#ifndef SENSOR_MODEL_H
#define SENSOR_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QDebug>
#include <QRegExp>
#include <QList>

#include "sensor_probe.h"
#include "device.h"
#include "sensor.h"


namespace Units {
    const QString Kbs = "kb/s";
    const QString Mhz = "Mhz";
    const QString Rpm = "rpm";
    const QString Celsius = "\u2103";
    const QString GB = "GB";
}


class SensorModel : public QAbstractItemModel {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> devices READ getDevices CONSTANT)
public:
    enum Roles {
        SensorValueRole = Qt::UserRole,
        SensorNameRole,
        MeterSensorListRole,
        GraphSensorListRole,
        SensorListRole,
        DeviceRole
    };

    explicit SensorModel(QObject *parent = nullptr);

    ~SensorModel() = default;

    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QHash<int, QByteArray> roleNames() const override;

    QList<QObject*> getDevices() const { return devices; }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void updateSensors(const QVariantMap& newDeviceData);

private:
    SensorProbe *sensorProbe;

    QList<QObject*> devices{
            new Device("Cpu",{
                new Sensor("Core0", 0, Units::Mhz, Sensor::Display::List | Sensor::Display::Graph, 4000, 800),
                new Sensor("Temp0", 0, Units::Celsius, Sensor::Display::List | Sensor::Display::Meter, 110, 20),
                new Sensor("Core1", 0, Units::Mhz    ),
                new Sensor("Temp1", 0, Units::Celsius  ),
                new Sensor("Core2", 0, Units::Mhz    ),
                new Sensor("Temp2", 0, Units::Celsius  ),
                new Sensor("Core3", 0, Units::Mhz    ),
                new Sensor("Temp3", 0, Units::Celsius  ),
                new Sensor("Core4", 0, Units::Mhz    ),
                new Sensor("Temp4", 0, Units::Celsius  ),
                new Sensor("Core5", 0, Units::Mhz    ),
                new Sensor("Temp5", 0, Units::Celsius  ),
                new Sensor("Core6", 0, Units::Mhz    ),
                new Sensor("Temp6", 0, Units::Celsius  ),
                new Sensor("Core7", 0, Units::Mhz    ),
                new Sensor("Temp7", 0, Units::Celsius  ),
                new Sensor("Fan", 800, Units::Rpm       )
            }),
            new Device("Battery", {
                    new Sensor("Temp", 40, Units::Celsius, Sensor::Display::List | Sensor::Display::Meter, 100, 0),
                    new Sensor("Health", 99, "%"    ),
                    new Sensor("Voltage", 0, "V"   )
            }),
            new Device("Memory", {
                    new Sensor("Clock", 3200, Units::Mhz, Sensor::Display::List | Sensor::Display::Meter, 4000, 400),
                    new Sensor("Size", 32, Units::GB        )
            }),
            new Device("Gpu", {
                    new Sensor("Core", 1700, Units::Mhz, Sensor::Display::List | Sensor::Display::Meter, 2000, 300),
                    new Sensor("Temp", 0, Units::Celsius, Sensor::Display::List | Sensor::Display::Graph, 100, 0),
                    new Sensor("Memory", 2000, Units::Mhz   ),
                    new Sensor("Vram", 6, Units::GB         )
            }),
            new Device("Network Card", {
                    new Sensor("Temp", 0, Units::Celsius, Sensor::Display::List | Sensor::Display::Meter, 100, 0),
                    new Sensor("Speed in", 0, Units::Kbs, Sensor::Display::List | Sensor::Display::Graph, 10000, 0 ),
                    new Sensor("Speed out", 0, Units::Kbs, Sensor::Display::List | Sensor::Display::Graph, 10000, 0)
            }),
            new Device("Drive", {
                    new Sensor("Capacity", 512, Units::GB),
                    new Sensor("Temp", 0, Units::Celsius, Sensor::Display::List | Sensor::Display::Meter | Sensor::Display::Graph, 100, 0)
            }),
    };
};

Q_DECLARE_METATYPE(QModelIndex)

#endif