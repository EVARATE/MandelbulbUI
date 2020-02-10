#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "pointcloudbool.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //pointCloudBool objects:
    pointCloudBool mBulb;

private:
    Ui::MainWindow *ui;
private slots:
    //Saving:
    void actionSaveMBulb();
    void actionSaveHull();
    //Loading:
    void actionLoadMBulb();
    void actionLoadHull();
    //Info/About Dialog:
    void actionInfo();
    void actionAbout();

    //small signals:
    void updateOutput();

    //Output:
    void calcMBulbUI();
    void calcHullUI();
};
#endif // MAINWINDOW_H
