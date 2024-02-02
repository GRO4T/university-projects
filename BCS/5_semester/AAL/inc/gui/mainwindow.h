#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "inc/gui/aalbackend.h"
#include "robot.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <algorithm>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    std::unique_ptr<CMYKRobot> robot;
    AALBackend backend;
    std::vector<std::size_t> current_moves;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_run_algorithm_button_clicked();
    void on_action_Set_Input_triggered();
    void on_reset_button_clicked();
    void on_play_button_clicked();
    void on_step_duration_valueChanged(double arg1);
    void on_action_Exit_triggered();
    void on_scale_slider_valueChanged(int value);
    void on_pause_button_clicked();
    void on_resume_button_clicked();

    void computation_finished(AlgorithmResult );
    void backend_updateStatusBar(std::string );
    void animation_state_changed(QSequentialAnimationGroup::State state);
    void animation_ready();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
