//
// Created by damiankolaska on 11/19/21.
//

#ifndef QT_SENSORS_WIDGETS_SENSOR_TEXT_VALUE_WIDGET_H
#define QT_SENSORS_WIDGETS_SENSOR_TEXT_VALUE_WIDGET_H


#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

class SensorTextValueWidget : public QWidget {
Q_OBJECT
public:
    SensorTextValueWidget(QModelIndex device,
                          int sensorId,
                          SensorModel *sensorModel,
                          QWidget *parent = nullptr)
            : QWidget(parent),
              sensorModel(sensorModel) {
        this->label = new QLabel(this);
        this->label->setText("undefined");
        value = new QLabel(this);
        value->setText("undefined");
        sensor = sensorModel->index(sensorId, 0, device);
        QObject::connect(sensorModel, &SensorModel::dataChanged, this, &SensorTextValueWidget::updateValue);
    }

    virtual ~SensorTextValueWidget() {};

    QLabel *getLabel() const { return label; }

    QLabel *getValue() const { return value; }

public slots:

    void updateValue(const QModelIndex &topLeft,
                     const QModelIndex &bottomRight,
                     const QVector<int> &roles = QVector<int>()) {
        const auto &sensorName = sensorModel->data(sensor, SensorModel::Roles::SensorNameRole);
        const auto &sensorValue = sensorModel->data(sensor, SensorModel::Roles::SensorValueRole);
        label->setText(sensorName.toString());
        value->setText(sensorValue.toString());
    }

private:
    QLabel *label;
    QLabel *value;

    QModelIndex sensor;
    QModelIndex device;
    SensorModel *sensorModel;
};


#endif //QT_SENSORS_WIDGETS_SENSOR_TEXT_VALUE_WIDGET_H
