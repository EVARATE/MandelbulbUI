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
    if(!ui->treeWidget_objects->currentItem()){return -1;}
        return ui->treeWidget_objects->currentItem()->text(2).toInt();
}
int MainWindow::getSelectedType(){
    int id = getSelectedID();
    internalEntity selectedEntity;
    entityHandler.getEntityAtID(id, selectedEntity);
    return selectedEntity.type;
}

void MainWindow::actionSaveBoolCloud(boolCloud& cloud){
    //Select file:
    QFileDialog saveDialog;
    saveDialog.setDefaultSuffix("bin");
    QString fileName = saveDialog.getSaveFileName();
    if(fileName.isNull()){
        return;
    }

    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath, "bin");

    //Write data:
    if(extension == "txt"){
        cloud.saveCoords(filePath);
    }
    else if(extension == "bin"){
        cloud.saveInternal(filePath);
    }
    setStatus("Saved cloud to " + filePath);
}
void MainWindow::actionSaveBoolCloud(){
    int id = getSelectedID();
    internalEntity cloudObj;
    entityHandler.getEntityAtID(id, cloudObj);
    actionSaveBoolCloud(cloudObj.bCloud);
}
void MainWindow::actionLoadBoolCloud(){
    //Select file:
    QFileDialog loadDialog;
    loadDialog.setDefaultSuffix("bin");
    QString fileName = loadDialog.getOpenFileName();
    if(fileName.isNull()){
        return;
    }

    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath);

    //Create and load object:
    boolCloud cloud;
    if(extension == "bin"){
        cloud.loadInternal(filePath);
        setStatus("Loaded " + filePath);
        internalEntity bCloudEntity(cloud, "Imported boolCloud");
        entityHandler.addEntity(bCloudEntity);
        createEntry(bCloudEntity.name, bCloudEntity.type, bCloudEntity.id);
    }
    else{
        setStatus("ERROR: Invalid file extension: " + extension);
    }

}
void MainWindow::actionSaveTriMesh(triVec& triMesh){
    //Save dialog:
    QFileDialog saveDialog;
    saveDialog.setDefaultSuffix("obj");
    QString fileName = saveDialog.getSaveFileName();
    if(fileName.isNull()){
        return;
    }

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
    setStatus("Saved mesh to " + filePath);
}
void MainWindow::actionSaveTriMesh(){
    int id = getSelectedID();
    internalEntity cloudObj;
    entityHandler.getEntityAtID(id, cloudObj);
    actionSaveTriMesh(cloudObj.triMesh);
}
void MainWindow::actionLoadpointCloud(){
    //Select file:
    QFileDialog loadDialog;
    loadDialog.setDefaultSuffix("txt");
    QString fileName = loadDialog.getOpenFileName();
    if(fileName.isNull()){
        return;
    }

    std::string filePath = fileName.toStdString();
    std::string extension = getFileExt(filePath);

    //Create and load object:
    std::vector<dvec> pointCloud;
    dvec point(3);
    if(extension == "txt"){
        std::ifstream ifile;
        ifile.open(filePath);
        if(!ifile.is_open()){
            setStatus("Couldn't open file: " + filePath);
            return;
        }
        while(true){
            if(ifile.eof()){
                break;
            }else{
                ifile >> point[0] >> point[1] >> point[2];
                pointCloud.push_back(point);
            }
        }
        ifile.close();
        internalEntity pointCloudEntity(pointCloud, "Point Cloud");
        entityHandler.addEntity(pointCloudEntity);
        createEntry(pointCloudEntity.name, pointCloudEntity.type, pointCloudEntity.id);
        setStatus("Imported file: " + filePath);
    }else{
        setStatus("Invalid file extension: " + extension);
        return;
    }
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
void MainWindow::selectedToGraph(){
    if(!ui->treeWidget_objects->currentItem()){
    scatterSeries.dataProxy()->removeItems(0,scatterSeries.dataProxy()->itemCount());
    scatterGraph.addSeries(&scatterSeries);
    return;
    }

    internalEntity selectedEntity;
    int id = getSelectedID();
    entityHandler.getEntityAtID(id, selectedEntity);
    int type = selectedEntity.type;

    if(type == 0){
        //boolcloud
        boolCloudToGraph(selectedEntity.bCloud);
    }
    else if(type == 1){
        //triMesh
        setStatus("Can't yet render mesh in viewport.");
    }
    else if(type == 2){
        //type = pointcloud
        pointCloudToGraph(selectedEntity.pointCloud);
    }
    else{
        return;
    }
    scatterGraph.seriesList().at(0)->setMesh(QtDataVisualization::QAbstract3DSeries::MeshCube);
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

void MainWindow::pointCloudToGraph(std::vector<dvec>& pointCloud){
    scatterSeries.dataProxy()->removeItems(0, scatterSeries.dataProxy()->itemCount());
    QtDataVisualization::QScatterDataArray scatterData;
    for(int i = 0; i < pointCloud.size(); ++i){
        scatterData << QVector3D(pointCloud[i][0], pointCloud[i][1], pointCloud[i][2]);
    }
    scatterSeries.dataProxy()->addItems(scatterData);
    scatterGraph.addSeries(&scatterSeries);
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
void MainWindow::updatePropertyViewer(){
    //Remove all rows
    ui->tableWidget_ObjectProperties->setRowCount(0);

    //Get selected object:

    int id = getSelectedID();
    if(id == -1){return;}
    internalEntity selectedEntity;
    entityHandler.getEntityAtID(id, selectedEntity);
    int type = selectedEntity.type;

    //Display base properties
    ui->tableWidget_ObjectProperties->setRowCount(3);
    setPropertyAtRow(0, "name", selectedEntity.name);
    setPropertyAtRow(1, "type", std::to_string(selectedEntity.type));
    setPropertyAtRow(2, "id", std::to_string(selectedEntity.id));

    //Display properties depending on entity type
    if(type == 0){
        //bCloud
        ui->tableWidget_ObjectProperties->setRowCount(19);
        //distance:
        setPropertyAtRow(3, "xdistance", std::to_string(selectedEntity.bCloud.xdistance));
        setPropertyAtRow(4, "ydistance", std::to_string(selectedEntity.bCloud.ydistance));
        setPropertyAtRow(5, "zdistance", std::to_string(selectedEntity.bCloud.zdistance));
        //min
        setPropertyAtRow(6, "xmin", std::to_string(selectedEntity.bCloud.xmin));
        setPropertyAtRow(7, "ymin", std::to_string(selectedEntity.bCloud.ymin));
        setPropertyAtRow(8, "zmin", std::to_string(selectedEntity.bCloud.zmin));
        //max
        setPropertyAtRow(9, "xmax", std::to_string(selectedEntity.bCloud.xmax));
        setPropertyAtRow(10, "ymax", std::to_string(selectedEntity.bCloud.ymax));
        setPropertyAtRow(11, "zmax", std::to_string(selectedEntity.bCloud.zmax));
        //size
        setPropertyAtRow(12, "xsize", std::to_string(selectedEntity.bCloud.xsize));
        setPropertyAtRow(13, "ysize", std::to_string(selectedEntity.bCloud.ysize));
        setPropertyAtRow(14, "zsize", std::to_string(selectedEntity.bCloud.zsize));
        //other
        setPropertyAtRow(15, "pointCount", std::to_string(selectedEntity.bCloud.pointCount));
        //custom data
        setPropertyAtRow(16, "iterations", std::to_string(selectedEntity.data0));
        setPropertyAtRow(17, "power", std::to_string(selectedEntity.data1));
        setPropertyAtRow(18, "maxLength", std::to_string(selectedEntity.data2));

    }
    else if(type == 1){
        //triMesh
        ui->tableWidget_ObjectProperties->setRowCount(4);
        setPropertyAtRow(3, "Triangles", std::to_string(selectedEntity.triMesh.size()));
    }
    else if(type == 2){
        //pointCloud
    ui->tableWidget_ObjectProperties->setRowCount(4);
        setPropertyAtRow(3, "pointCount", std::to_string(selectedEntity.pointCloud.size()));
    }
    else{
        return;
    }
}
void MainWindow::setPropertyAtRow(const int row, const std::string& name, const std::string& value){
    ui->tableWidget_ObjectProperties->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(name), 0));
    ui->tableWidget_ObjectProperties->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(value), 0));
}
void MainWindow::setStatus(const std::string& statusMessage){
    ui->statusBar->showMessage(QString::fromStdString(statusMessage));
}

void MainWindow::createEntry(const std::string& name, int type, int id){
    QString typeName;
    auto *item = new QTreeWidgetItem();
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
        typeName = "pointCloud";
        item->setIcon(0,(QIcon(":/icons/icons/icon_pointCloud.png")));
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
void MainWindow::deleteEntry(){
    if(!ui->treeWidget_objects->currentItem()){
        return;
    }
    int id = getSelectedID();
    delete ui->treeWidget_objects->currentItem();
    //Delete data:
    entityHandler.deleteEntity(id);
    //View next item:
    selectedToGraph();
}
void MainWindow::updateActionAvailability(){
    if(!ui->treeWidget_objects->currentItem()){
        ui->action_saveInternal->setEnabled(false);
        ui->actionFilter_Hull->setEnabled(false);
        ui->actionGenerate_Mesh->setEnabled(false);
        ui->action_saveBCloud->setEnabled(false);
        ui->action_saveMeshObj->setEnabled(false);
        ui->pushButton_viewObject->setEnabled(false);
        //Activate Generate button:
        ui->pushButton_generate->setEnabled(true);
        return;
    }
    ui->pushButton_viewObject->setEnabled(true);

    int id = getSelectedID();
    internalEntity item;
    entityHandler.getEntityAtID(id, item);
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
