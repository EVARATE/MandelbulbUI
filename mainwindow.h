/*
Copyright 2019, 2020 Sebastian Motzet

This file is part of MandelbulbUI.

MandelbulbUI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MandelbulbUI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MandelbulbUI.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <Q3DScatter>
#include "boolcloud.h"
#include "utilityFunctions.cpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //boolCloud:
    boolCloud mBulb;
    boolCloud hull;
    //Scatter graph:
    QtDataVisualization::QScatter3DSeries scatterSeries;
    QtDataVisualization::Q3DScatter scatterGraph;

    //Input data:
    struct {
        int res = 50;
        int iter = 10;
        double min = -1.75;
        double max = 1.75;
        double power = 8.0;
        double maxLength = 2.0;
    } inputValues;

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

    //Output:
    void updateOutput();
    //Scatter graph:
    void toggleScatterGraph();
    void MBulbToGraph();
    void HullToGraph();

    //Main functions:
    void calcMBulbUI();
    void calcHullUI();

    //Marching Cubes:
    void generateMesh();

    //Delete cache:
    void delMBulbCache();
    void delHullCache();

};
#endif // MAINWINDOW_H
