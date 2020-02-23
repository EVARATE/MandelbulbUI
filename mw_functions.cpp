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

//Methods:
int MainWindow::getSelectedID(){
        return ui->treeWidget_objects->currentItem()->text(2).toInt();
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
void MainWindow::actionSaveBoolCloud(){
    int id = getSelectedID();
    abstrItem cloudObj;
    getObjAtID(id, cloudObj);
    actionSaveBoolCloud(cloudObj.cloud);
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
        createAbstrObj(cloud, "Cloud import");
    }
    else{
        ui->label_infoText->setText(QString::fromStdString("ERROR: Invalid file extension: " + extension));
    }

}
void MainWindow::actionSaveTriMesh(std::vector<TRIANGLE>& triMesh){
    //Save dialog:
    QFileDialog saveDialog;
    saveDialog.setDefaultSuffix("obj");
    QString fileName = saveDialog.getSaveFileName();
    std::string filePath = fileName.toStdString();
    setFileExt(filePath, "obj");

    //Quick .obj exporter:
    std::ofstream ofile;
    ofile.open(filePath);
    if(!ofile.is_open()){return;}
    std::string polyBuffer;
    int lineCounter = 0;
    ofile << "#MandelbulbUI: Marching cubes mesh\n";
    ofile << "o MandelbulbUI Mesh\n";
    for(int i = 0; i < triMesh.size(); ++i){
        ofile << "v " << triMesh[i].p[0][0] << " " << triMesh[i].p[0][1] << " " << triMesh[i].p[0][2] << "\n";
        ofile << "v " << triMesh[i].p[1][0] << " " << triMesh[i].p[1][1] << " " << triMesh[i].p[1][2] << "\n";
        ofile << "v " << triMesh[i].p[2][0] << " " << triMesh[i].p[2][1] << " " << triMesh[i].p[2][2] << "\n";
        lineCounter++; polyBuffer.append("f " + std::to_string(lineCounter));
        lineCounter++; polyBuffer.append(" " + std::to_string(lineCounter));
        lineCounter++; polyBuffer.append(" " + std::to_string(lineCounter) + "\n");
    }
    ofile << polyBuffer;
    ofile.close();
    ui->label_infoText->setText(QString::fromStdString("Saved mesh to " + filePath ));
}
void MainWindow::actionSaveTriMesh(){
    int id = getSelectedID();
    abstrItem cloudObj;
    getObjAtID(id, cloudObj);
    actionSaveTriMesh(cloudObj.triMesh);
}

//Info/About Dialog:
void MainWindow::actionInfo(){
    QMessageBox infoBox;
    infoBox.setTextFormat(Qt::RichText);
    infoBox.setWindowTitle("Info");
    infoBox.setText("Please refer to the <a href='https://github.com/EVARATE/MandelbulbUI/wiki'>wiki</a> for detailed information.");
    infoBox.exec();
}
void MainWindow::actionAbout(){
    QMessageBox aboutBox;
    aboutBox.about(this,"Mandelbulb_V2 Pre Release",
    "Made with Qt Creator 4.11\n\nLicensed under the GNU General Public License v3.0\n\nCopyright 2019, 2020 Sebastian Motzet");
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
void MainWindow::createAbstrObj(std::vector<dvec>& pointSet, std::string name){
    abstrItem triItem;
    if(name.size() == 0){
        name = "Point cloud";
    }
    triItem.id = nextObjID;
    nextObjID++;
    triItem.name = name;
    triItem.type = 2;
    triItem.pointSet = pointSet;
    allItems.push_back(triItem);
    createItemEntry(name, 2, triItem.id);
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
    item->setFlags(item->flags() | Qt::ItemIsSelectable);
    item->setText(0,QString::fromStdString(name));
    if(type == 0){
        typeName = "boolCloud";
        item->setIcon( 0, QIcon(":/icons/icons/icon_bcloud.png") );
    }
    else if(type == 1){
        typeName = "triMesh";
        item->setIcon(0,(QIcon(":/icons/icons/icon_mesh.png")));
    }
    else if(type == 2){
        typeName = "pointSet";
        item->setIcon(0,(QIcon(":/icons/icons/icon_pointSet.png")));
    }
    else{
        typeName = "Unknown";
        item->setIcon(0,(QIcon(":/icons/icons/icon_unknown.png")));
    }
    item->setText(1, typeName);
    item->setText(2, QString::number(id));
    ui->treeWidget_objects->insertTopLevelItem(0, item);
    ui->treeWidget_objects->setCurrentItem(item);
}
void MainWindow::deleteItem(){
    if(!ui->treeWidget_objects->currentItem()){
        return;
    }
    int id = getSelectedID();
    delete ui->treeWidget_objects->currentItem();
    //Delete data:
    deleteAbstrObj(id);
    //View next item:
    boolCloudToGraph();
}
void MainWindow::updateActionAvailability(){
    if(!ui->treeWidget_objects->currentItem()){
        ui->action_saveInternal->setEnabled(false);
        ui->actionFilter_Hull->setEnabled(false);
        ui->actionGenerate_Mesh->setEnabled(false);
        ui->action_saveBCloud->setEnabled(false);
        ui->action_saveMeshObj->setEnabled(false);
        //Activate Generate button:
        ui->pushButton_generate->setEnabled(true);
        return;
    }
    int id = getSelectedID();
    abstrItem item;
    getObjAtID(id, item);
    if(item.type == 0){
        //bool cloud:
        ui->action_saveInternal->setEnabled(true);
        ui->actionFilter_Hull->setEnabled(true);
        ui->actionGenerate_Mesh->setEnabled(true);
        ui->action_saveBCloud->setEnabled(true);
        ui->action_saveMeshObj->setEnabled(false);
    }else if(item.type == 1){
        ui->action_saveInternal->setEnabled(false);
        ui->actionFilter_Hull->setEnabled(false);
        ui->actionGenerate_Mesh->setEnabled(false);
        ui->action_saveBCloud->setEnabled(false);
        ui->action_saveMeshObj->setEnabled(true);
    }else{
        ui->action_saveInternal->setEnabled(false);
        ui->actionFilter_Hull->setEnabled(false);
        ui->actionGenerate_Mesh->setEnabled(false);
        ui->action_saveBCloud->setEnabled(false);
        ui->action_saveMeshObj->setEnabled(false);
    }
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
