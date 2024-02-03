//
// Created by damiankolaska on 11/26/21.
//

#ifndef QT_SENSORS_WIDGETS_SENSOR_H
#define QT_SENSORS_WIDGETS_SENSOR_H

#include <QObject>
#include <QVariant>
#include <QQueue>
#include <QDebug>

class Sensor : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QVariant value READ getValue NOTIFY valueChanged)
    Q_PROPERTY(QString unit READ getUnit CONSTANT)
    Q_PROPERTY(float minValue READ getMinValue CONSTANT)
    Q_PROPERTY(float maxValue READ getMaxValue CONSTANT)
    Q_PROPERTY(float stepSize READ getStepSize CONSTANT)
    Q_PROPERTY(QList<QVariant> history READ getHistory NOTIFY historyChanged)
signals:
    void valueChanged(const QVariant& newValue);
    void historyChanged(const QList<QVariant>& newHistory);
public:
    enum Display {
        List = 0x01,
        Meter = 0x02,
        Graph = 0x04
    };

    Sensor() {}
    Sensor(const QString &name, const QVariant &value, const QString &unit = QString(), int displayMode = Display::List, const QVariant &maxValue = QVariant(),
           const QVariant &minValue = QVariant(), QObject* parent = nullptr) : displayMode(displayMode), name(name), value(value),
                                                            maxValue(maxValue), minValue(minValue), unit(unit), QObject(parent) {
        addSample(value);
    }

    const QString &getName() const { return name; }

    const QVariant &getValue() const { return value; }

    const QString &getUnit() const { return unit; }

    int getDisplayMode() const {
        return displayMode;
    }

    float getMaxValue() const {
        return maxValue.toFloat();
    }

    float getMinValue() const {
        return minValue.toFloat();
    }

    float getStepSize() const {
        if (getMaxValue() < 200)
            return 10;
        if (getMaxValue() < 2000)
            return 100;
        return 200;
    }

    QList<QVariant> getHistory() const {
        return history;
    }

    void addSample(const QVariant& value) {
        history.enqueue(value);
        if (history.size() > HISTORY_SIZE)
            history.dequeue();
        this->value = value;
        emit historyChanged(history);
        emit valueChanged(this->value);
    }

private:
    const static int HISTORY_SIZE = 60;

    int displayMode;
    QString name;
    QVariant value;
    QVariant maxValue;
    QVariant minValue;
    QString unit;
    QQueue<QVariant> history;
};

#endif //QT_SENSORS_WIDGETS_SENSOR_H
