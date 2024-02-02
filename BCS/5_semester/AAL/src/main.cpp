#include "inc/gui/mainwindow.h"

#include "inc/algorithm/alg_params.hpp"
#include <QApplication>

Q_DECLARE_METATYPE(AlgorithmResult)
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<AlgorithmResult>();
    MainWindow w;
    w.show();
    return a.exec();
}
