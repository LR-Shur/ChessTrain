/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionrestart;
    QAction *actionquit;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QWidget *boardContainer;
    QWidget *rightPanel;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *newGameButton;
    QLabel *label;
    QComboBox *firstMoveCombo;
    QPushButton *saveFirstMoveButton;
    QCheckBox *useAzAiCheckBox;
    QCheckBox *aiVsAiCheckBox;
    QLabel *aiVsAiBlackLabel;
    QComboBox *aiVsAiBlackCombo;
    QPlainTextEdit *infoTextEdit;
    QMenuBar *menubar;
    QMenu *menu_ai;
    QMenu *menu_G;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionrestart = new QAction(MainWindow);
        actionrestart->setObjectName("actionrestart");
        actionquit = new QAction(MainWindow);
        actionquit->setObjectName("actionquit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        boardContainer = new QWidget(centralwidget);
        boardContainer->setObjectName("boardContainer");

        horizontalLayout->addWidget(boardContainer);

        rightPanel = new QWidget(centralwidget);
        rightPanel->setObjectName("rightPanel");
        verticalLayoutWidget = new QWidget(rightPanel);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 10, 160, 281));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        newGameButton = new QPushButton(verticalLayoutWidget);
        newGameButton->setObjectName("newGameButton");

        verticalLayout->addWidget(newGameButton);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        firstMoveCombo = new QComboBox(verticalLayoutWidget);
        firstMoveCombo->addItem(QString());
        firstMoveCombo->addItem(QString());
        firstMoveCombo->addItem(QString());
        firstMoveCombo->setObjectName("firstMoveCombo");

        verticalLayout->addWidget(firstMoveCombo);

        saveFirstMoveButton = new QPushButton(verticalLayoutWidget);
        saveFirstMoveButton->setObjectName("saveFirstMoveButton");

        verticalLayout->addWidget(saveFirstMoveButton);

        useAzAiCheckBox = new QCheckBox(verticalLayoutWidget);
        useAzAiCheckBox->setObjectName("useAzAiCheckBox");
        useAzAiCheckBox->setChecked(false);

        verticalLayout->addWidget(useAzAiCheckBox);

        aiVsAiCheckBox = new QCheckBox(verticalLayoutWidget);
        aiVsAiCheckBox->setObjectName("aiVsAiCheckBox");
        aiVsAiCheckBox->setChecked(false);

        verticalLayout->addWidget(aiVsAiCheckBox);

        aiVsAiBlackLabel = new QLabel(verticalLayoutWidget);
        aiVsAiBlackLabel->setObjectName("aiVsAiBlackLabel");

        verticalLayout->addWidget(aiVsAiBlackLabel);

        aiVsAiBlackCombo = new QComboBox(verticalLayoutWidget);
        aiVsAiBlackCombo->addItem(QString());
        aiVsAiBlackCombo->addItem(QString());
        aiVsAiBlackCombo->addItem(QString());
        aiVsAiBlackCombo->setObjectName("aiVsAiBlackCombo");

        verticalLayout->addWidget(aiVsAiBlackCombo);

        infoTextEdit = new QPlainTextEdit(verticalLayoutWidget);
        infoTextEdit->setObjectName("infoTextEdit");

        verticalLayout->addWidget(infoTextEdit);


        horizontalLayout->addWidget(rightPanel);

        horizontalLayout->setStretch(0, 4);
        horizontalLayout->setStretch(1, 1);

        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 20));
        menu_ai = new QMenu(menubar);
        menu_ai->setObjectName("menu_ai");
        menu_G = new QMenu(menubar);
        menu_G->setObjectName("menu_G");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu_ai->menuAction());
        menubar->addAction(menu_G->menuAction());
        menu_G->addSeparator();
        menu_G->addSeparator();
        menu_G->addSeparator();
        menu_G->addSeparator();
        menu_G->addSeparator();
        menu_G->addAction(actionrestart);
        menu_G->addAction(actionquit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionrestart->setText(QCoreApplication::translate("MainWindow", "restart", nullptr));
        actionquit->setText(QCoreApplication::translate("MainWindow", "quit", nullptr));
        newGameButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\226\260\346\270\270\346\210\217", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\350\260\201\345\205\210\350\265\260", nullptr));
        firstMoveCombo->setItemText(0, QCoreApplication::translate("MainWindow", "\351\273\221\346\243\213\345\205\210\350\265\260", nullptr));
        firstMoveCombo->setItemText(1, QCoreApplication::translate("MainWindow", "\347\231\275\346\243\213\345\205\210\350\265\260", nullptr));
        firstMoveCombo->setItemText(2, QCoreApplication::translate("MainWindow", "\351\232\217\346\234\272", nullptr));

        saveFirstMoveButton->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\350\256\276\347\275\256", nullptr));
        useAzAiCheckBox->setText(QCoreApplication::translate("MainWindow", "\344\275\277\347\224\250 AZai", nullptr));
        aiVsAiCheckBox->setText(QCoreApplication::translate("MainWindow", "\345\217\214\346\226\271 AI \345\257\271\344\270\213", nullptr));
        aiVsAiBlackLabel->setText(QCoreApplication::translate("MainWindow", "AI\345\257\271\344\270\213\351\273\221\346\226\271", nullptr));
        aiVsAiBlackCombo->setItemText(0, QCoreApplication::translate("MainWindow", "\345\216\237\345\247\213AI\346\211\247\351\273\221", nullptr));
        aiVsAiBlackCombo->setItemText(1, QCoreApplication::translate("MainWindow", "AZai\346\211\247\351\273\221", nullptr));
        aiVsAiBlackCombo->setItemText(2, QCoreApplication::translate("MainWindow", "\351\232\217\346\234\272", nullptr));

        menu_ai->setTitle(QCoreApplication::translate("MainWindow", "\345\223\210\345\237\272\345\245\245\346\231\272\350\203\275\344\272\224\345\255\220\346\243\213\345\257\271\346\210\230ai", nullptr));
        menu_G->setTitle(QCoreApplication::translate("MainWindow", "\346\270\270\346\210\217(&G)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
