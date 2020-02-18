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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //FILE ACTIONS:
    //Import:
    connect(ui->action_loadInternal, SIGNAL(triggered(bool)), this, SLOT(actionLoadBoolCloud()));
    //Export:
    connect(ui->action_saveMeshObj, SIGNAL(triggered(bool)), this, SLOT(actionSaveTriMesh()));
    connect(ui->action_saveBCloud, SIGNAL(triggered(bool)), this, SLOT(actionSaveBoolCloud()));
    //Exit:
    connect(ui->actionExit, SIGNAL(triggered(bool)),this, SLOT(close()));

    //OBJECT ACTIONS:
    connect(ui->actionFilter_Hull, SIGNAL(triggered(bool)), this, SLOT(calcHull()));
    connect(ui->actionGenerate_Mesh, SIGNAL(triggered(bool)), this, SLOT(generateMesh()));

    //HELP ACTIONS:
    connect(ui->actionInfo, SIGNAL(triggered(bool)),this, SLOT(actionInfo()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)),this, SLOT(actionAbout()));

    //---RESET BUTTONS---
    connect(ui->pushButtonReset_res, &QPushButton::clicked, this, [this](){ui->spinBox_res->setValue(50);});
    connect(ui->pushButtonReset_iter, &QPushButton::clicked, this, [this](){ui->spinBox_iter->setValue(10);});
    connect(ui->pushButtonReset_min, &QPushButton::clicked, this, [this](){ui->doubleSpinBox_min->setValue(-1.75);});
    connect(ui->pushButtonReset_max, &QPushButton::clicked, this, [this](){ui->doubleSpinBox_max->setValue(1.75);});
    connect(ui->pushButtonReset_power, &QPushButton::clicked, this, [this](){ui->doubleSpinBox_power->setValue(8.0);});
    connect(ui->pushButtonReset_maxLength, &QPushButton::clicked, this, [this](){ui->doubleSpinBox_maxLength->setValue(2.0);});

    //---INPUT CHANGE UNLOCKS GENERATE-BUTTON---
    connect(ui->spinBox_res, SIGNAL(valueChanged(int)), this, SLOT(updateOutput()));
    connect(ui->spinBox_iter, SIGNAL(valueChanged(int)), this, SLOT(updateOutput()));
    connect(ui->doubleSpinBox_min, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->doubleSpinBox_max, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->doubleSpinBox_power, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->doubleSpinBox_maxLength, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));

    //---GENERATE/HULL BUTTONS---
    connect(ui->pushButton_generate, SIGNAL(clicked(bool)), this, SLOT(calcMBulb()));

    //---SCATTER GRAPH---
    //Cloud to graph:
    connect(ui->pushButton_viewObject, SIGNAL(clicked(bool)), this, SLOT(boolCloudToGraph()));
    //Checkbox:
    connect(ui->checkBox_showGraph, SIGNAL(stateChanged(int)),this, SLOT(toggleScatterGraph()));
    //Graph:
    QWidget *container = QWidget::createWindowContainer(&scatterGraph,this);
    ui->horizontalLayout->addWidget(container);
    scatterGraph.setAspectRatio(1.0);
    scatterGraph.setHorizontalAspectRatio(1.0);

    //Marching Cubes:
    connect(ui->actionMesh_obj, SIGNAL(triggered(bool)), this, SLOT(generateMesh()));

    //Object management:
    connect(ui->pushButton_remObject, SIGNAL(clicked(bool)), this, SLOT(deleteItem()));

}
