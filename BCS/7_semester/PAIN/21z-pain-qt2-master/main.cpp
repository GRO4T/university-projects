#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>

#include "sensor_model/sensor_model.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    SensorModel* sensorModel = new SensorModel();

    QQmlApplicationEngine engine;
    qmlRegisterType<SensorModel>("Sensors", 1, 0, "SensorModel");
    qmlRegisterType<Sensor>("Sensors", 1, 0, "Device");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
