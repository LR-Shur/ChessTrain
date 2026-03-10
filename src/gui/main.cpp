#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // QMessageBox::information(nullptr, "Debug", "Before MainWindow");

    ChessGame chessGame;
    ChessAi chessAi(&chessGame);
    MainWindow w((QWidget *)nullptr, &chessGame, &chessAi);
    w.show();

    // QMessageBox::information(nullptr, "Debug", "After show");

    return app.exec();
}