
#include "sensor_model.h"
#include <QDebug>
#include <QVariant>
#include "device.h"

SensorModel::SensorModel(QObject *parent) : QAbstractItemModel(parent) {
    sensorProbe = new SensorProbe(1000);
    QObject::connect(sensorProbe, &SensorProbe::newSensorData, this, &SensorModel::updateSensors);
    sensorProbe->init();
}

QVariant SensorModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    auto deviceNameOrEmptyQVariant = [] (const QModelIndex& index) {
        Device *device = qobject_cast<Device *>((QObject*)index.internalPointer());
        if (device == nullptr) return QVariant();
        return QVariant(device->getName());
    };
    auto sensorNameOrEmptyQVariant = [] (const QModelIndex& index) {
        Sensor* sensor = qobject_cast<Sensor*>((QObject*)index.internalPointer());
        if (sensor == nullptr) return QVariant();
        return QVariant(sensor->getName());
    };

    if (role ==  Roles::DeviceRole) {
        return deviceNameOrEmptyQVariant(index);
    } else if (role == Roles::SensorNameRole) {
        return sensorNameOrEmptyQVariant(index);
    } else if (role == Roles::SensorValueRole) {
        Sensor* sensor = qobject_cast<Sensor*>((QObject*)index.internalPointer());
        if (sensor == nullptr) return QVariant();
        return sensor->getValue().toString() + sensor->getUnit();
    } else if (role == Roles::SensorListRole) {
        Device *device = qobject_cast<Device *>((QObject*)index.internalPointer());
        if (device == nullptr) return QVariant();
        return QVariant::fromValue(device->getListSensors());
    } else if (role == Roles::MeterSensorListRole) {
        Device *device = qobject_cast<Device *>((QObject*)index.internalPointer());
        if (device == nullptr) return QVariant();
        return QVariant::fromValue(device->getMeters());
    } else if (role == Roles::GraphSensorListRole) {
        Device *device = qobject_cast<Device *>((QObject*)index.internalPointer());
        if (device == nullptr) return QVariant();
        return QVariant::fromValue(device->getGraphSensors());
    } else if (role == Qt::DisplayRole) {
        Device *device = qobject_cast<Device *>((QObject*)index.internalPointer());
        if (device != nullptr) return device->getName();
        Sensor* sensor = qobject_cast<Sensor*>((QObject*)index.internalPointer());
        if (sensor == nullptr) return QVariant();
        return sensor->getName() + "\t" + sensor->getValue().toString() + sensor->getUnit();
    }
    return QVariant();
}

QModelIndex SensorModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    if (!parent.isValid()) {
        return createIndex(row, column, devices[row]);
    }
    Device* device = qobject_cast<Device*>((QObject*)parent.internalPointer());
    if (device == nullptr)
        return QModelIndex();
    return createIndex(row, column, device->getSensors()[row]);
}

QModelIndex SensorModel::parent(const QModelIndex &child) const {
    Device *device = qobject_cast<Device *>((QObject*)child.internalPointer());
    if (device != nullptr) return QModelIndex();
    Sensor* childSensor = qobject_cast<Sensor*>((QObject*)child.internalPointer());
    if (childSensor == nullptr) return QModelIndex();
    for (int i = 0; devices.size(); ++i) {
        for (const auto& sensor : qobject_cast<Device*>(devices[i])->getSensors()) {
            if (childSensor == sensor)
                return createIndex(i, 0, devices[i]);
        }
    }
    return QModelIndex();
}

int SensorModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid()) {
        return devices.size();
    }
    Device* device = qobject_cast<Device*>((QObject*)parent.internalPointer());
    if (device == nullptr)
        return 0;
    return device->getSensors().size();
}

int SensorModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

Qt::ItemFlags SensorModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index);
}

QHash<int, QByteArray> SensorModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[MeterSensorListRole] = "meterSensorList";
    roles[GraphSensorListRole] = "graphSensorList";
    roles[SensorListRole] = "sensorList";
    roles[DeviceRole] = "device";
    return roles;
}

void SensorModel::updateSensors(const QVariantMap &newDeviceData) {
    int deviceId = -1;
    for (const auto& deviceQObject : devices) {
        ++deviceId;
        const auto& device = qobject_cast<Device*>(deviceQObject);

        if (!newDeviceData.contains(device->getName())) continue;

        const auto& newSensorData = newDeviceData[device->getName()].toMap();
        int sensorId = -1;
        for (const auto& sensorQObject : device->getSensors()) {
            ++sensorId;
            const auto& sensor = qobject_cast<Sensor*>(sensorQObject);
            if (!newSensorData.contains(sensor->getName())) continue;
            const auto& newSensorValue = newSensorData[sensor->getName()];
            qobject_cast<Device*>(devices[deviceId])->getSensor(sensorId)->addSample(newSensorValue);
        }
    }
    emit dataChanged(QModelIndex(), QModelIndex(), {});
}

QVariant SensorModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        return "Sensors";
    }
    return QVariant();
}
