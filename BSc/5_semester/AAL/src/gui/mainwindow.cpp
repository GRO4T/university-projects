#include "inc/gui/mainwindow.h"
#include "inc/gui/container.h"
#include "inc/gui/inputstringdialog.h"
#include "inc/gui/robot.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTableWidgetItem>
#include <cmath>
#include <memory>
#include <qnamespace.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    robot = std::make_unique<CMYKRobot>();
    
    const auto scene = new QGraphicsScene();
    scene->addItem(robot.get());
    ui->canvas->setScene(scene);

    ui->canvas->setRenderHint(QPainter::RenderHint::Antialiasing);
    robot->setMargin(QPointF(10, 10));
    robot->setContainerSize(QPointF(50, 50));
    ui->resume_button->hide();

    QObject::connect(&backend, &AALBackend::computationFinished, 
            this, &MainWindow::computation_finished);
    QObject::connect(&backend, &AALBackend::updateStatusBar,
            this, &MainWindow::backend_updateStatusBar);
    QObject::connect(robot.get(), &CMYKRobot::animationStateChanged,
                     this, &MainWindow::animation_state_changed);
    QObject::connect(robot.get(), &CMYKRobot::animationReady,
                     this, &MainWindow::animation_ready);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_run_algorithm_button_clicked() {
    const auto params = AlgorithmParams {
        robot->get_initial_containers(),
        ui->timeout_timer->value(),
        ui->timeout_settings->isChecked()
    };

    AdditionalBFSParams::ReturnOn ret_on;
    switch (ui->algorithm_combo_box->currentIndex()) {
        case 0:
            switch (ui->return_on_choice->currentIndex()) {
                case 0: ret_on = AdditionalBFSParams::Sorted; break;
                case 1: ret_on = AdditionalBFSParams::CMYKFound; break;
            }
            backend.computeBFS(params, AdditionalBFSParams {
                ret_on,
                ui->is_front->isChecked(),
                static_cast<std::size_t>(ui->seq_len->value())
            });
            break;

        case 1:
            backend.computeNaive(params, AdditionalNaiveParams {
                static_cast<std::size_t>(ui->opti_level->currentIndex())      
            });
            break;

        case 2:
            backend.computeAstar(params, AdditionalAstarParams {
                static_cast<std::size_t>(ui->heuristic_function_choice->currentIndex())
            });
            break;
    }
}


void MainWindow::on_action_Set_Input_triggered() {
    InputStringDialog dialog(this);
    dialog.exec();
    if (dialog.result() == QDialog::DialogCode::Accepted) {
        robot->setup_containers(dialog.input().toStdString());        
    }
}
    
void MainWindow::computation_finished(AlgorithmResult result) {
    current_moves = result.moves;
    robot->animate_moves(current_moves, ui->step_duration->value() * 1000.0f);
    ui->algorithm_result_table->setRowCount(result.results.size()); 

    auto it = result.results.begin();
    for (std::size_t row=0; row < result.results.size(); ++row) {
        const auto & [ name_str, value_str ] = *it;

        const auto name = new QTableWidgetItem();
        const auto value = new QTableWidgetItem();
        name->setText(QString(name_str.c_str()));
        value->setText(QString(value_str.c_str()));
        ui->algorithm_result_table->setItem(row, 0, name);
        ui->algorithm_result_table->setItem(row, 1, value);
        
        std::advance(it, 1);
    }
}
    
void MainWindow::backend_updateStatusBar(std::string str) {
    //TODO implement
}

void MainWindow::on_reset_button_clicked() {
    robot->reset();
}

void MainWindow::on_play_button_clicked() {
    robot->play();
}

void MainWindow::on_step_duration_valueChanged(double arg1) {
    robot->update_animation_duration(arg1 * 1000.0f);
}

constexpr qreal SCALE_FACTOR = 0.05f;

void MainWindow::on_action_Exit_triggered() {
    close();
}

void MainWindow::on_scale_slider_valueChanged(int value) {
    const qreal scale = std::exp(value / 500.0f);
    robot->setScale(scale);
}

void MainWindow::on_pause_button_clicked() {
    robot->pause();
}

void MainWindow::on_resume_button_clicked() {
    robot->resume();
}
    
void MainWindow::animation_state_changed(QSequentialAnimationGroup::State state) {
    if (state == QSequentialAnimationGroup::State::Running) {
        ui->pause_button->setEnabled(true);
        ui->pause_button->show();
        ui->resume_button->hide();
        ui->play_button->setEnabled(false);
    } else if (state == QSequentialAnimationGroup::State::Paused) {
        ui->resume_button->setEnabled(true);
        ui->resume_button->show();
        ui->pause_button->hide();
    } else {
        ui->pause_button->setEnabled(false);
        ui->resume_button->setEnabled(false);
        ui->pause_button->show();
        ui->resume_button->hide();
        ui->play_button->setEnabled(true);
    }
}

void MainWindow::animation_ready() {
    ui->reset_button->setEnabled(true);
    ui->play_button->setEnabled(true);
    ui->resume_button->setEnabled(false);
    ui->pause_button->setEnabled(false);
}
