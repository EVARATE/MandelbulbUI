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

//Deconstructor:
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actionSaveBoolCloud(boolCloud& cloud){
    //Select file:
    QFileDialog saveDialog;
    saveDialog.setDefaultSuffix("txt");
    QString fileName = saveDialog.getSaveFileName();
    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath, "txt");

    //Write data:
    if(extension == "txt"){
        cloud.saveCoords(filePath);
    }
    else if(extension == "bin"){
        cloud.saveInternal(filePath);
    }
    ui->label_infoText->setText(QString::fromStdString("Saved cloud to " + filePath ));
}
void MainWindow::actionLoadBoolCloud(){
    //Select file:
    QFileDialog loadDialog;
    loadDialog.setDefaultSuffix("bin");
    QString fileName = loadDialog.getOpenFileName();
    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath);

    //Create and load object:
    boolCloud cloud;
    if(extension == "bin"){
        cloud.loadInternal(filePath);
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
//Scatter graph:
void MainWindow::toggleScatterGraph(){
    bool checked = ui->checkBox_showGraph->isChecked();
    if(checked){
        scatterGraph.show();
    }else{
        scatterGraph.hide();
    }
}
//boolCloud to graph:
void MainWindow::boolCloudToGraph(boolCloud& cloud){
    scatterSeries.dataProxy()->removeItems(0,scatterSeries.dataProxy()->itemCount());
    QtDataVisualization::QScatterDataArray scatterData;
    for(int i = 0; i < cloud.xsize; ++i){
    for(int j = 0; j < cloud.ysize; ++j){
    for(int k = 0; k < cloud.zsize; ++k){
        ivec index = {i,j,k};
        if(cloud.getState(index)){
            dvec coords(3);
            cloud.convIndexToCoord(index,coords);
            scatterData << QVector3D(coords[0], coords[1], coords[2]);
        }
    }
    }
    }
    scatterSeries.dataProxy()->addItems(scatterData);
    scatterGraph.addSeries(&scatterSeries);
}
void MainWindow::boolCloudToGraph(){
    abstrItem cloudObj;
    if(!ui->treeWidget_objects->currentItem()){
    scatterSeries.dataProxy()->removeItems(0,scatterSeries.dataProxy()->itemCount());
    scatterGraph.addSeries(&scatterSeries);
    return;
    }
    int id = ui->treeWidget_objects->currentItem()->text(2).toInt();
    getObjAtID(id, cloudObj);
    boolCloudToGraph(cloudObj.cloud);
}

//Update output:
void MainWindow::updateOutput(){
    ui->pushButton_generate->setEnabled(true);
    int res = ui->spinBox_res->value();
    double min = ui->doubleSpinBox_min->value();
    double max = ui->doubleSpinBox_max->value();
    double distance = std::abs(max - min) / res;

    ui->label_distance->setText(QString::number(distance));
    ui->label_rawPoints->setText(QString::number(std::pow(res,3.0)));

}

//Abstract object management:
void MainWindow::createAbstrObj(boolCloud& cloud, std::string name){
    abstrItem cloudItem;
    if(name.size() == 0){
        name = "Point cloud";
    }
    cloudItem.id = nextObjID;
    nextObjID++;
    cloudItem.name = name;
    cloudItem.type = 0;
    cloudItem.cloud = cloud;
    allItems.push_back(cloudItem);
    createItemEntry(name, 0, cloudItem.id);
}
void MainWindow::createAbstrObj(std::vector<TRIANGLE>& triMesh, std::string name){
    abstrItem triItem;
    if(name.size() == 0){
        name = "Point cloud";
    }
    triItem.id = nextObjID;
    nextObjID++;
    triItem.name = name;
    triItem.type = 1;
    triItem.triMesh = triMesh;
    allItems.push_back(triItem);
    createItemEntry(name, 1, triItem.id);
}
void MainWindow::deleteAbstrObj(int id){
    std::list<abstrItem>::iterator it = allItems.begin();
    do{
        if(it->id == id){
            allItems.erase(it);
            break;
        }
    ++it;
    }while(it != allItems.end());
}
void MainWindow::createItemEntry(std::string name, int type, int id){
    QString typeName;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    item->setCheckState(0, Qt::Unchecked);
    item->setText(0,QString::fromStdString(name));
    if(type == 0){typeName = "boolCloud";}
    else if(type == 1){typeName = "triMesh";}
    else{typeName = "Unknown";}
    item->setText(1, typeName);
    item->setText(2, QString::number(id));
    ui->treeWidget_objects->insertTopLevelItem(0, item);
    ui->treeWidget_objects->setCurrentItem(item);
}
void MainWindow::deleteItem(){
    if(!ui->treeWidget_objects->currentItem()){
        return;
    }
    int id = ui->treeWidget_objects->currentItem()->text(2).toInt();
    delete ui->treeWidget_objects->currentItem();
    //Delete data:
    deleteAbstrObj(id);
    //View next item:
    boolCloudToGraph();
}
void MainWindow::getObjAtID(int id, abstrItem& item){
    std::list<abstrItem>::iterator it = allItems.begin();
    do{
        if(it->id == id){
            item = *it;
            break;
        }
    ++it;
    }while(it != allItems.end());
}