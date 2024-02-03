#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "widgets/sensor_grouper_factory.h"

#include <QGridLayout>
#include <QLabel>
#include <QItemDelegate>
#include <QTabWidget>
#include <QTableView>
#include <QTreeView>
#include <QHeaderView>


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    SensorModel* sensorModel = new SensorModel(this);
    ProcessListModel* processListModel = new ProcessListModel(this);

    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->addTab(setUpSensorGridTab(sensorModel), "SensorsGrid");
    tabWidget->addTab(setUpProcessListTab(processListModel), "Processes");
    tabWidget->addTab(setUpSensorTreeTab(sensorModel), "SensorsTree");
    setCentralWidget(tabWidget);

    emit sensorModel->dataChanged(QModelIndex(), QModelIndex(), {});
    processListModel->init();
}

MainWindow::~MainWindow() {
    delete ui;
}


QWidget *MainWindow::setUpSensorGridTab(SensorModel *sensorModel) {
    QWidget *sensorGridTab = new QWidget();
    QWidget* firstColumnWidget = new QWidget(sensorGridTab);
    QWidget* secondColumnWidget = new QWidget(sensorGridTab);
    QVBoxLayout* firstColumnLayout = new QVBoxLayout(firstColumnWidget);
    QVBoxLayout* secondColumnLayout = new QVBoxLayout(secondColumnWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(sensorGridTab);
    SensorGrouperFactory sensorGrouperFactory(sensorModel);
    firstColumnLayout->addWidget(sensorGrouperFactory.create(0));
    firstColumnLayout->addWidget(sensorGrouperFactory.create(1));
    firstColumnLayout->addWidget(sensorGrouperFactory.create(2));
    secondColumnLayout->addWidget(sensorGrouperFactory.create(3));
    secondColumnLayout->addWidget(sensorGrouperFactory.create(4));
    secondColumnLayout->addWidget(sensorGrouperFactory.create(5));
    firstColumnWidget->setLayout(firstColumnLayout);
    secondColumnWidget->setLayout(secondColumnLayout);
    mainLayout->addWidget(firstColumnWidget);
    mainLayout->addWidget(secondColumnWidget);
    sensorGridTab->setLayout(mainLayout);
    return sensorGridTab;
}

QWidget *MainWindow::setUpProcessListTab(ProcessListModel *processListModel) {
    QWidget* processListTab = new QWidget(this);
    QVBoxLayout* processViewLayout = new QVBoxLayout(this);

    QLineEdit* searchBar = new QLineEdit(this);

    QTableView* processList = new QTableView();
    processList->setModel(processListModel->getProxy());
    processList->setItemDelegate(&processListDelegate);
    processList->horizontalHeader()->setStretchLastSection(true);
    processList->setColumnWidth(0, 200);
    processList->setColumnWidth(1, 100);
    processList->setColumnWidth(2, 50);
    processList->setColumnWidth(3, 50);
    processList->setColumnWidth(4, 50);
    processList->setColumnWidth(5, 50);
    processList->setColumnWidth(6, 50);

    QObject::connect(searchBar, &QLineEdit::textChanged,
                     processListModel, &ProcessListModel::setSearchExpression);

    processViewLayout->addWidget(searchBar);
    processViewLayout->addWidget(processList);
    processListTab->setLayout(processViewLayout);
    return processListTab;
}

QWidget *MainWindow::setUpSensorTreeTab(SensorModel *sensorModel) {
    QWidget* sensorTreeTab = new QWidget(this);
    QHBoxLayout* hbox = new QHBoxLayout(this);
    QTreeView* sensorTree = new QTreeView();
    sensorTree->setModel(sensorModel);
    hbox->addWidget(sensorTree);
    sensorTreeTab->setLayout(hbox);
    return sensorTreeTab;
}


