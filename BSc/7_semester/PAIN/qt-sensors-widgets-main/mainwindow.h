#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

#include "process_list_model/process_list_delegate.h"
#include "sensor_model/sensor_model.h"
#include "process_list_model/process_list_model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ProcessListDelegate processListDelegate;

    QWidget* setUpSensorGridTab(SensorModel* sensorModel);
    QWidget* setUpProcessListTab(ProcessListModel* processListModel);
    QWidget* setUpSensorTreeTab(SensorModel* sensorModel);
};

#endif // MAINWINDOW_H
