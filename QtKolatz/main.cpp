#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Коллатц Пошук");
    window.resize(600, 400);
    window.show();
    return app.exec();
}
