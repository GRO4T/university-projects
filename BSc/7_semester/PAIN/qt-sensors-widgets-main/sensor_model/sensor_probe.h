//
// Created by damiankolaska on 11/20/21.
//

#ifndef QT_SENSORS_WIDGETS_SENSOR_PROBE_H
#define QT_SENSORS_WIDGETS_SENSOR_PROBE_H

#include <QProcess>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QRandomGenerator>

#include "command_executor.h"

class SensorProbe: public QObject {
   Q_OBJECT
public:
    SensorProbe(float probingDelay) {
       QTimer* timer = new QTimer(this);
       process = new QProcess(this);
       QObject::connect(timer, &QTimer::timeout, this, &SensorProbe::runSensorsCommand);
       QObject::connect(process, &QProcess::readyReadStandardOutput, this, &SensorProbe::readSensorsOutput);
       timer->start(probingDelay);
    }

    void init() {
        const auto& initialData = jsonAsByteArrayToToQVariantMap(CommandExecutor::execute(sensorsCmd));
        emit newSensorData(processData(initialData));
    }

signals:
    void newSensorData(const QVariantMap& newSensorData);

private:
    QProcess* process;
    int coreCount = 8;
    QString sensorsCmd = "sensors -j";

    void runSensorsCommand() {
        process->start(sensorsCmd);
    }

    void readSensorsOutput() {
        const auto& rawQVariantData = jsonAsByteArrayToToQVariantMap(process->readAllStandardOutput());
        emit newSensorData(processData(rawQVariantData));
    }

    QVariantMap processData(const QVariantMap &rawData) {
        QVariantMap processedData;
        for (auto rawDeviceIt = rawData.constBegin(); rawDeviceIt != rawData.constEnd(); ++rawDeviceIt) {
            if (rawDeviceIt.key().contains("k10temp")) {
                QVariantMap deviceData;
                QVariant coreClock = QString::number(CommandExecutor::execute("lscpu | grep 'CPU MHz' | awk '{print $3}'").toFloat(), 'f', 2);
                for (int i = 0; i < coreCount; ++i) {
                    deviceData["Temp" + QString::number(i)] = rawDeviceIt.value().toMap()["Tctl"].toMap()["temp1_input"];
                    deviceData["Core" + QString::number(i)] = coreClock;
                }
                processedData["Cpu"] = deviceData;
            }
            else if (rawDeviceIt.key().contains("gpu")) {
                QVariantMap deviceData;
                deviceData["Temp"] =  rawDeviceIt.value().toMap()["edge"].toMap()["temp1_input"];
                processedData["Gpu"] = deviceData;
            }
            else if (rawDeviceIt.key().contains("nvme")) {
                QVariantMap deviceData;
                deviceData["Temp"] =  rawDeviceIt.value().toMap()["Composite"].toMap()["temp1_input"];
                processedData["Drive"] = deviceData;
            }
            else if (rawDeviceIt.key().contains("wifi")) {
                QVariantMap deviceData;
                deviceData["Temp"] =  rawDeviceIt.value().toMap()["temp1"].toMap()["temp1_input"];
                deviceData["Speed in"] = QRandomGenerator::global()->generate() % 8000;
                deviceData["Speed out"] = QRandomGenerator::global()->generate() % 1000;
                processedData["Network Card"] = deviceData;
            } else if (rawDeviceIt.key().contains("BAT")) {
                QVariantMap deviceData;
                deviceData["Voltage"] =  rawDeviceIt.value().toMap()["in0"].toMap()["in0_input"];
                processedData["Battery"] = deviceData;
            }
        }
        return processedData;
    }

    QVariantMap jsonAsByteArrayToToQVariantMap(QByteArray rawJson) {
        QJsonParseError parseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(rawJson, &parseError);
        if (jsonDocument.isNull()) {
            qDebug() << "ParseError: " << parseError.errorString();
        }
        return jsonDocument.object().toVariantMap();
    }
};

#endif //QT_SENSORS_WIDGETS_SENSOR_PROBE_H
