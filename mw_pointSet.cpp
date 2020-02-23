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


void MainWindow::actionLoadPointSet(){
    //Select file:
    QFileDialog loadDialog;
    loadDialog.setDefaultSuffix("txt");
    QString fileName = loadDialog.getOpenFileName();
    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath);

    //Create and load object:
    std::vector<dvec> pointSet;
    dvec point(3);
    if(extension == "txt"){
        std::ifstream ifile;
        ifile.open(filePath);
        if(!ifile.is_open()){
            ui->label_infoText->setText(QString::fromStdString("Couldn't open file: " + filePath));
            return;
        }
        while(true){
            if(ifile.eof()){
                break;
            }else{
                ifile >> point[0] >> point[1] >> point[2];
                pointSet.push_back(point);
            }
        }
        ifile.close();
        createAbstrObj(pointSet, "Point Set");
        ui->label_infoText->setText(QString::fromStdString("Imported file: " + filePath));
    }else{
        ui->label_infoText->setText(QString::fromStdString("Invalid file extension: " + extension));
        return;
    }
}
