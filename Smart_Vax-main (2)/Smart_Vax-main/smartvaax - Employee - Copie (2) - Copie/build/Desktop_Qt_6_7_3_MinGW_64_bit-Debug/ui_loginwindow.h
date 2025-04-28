/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginWindow
{
public:
    QWidget *centralwidget;
    QLabel *label_6;
    QPushButton *pushButton_close;
    QLabel *sidentifier;
    QWidget *widget;
    QLabel *label;
    QLabel *label_4;
    QLabel *mdp;
    QLabel *id;
    QLineEdit *lineEdit_id;
    QLineEdit *lineEdit_mdp;
    QPushButton *pushButton_Face;
    QLineEdit *lineEdit_35;
    QPushButton *pushButton_conn;
    QLabel *label_7;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *loginWindow)
    {
        if (loginWindow->objectName().isEmpty())
            loginWindow->setObjectName("loginWindow");
        loginWindow->resize(770, 489);
        loginWindow->setStyleSheet(QString::fromUtf8("background-color:rgb(63, 123, 134);\n"
"\n"
"border-radius: 15px;"));
        centralwidget = new QWidget(loginWindow);
        centralwidget->setObjectName("centralwidget");
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(690, 10, 51, 61));
        label_6->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 0, 0);\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        pushButton_close = new QPushButton(centralwidget);
        pushButton_close->setObjectName("pushButton_close");
        pushButton_close->setGeometry(QRect(690, 10, 51, 51));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        pushButton_close->setFont(font);
        pushButton_close->setStyleSheet(QString::fromUtf8("background-color:rgb(63, 123, 134);\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit));
        pushButton_close->setIcon(icon);
        pushButton_close->setIconSize(QSize(32, 26));
        sidentifier = new QLabel(centralwidget);
        sidentifier->setObjectName("sidentifier");
        sidentifier->setGeometry(QRect(230, 10, 311, 61));
        sidentifier->setStyleSheet(QString::fromUtf8("font-size: 50px;\n"
"    font-weight: bold;\n"
"    color: black;\n"
""));
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(30, 80, 721, 331));
        widget->setStyleSheet(QString::fromUtf8("background-color: #6D9EA3;\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;"));
        label = new QLabel(widget);
        label->setObjectName("label");
        label->setGeometry(QRect(250, 90, 331, 61));
        label->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 0, 0);\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        label_4 = new QLabel(widget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(250, 200, 421, 61));
        label_4->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 0, 0);\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        mdp = new QLabel(widget);
        mdp->setObjectName("mdp");
        mdp->setGeometry(QRect(20, 200, 201, 51));
        mdp->setStyleSheet(QString::fromUtf8(" font-size: 24px;\n"
" font-weight: bold;\n"
" color: black;\n"
"background-color:rgb(63, 123, 134);"));
        id = new QLabel(widget);
        id->setObjectName("id");
        id->setGeometry(QRect(20, 90, 201, 51));
        id->setStyleSheet(QString::fromUtf8(" font-size: 24px;\n"
" font-weight: bold;\n"
" color: black;\n"
"background-color:rgb(63, 123, 134);"));
        lineEdit_id = new QLineEdit(widget);
        lineEdit_id->setObjectName("lineEdit_id");
        lineEdit_id->setGeometry(QRect(250, 90, 331, 51));
        lineEdit_id->setStyleSheet(QString::fromUtf8("background-color:rgb(63, 123, 134);\n"
"font: 18pt \"Segoe UI\";\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        lineEdit_mdp = new QLineEdit(widget);
        lineEdit_mdp->setObjectName("lineEdit_mdp");
        lineEdit_mdp->setGeometry(QRect(250, 200, 421, 51));
        lineEdit_mdp->setStyleSheet(QString::fromUtf8("background-color:rgb(63, 123, 134);\n"
"font: 18pt \"Segoe UI\";\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        lineEdit_mdp->setEchoMode(QLineEdit::EchoMode::Password);
        pushButton_Face = new QPushButton(widget);
        pushButton_Face->setObjectName("pushButton_Face");
        pushButton_Face->setGeometry(QRect(610, 90, 61, 51));
        pushButton_Face->setFont(font);
        pushButton_Face->setStyleSheet(QString::fromUtf8("background-color:rgb(63, 123, 134);\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::CameraWeb));
        pushButton_Face->setIcon(icon1);
        lineEdit_35 = new QLineEdit(widget);
        lineEdit_35->setObjectName("lineEdit_35");
        lineEdit_35->setGeometry(QRect(610, 100, 61, 51));
        lineEdit_35->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 0, 0);\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        label->raise();
        label_4->raise();
        mdp->raise();
        id->raise();
        lineEdit_id->raise();
        lineEdit_mdp->raise();
        lineEdit_35->raise();
        pushButton_Face->raise();
        pushButton_conn = new QPushButton(centralwidget);
        pushButton_conn->setObjectName("pushButton_conn");
        pushButton_conn->setGeometry(QRect(280, 380, 181, 51));
        pushButton_conn->setStyleSheet(QString::fromUtf8("color: black;\n"
"font-weight: bold;\n"
"background-color:rgb(63, 123, 134);\n"
"    border: 2px solid black;  /* Change border color to blue */\n"
""));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(280, 390, 181, 51));
        label_7->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 0, 0);\n"
"border: 2px solid #000000;\n"
"border-radius: 15px;\n"
""));
        loginWindow->setCentralWidget(centralwidget);
        label_7->raise();
        widget->raise();
        label_6->raise();
        pushButton_close->raise();
        sidentifier->raise();
        pushButton_conn->raise();
        menubar = new QMenuBar(loginWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 770, 25));
        loginWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(loginWindow);
        statusbar->setObjectName("statusbar");
        loginWindow->setStatusBar(statusbar);

        retranslateUi(loginWindow);

        QMetaObject::connectSlotsByName(loginWindow);
    } // setupUi

    void retranslateUi(QMainWindow *loginWindow)
    {
        loginWindow->setWindowTitle(QCoreApplication::translate("loginWindow", "SmartVax ", nullptr));
        label_6->setText(QString());
        pushButton_close->setText(QString());
        sidentifier->setText(QCoreApplication::translate("loginWindow", "S'IDENTIFIER", nullptr));
        label->setText(QString());
        label_4->setText(QString());
        mdp->setText(QCoreApplication::translate("loginWindow", "MOT DE PASSE :", nullptr));
        id->setText(QCoreApplication::translate("loginWindow", "IDENTIFIANT :", nullptr));
        pushButton_Face->setText(QString());
        pushButton_conn->setText(QCoreApplication::translate("loginWindow", "Se Connecter", nullptr));
        label_7->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class loginWindow: public Ui_loginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
