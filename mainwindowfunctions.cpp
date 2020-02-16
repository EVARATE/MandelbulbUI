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
#include "blockymesh.cpp"
#include "utilityFunctions.cpp"

MainWindow::~MainWindow()
{
    delete ui;
}

//---IMPLEMENTATION---
//Saving:
void MainWindow::actionSaveMBulb(){
    QFileDialog saveDialog;
    saveDialog.setDefaultSuffix("txt");
    QString fileName = saveDialog.getSaveFileName();
    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath, "txt");
    if(extension == "txt"){
        mBulb.saveCoords(filePath);
    }
    else if(extension == "bin"){
        mBulb.saveInternal(filePath);
    }
    ui->label_infoText->setText(QString::fromStdString("Saved mandelbulb to " + filePath ));
}
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
//Loading:
void MainWindow::actionLoadMBulb(){
    QFileDialog loadDialog;
    loadDialog.setDefaultSuffix("bin");
    QString fileName = loadDialog.getOpenFileName();
    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath);
    if(extension == "bin"){
        mBulb.loadInternal(filePath);
        MBulbToGraph();
        ui->label_infoText->setText(QString::fromStdString("Loaded " + filePath ));
    }
    else{
        ui->label_infoText->setText(QString::fromStdString("ERROR: Invalid file extension: " + extension));
    }

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
//Info/About Dialog:
void MainWindow::actionInfo(){
    QMessageBox infoBox;
    infoBox.information(this,"Info","Please read the readme.md file");
    infoBox.show();
}
void MainWindow::actionAbout(){
    QMessageBox aboutBox;
    aboutBox.about(this,"Mandelbulb_V2 Pre Alpha",
    "Made with Qt Creator 4.11.0\n\nLicensed under the GNU General Public License v3.0\n\nCopyright 2019, 2020 Sebastian Motzet");
}

//Output:
void MainWindow::updateOutput(){
    ui->pushButton_generate->setEnabled(true);
    int res = ui->spinBox_res->value();
    double min = ui->doubleSpinBox_min->value();
    double max = ui->doubleSpinBox_max->value();
    double distance = std::abs(max - min) / res;
    ui->label_distance->setText(QString::number(distance));
    ui->label_rawPoints->setText(QString::number(std::pow(res,3.0)));

}
//Scatter graph:
void MainWindow::toggleScatterGraph(){
    bool checked = ui->checkBox_showGraph->isChecked();
    if(checked){
        scatterGraph.show();
    }else{
        scatterGraph.hide();
    }
}
void MainWindow::MBulbToGraph(){
    scatterSeries.dataProxy()->removeItems(0,scatterSeries.dataProxy()->itemCount());
    QtDataVisualization::QScatterDataArray scatterData;
    for(int i = 0; i < mBulb.xsize; ++i){
    for(int j = 0; j < mBulb.ysize; ++j){
    for(int k = 0; k < mBulb.zsize; ++k){
        ivec index = {i,j,k};
        if(mBulb.getState(index)){
            dvec coords(3);
            mBulb.convIndexToCoord(index,coords);
            scatterData << QVector3D(coords[0], coords[1], coords[2]);
        }
    }
    }
    }
    scatterSeries.dataProxy()->addItems(scatterData);
    scatterGraph.addSeries(&scatterSeries);
    ui->pushButton_filterHull->setEnabled(true);
}
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

//============================================================================================
//Main functions:
void MainWindow::calcMBulbUI(){
    //See if 'auto' is checked:
    bool autoHull = ui->checkBox_autoHull->isChecked();
    //Adapt UI:
    ui->pushButton_generate->setEnabled(false);
    if(autoHull){
        ui->pushButton_filterHull->setEnabled(false);
        ui->checkBox_autoHull->setEnabled(false);
    }
    //Get input values:
    int res = ui->spinBox_res->value();
    int iter = ui->spinBox_iter->value();
    double min = ui->doubleSpinBox_min->value();
    double max = ui->doubleSpinBox_max->value();
    double power = ui->doubleSpinBox_power->value();
    double maxLength = ui->doubleSpinBox_maxLength->value();
    double distance = std::abs(max - min) / res;
    dvec vDist(3,distance);
    dvec vMin(3,min);
    ivec vSize(3,res);
    //Reset primCloud:
    mBulb.remove();
    mBulb.init(vDist,vMin,vSize);
    ui->label_setPoints->setText("0");
    ui->label_infoText->setText("Generating Mandelbulb...");
    //Initialize Scatter series:
    scatterSeries.dataProxy()->removeItems(0,scatterSeries.dataProxy()->itemCount());
    QtDataVisualization::QScatterDataArray scatterData;

    //Calculation:
          double density = 0;
          double xpos, ypos, zpos;
          double xpoint, ypoint, zpoint;
          double cx, cy, cz;
          double r, phi, theta;

          //Track progress:
          double progress = 0.0;
          double progdiv = 100.0 / pow(double(res - 3), 3.0);
          int pointCount = 0;

          for(xpos = min; xpos <= max; xpos += distance){
            for(ypos = min; ypos <= max; ypos += distance){
            for(zpos = min; zpos <= max; zpos += distance){
                //reset for next point:
                xpoint = xpos;
                ypoint = ypos;
                zpoint = zpos;
                cx = xpos;
                cy = ypos;
                cz = zpos;

                //Sequence loop:
                for(int i = 0; i <= iter; i++)
                {
                    r = sqrt(xpoint*xpoint + ypoint*ypoint + zpoint*zpoint);
                    phi = atan(ypoint/xpoint);
                    theta = acos(zpoint/r);

                    xpoint = pow(r, power) * sin(power * theta) * cos(power * phi) + cx;
                    ypoint = pow(r, power) * sin(power * theta) * sin(power * phi) + cy;
                    zpoint = pow(r, power) * cos(power * theta) + cz;

                    if(r >= maxLength)
                    {
                        density = 0.0;
                        break;
                    }
                    density = 1.0;

                }

                //Display progress:
                progress += progdiv;
                ui->progressBar->setValue(progress);

                if(density == 1.0)
                {
                    //Update Set:
                    pointCount++;
                    ui->label_setPoints->setText(QString::number(pointCount));
                    dvec coord = {xpos,ypos,zpos};
                    mBulb.setState(coord,true);
                    scatterData << QVector3D(xpos,ypos,zpos);
                }

            }//zpos loop end
            }//ypos loop end
            }//xpos loop end
          scatterSeries.dataProxy()->addItems(scatterData);
          scatterGraph.addSeries(&scatterSeries);
          ui->label_infoText->setText("Generated Mandelbulb");
          ui->actionSave_Mandelbulb->setEnabled(true);
          if(autoHull){
              calcHullUI();
          }else{
              ui->pushButton_filterHull->setEnabled(true);
          }
}
void MainWindow::calcHullUI(){
    //Get input values:
    int res = ui->spinBox_res->value();
    double min = ui->doubleSpinBox_min->value();
    double max = ui->doubleSpinBox_max->value();
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
        ui->actionExport_Mesh_obj->setEnabled(true);
        ui->label_infoText->setText("Generated Hull");
}

void MainWindow::saveCubeModel(){
    /*
    QFileDialog saveDialog;
    saveDialog.setDefaultSuffix("obj");
    QString fileName = saveDialog.getSaveFileName();
    ui->label_infoText->setText("Building cubemodel...");
    cubeMat cubeModel(mBulb);
    //Remove overlapping:
    double progress = 0;
    double progdiv = 100.0 / std::pow(cubeModel.cubeSlots.size() - 3, 3.0);
    if(!cubeModel.isInit){return;}
    for(int i = 0; i < cubeModel.cubeSlots.size(); ++i){
    for(int j = 0; j < cubeModel.cubeSlots[i].size(); ++j){
    for(int k = 0; k < cubeModel.cubeSlots[i][j].size(); ++k){
        cube active = cubeModel.cubeSlots[i][j][k];
        if(!cubeModel.isEmpty(active)){
            cubeModel.remOverlapping(cubeModel.cubeSlots[i-1][j][k],active);//Left
            cubeModel.remOverlapping(cubeModel.cubeSlots[i+1][j][k],active);//Right
            cubeModel.remOverlapping(cubeModel.cubeSlots[i][j-1][k],active);//Back
            cubeModel.remOverlapping(cubeModel.cubeSlots[i][j+1][k],active);//Front
            cubeModel.remOverlapping(cubeModel.cubeSlots[i][j][k-1],active);//Down
            cubeModel.remOverlapping(cubeModel.cubeSlots[i][j][k+1],active);//Up
        }
        progress += progdiv;
        ui->progressBar->setValue(progress);
    }
    }
    }

    ui->label_infoText->setText("Saving cubemodel...");
    //cubeModel.saveObj(fileName.toStdString());
    std::ofstream ofile;
    ofile.open(fileName.toStdString());
    if(!ofile.is_open()){return;}
    std::string polyBuffer;
    std::string faceBuffer;
    int vCounter = 0;
    progress = 0;
    ofile << "#DANGER: File generated via MandelbulbUI:\n";
    ofile << "o Mandelbulb\n";
    for(int i = 0; i < cubeModel.cubeSlots.size(); ++i){
    for(int j = 0; j < cubeModel.cubeSlots[i].size(); ++j){
    for(int k = 0; k < cubeModel.cubeSlots[i][j].size(); ++k){
        if(!cubeModel.isEmpty(cubeModel.cubeSlots[i][j][k])){
            ivec index = {i,j,k};
            dvec coord(3);
            cubeModel.bCloud.convIndexToCoord(index,coord);
            cube active = cubeModel.cubeSlots[i][j][k];
            for(int n = 0; n < active.size(); ++n){
                cubeModel.faceToBuffer(active[n], faceBuffer, polyBuffer, vCounter);
            }
        }
        progress += progdiv;
        ui->progressBar->setValue(progress);
    }
    }
    }
    ofile << faceBuffer;
    ofile << polyBuffer;
    ofile.close();
    ui->label_infoText->setText("Saved cubemodel");
    */
}

//Experimental:
void MainWindow::doCubeMarch(){
    /*
    MarchingCube march(mBulb);
    march.cubeMarch();

    //Quick write to .obj:
    std::ofstream ofile;
    ofile.open("/tmp/cubeMarchMBulb.obj");
    if(!ofile.is_open()){return;}
    std::string polyBuffer;
    unsigned int vCounter = 0;
    ofile << "#Experimental cubemarching mesh:\n";
    ofile << "o cubeMarch\n";
    for(int i = 0; i < march.triBuffer.size(); ++i){
        for(int j = 0; j < march.triCountBuffer[i]; ++j){
            ofile << "v " << march.triBuffer[j].p[0][0] << " " << march.triBuffer[j].p[0][1] << " " << march.triBuffer[j].p[0][2] << "\n";
            ofile << "v " << march.triBuffer[j].p[0][0] << " " << march.triBuffer[j].p[1][1] << " " << march.triBuffer[j].p[1][2] << "\n";
            ofile << "v " << march.triBuffer[j].p[0][0] << " " << march.triBuffer[j].p[2][1] << " " << march.triBuffer[j].p[2][2] << "\n";
            vCounter++; polyBuffer.append("f " + std::to_string(vCounter));
            vCounter++; polyBuffer.append(" " + std::to_string(vCounter));
            vCounter++; polyBuffer.append(" " + std::to_string(vCounter) + "\n");
        }
    }
    ofile << "#triangles:\n";
    ofile << polyBuffer;
    ofile.close();
    */
}
