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

#include <list>
#include <iterator>
#include <algorithm>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <Q3DScatter>
#include "boolcloud.h"
#include "utilityFunctions.cpp"

//Structs:
typedef struct {
   dvec p[3];
} TRIANGLE;

typedef struct {
   dvec p[8];
   double val[8];
} GRIDCELL;
//Items in memory:
typedef struct {
    std::string name = "Abstract Item";
    int type = 0;
    int id;
    //Each item can only contain one of the following:
    boolCloud cloud;//Type: 0
    std::vector<TRIANGLE> triMesh;//Type: 1
    std::vector<dvec> pointSet;//Type 2
} abstrItem;




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

    //Methods:
    int getSelectedID();

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

    //List of all items:
    int nextObjID = 0;
    std::list<abstrItem> allItems;

private slots:
    //Saving/Loading:
    void actionSaveBoolCloud(boolCloud& cloud);
    void actionSaveBoolCloud();
    void actionLoadBoolCloud();
    void actionSaveTriMesh(std::vector<TRIANGLE>& triMesh);
    void actionSaveTriMesh();
    void actionLoadPointSet();
    //Info/About Dialog:
    void actionInfo();
    void actionAbout();

    //Output:
    void updateOutput();
    //Scatter graph:
    void toggleScatterGraph();
    void boolCloudToGraph(boolCloud& cloud);
    void boolCloudToGraph();

    //Main functions:
    void calcMBulb();
    void calcHull(boolCloud& cloud);
    void calcHull();

    //Marching Cubes:
    void generateMesh(boolCloud& cloud);
    void generateMesh();

    //Abstract object management:
    void createAbstrObj(boolCloud& cloud, std::string name);
    void createAbstrObj(std::vector<TRIANGLE>& triMesh, std::string name);
    void createAbstrObj(std::vector<dvec>& pointSet, std::string name);
    void deleteAbstrObj(int id);
    void createItemEntry(std::string name, int type, int id);
    void deleteItem();
    void updateActionAvailability();
    void getObjAtID(int id, abstrItem& item);

};
#endif // MAINWINDOW_H
