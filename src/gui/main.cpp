#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // QMessageBox::information(nullptr, "Debug", "Before MainWindow");

    MainWindow w;
    w.show();

    // QMessageBox::information(nullptr, "Debug", "After show");

    return app.exec();
}