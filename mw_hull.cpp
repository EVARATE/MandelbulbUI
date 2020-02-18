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

//Saving/Loading:
void MainWindow::actionSaveHull(){
    QFileDialog saveDialog;
    saveDialog.setDefaultSuffix("txt");
    QString fileName = saveDialog.getSaveFileName();
    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath, "txt");
    if(extension == "txt"){
        hull.saveCoords(filePath);
    }
    else if(extension == "bin"){
        hull.saveInternal(filePath);
    }
    ui->label_infoText->setText(QString::fromStdString("Saved hull to " + filePath ));
}
void MainWindow::actionLoadHull(){
    QFileDialog loadDialog;
    loadDialog.setDefaultSuffix("bin");
    QString fileName = loadDialog.getOpenFileName();
    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath);
    if(extension == "bin"){
        hull.loadInternal(filePath);
        ui->pushButton_filterHull->setEnabled(false);
        HullToGraph();
        ui->label_infoText->setText(QString::fromStdString("Loaded " + filePath ));
    }
    else{
        ui->label_infoText->setText(QString::fromStdString("ERROR: Invalid file extension: " + extension));
    }
}
//Show in scatter graph:
void MainWindow::HullToGraph(){
    scatterSeries.dataProxy()->removeItems(0,scatterSeries.dataProxy()->itemCount());
    QtDataVisualization::QScatterDataArray scatterData;
    for(int i = 0; i < mBulb.xsize; ++i){
    for(int j = 0; j < mBulb.ysize; ++j){
    for(int k = 0; k < mBulb.zsize; ++k){
        ivec index = {i,j,k};
        if(hull.getState(index)){
            dvec coords(3);
            mBulb.convIndexToCoord(index,coords);
            scatterData << QVector3D(coords[0], coords[1], coords[2]);
        }
    }
    }
    }
    scatterSeries.dataProxy()->addItems(scatterData);
    scatterGraph.addSeries(&scatterSeries);
}
//Delete cache:
void MainWindow::delHullCache(){
    hull.remove();
    ui->actionSave_Hull->setEnabled(false);
    ui->actionMesh_obj->setEnabled(false);
    ui->label_infoText->setText("Deleted hull cache.");
}
//Algorithm:
void MainWindow::calcHullUI(){
    //Get input values:
    int res = inputValues.res;
    double min = inputValues.min;
    double max = inputValues.min;
    double distance = std::abs(max - min) / res;
    dvec vDist(3,distance);
    dvec vMin(3,min);
    ivec vSize(3,res);
    //Reset Hull cloud:
    hull.remove();
    hull.init(vDist, vMin, vSize);
    //Reset UI:
    ui->label_hullPoints->setText("0");
    ui->label_infoText->setText("Generating Hull...");
    //Initialize Scatter series:
    scatterSeries.dataProxy()->removeItems(0,scatterSeries.dataProxy()->itemCount());
    QtDataVisualization::QScatterDataArray scatterData;
    //Progress:
        double progress = 0.0;
        double progdiv = 100.0 / pow(double(mBulb.xsize - 3), 3.0);
        int pC = 0;

        //Check all points:
        for(int i = 0; i < mBulb.xsize; ++i){
        for(int j = 0; j < mBulb.ysize; ++j){
        for(int k = 0; k < mBulb.zsize; ++k){
            ivec active = {i,j,k};
            dvec coord(3);
            mBulb.convIndexToCoord(active,coord);
            if(mBulb.getState(active)){
                     if(!mBulb.getState(i-1,j-1,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i-1,j-1,k  )){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i-1,j-1,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i-1,j  ,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i-1,j  ,k  )){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i-1,j  ,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i-1,j+1,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i-1,j+1,k  )){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i-1,j+1,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}

                else if(!mBulb.getState(i  ,j-1,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i  ,j-1,k  )){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i  ,j-1,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i  ,j  ,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}

                else if(!mBulb.getState(i  ,j  ,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i  ,j+1,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i  ,j+1,k  )){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i  ,j+1,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}

                else if(!mBulb.getState(i+1,j-1,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i+1,j-1,k  )){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i+1,j-1,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i+1,j  ,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i+1,j  ,k  )){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i+1,j  ,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i+1,j+1,k-1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i+1,j+1,k  )){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
                else if(!mBulb.getState(i+1,j+1,k+1)){hull.setState(active,true);pC++;scatterData << QVector3D(coord[0],coord[1],coord[2]);}
            }


            //Display progress:
            progress += progdiv;
            ui->progressBar->setValue(progress);
            ui->label_hullPoints->setText(QString::number(pC));

        }
        }
        }
        scatterSeries.dataProxy()->addItems(scatterData);
        scatterGraph.addSeries(&scatterSeries);
        ui->pushButton_filterHull->setEnabled(false);
        ui->checkBox_autoHull->setEnabled(true);
        ui->actionSave_Hull->setEnabled(true);
        ui->label_infoText->setText("Generated Hull");
        ui->actionMesh_obj->setEnabled(true);
}
