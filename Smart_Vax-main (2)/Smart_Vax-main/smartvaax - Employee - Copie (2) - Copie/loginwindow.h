#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "connection.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QPoint>
#include <QProcess>


namespace Ui {
class loginWindow;
}

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = nullptr);
    ~loginWindow();

private slots:
    void on_pushButton_conn_clicked();
    void on_pushButton_close_clicked();

    void on_pushButton_Face_clicked();

private:
    Ui::loginWindow *ui;
    MainWindow *mainWindow;
    QPoint m_dragStartPosition;
    Connection conn;
    QProcess* faceProcess;



protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // LOGINWINDOW_H
