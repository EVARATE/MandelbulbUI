#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //display tools
    displayTools();

    //Connections
    connect(mBulbWindow, SIGNAL(transferEntity(internalEntity&)), this, SLOT(addEntity(internalEntity&)));

    //Connect actions
    connect(ui->actionExport_Entity_dat, SIGNAL(triggered()), this, SLOT(exportEntity()));
    connect(ui->actionImport_Entity_dat, SIGNAL(triggered()), this, SLOT(importEntity()));

    //ObjectViewer selection change updates property viewer:
    connect(ui->treeWidget_objectViewer, SIGNAL(itemSelectionChanged()), this, SLOT(updatePropertyViewer()));
    //ObjectViewer tools
    connect(ui->buttonDeleteObject, SIGNAL(clicked()), this, SLOT(deleteSelectedEntity()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//File menu
void MainWindow::actionExport(){

}
void MainWindow::actionImport(){

}
void MainWindow::actionExit(){

}
//Help menu
void MainWindow::actionAbout(){

}
void MainWindow::actionInfo(){

}
//Import/Export
QString MainWindow::getExportPath(const std::string& extension){
    QFileDialog exportDialog;
    exportDialog.setDefaultSuffix(QString::fromStdString(extension));

    std::string exportPath = exportDialog.getSaveFileName().toStdString();
    setFileExt(exportPath, extension);
    return QString::fromStdString(exportPath);
}
QString MainWindow::getImportPath(const std::string& extension){
    QFileDialog importDialog;
    importDialog.setDefaultSuffix(QString::fromStdString(extension));
    QString importPath = importDialog.getOpenFileName();
    std::string path = importPath.toStdString();
    setFileExt(path, extension);
    return QString::fromStdString(path);
}
void MainWindow::exportEntity(){
    //Get File path and name
    QString exportPath = getExportPath("dat");
    if(exportPath.isNull()){return;}

    //Open file
    internalEntity entity;
    getSelectedEntity(entity);
    std::ofstream ofile;
    ofile.open(exportPath.toStdString());
    if(!ofile.is_open()){
        setStatus("Couldn't open " + exportPath.toStdString());
        return;
    }
    //Write File
    ofile << entity.name << "\n";
    ofile << entity.type << "\n";
    ofile << "*properties\n";
    for(int i = 0; i < entity.properties.size(); ++i){
        ofile << entity.properties[i].name << " " << entity.properties[i].value << "\n";
    }
    ofile << "*data\n";
    if(entity.type == 0){
        writeBoolCloud(ofile, entity);
    }
    else if(entity.type == 1){
        writeTriMesh(ofile, entity);
    }
    else if(entity.type == 2){
        writePointCloud(ofile, entity);
    }
    else{
        setStatus("Error writing file. Unknown entity type: " + std::to_string(entity.type));
    }
    ofile.close();
    setStatus("Exported " + exportPath.toStdString());
}
void MainWindow::importEntity(){
    //Get File path and name
    QString importPath = getImportPath("dat");
    if(importPath.isNull()){return;}

    //Open file

    std::ifstream ifile;
    ifile.open(importPath.toStdString());
    if(!ifile.is_open()){
        setStatus("Couldn't open " + importPath.toStdString());
        return;
    }
    //Read File
    std::string entityName;
    std::string entityType;
    ifile >> entityName;
    ifile >> entityType;

    internalEntity entity(entityName, std::stoi(entityType));

    std::string beginProp;
    ifile >> beginProp;
    if(beginProp != "*properties"){
        setStatus("Error: Invalid file format: " + importPath.toStdString());
        return;
    }
    //Properties
    std::string prop;
    std::string value;
    bool endProp = false;
    do{
        ifile >> prop >> value;
        if( (prop[0] == '*') || (value[0] == '*') ){
            endProp = true;
        }
        else{
            entity.addProperty(prop, value);
        }
    }while(!endProp);

    //Read data

    if(entity.type == 0){
        //Initialize boolCloud:
        double xmin = std::stod(entity.properties[3].value);
        double ymin = std::stod(entity.properties[4].value);
        double zmin = std::stod(entity.properties[5].value);
        double xdist = std::stod(entity.properties[9].value);
        double ydist = std::stod(entity.properties[10].value);
        double zdist = std::stod(entity.properties[11].value);
        int xsize = std::stoi(entity.properties[12].value);
        int ysize = std::stoi(entity.properties[13].value);
        int zsize = std::stoi(entity.properties[14].value);
        dvec dist = {xdist, ydist, zdist};
        dvec min = {xmin, ymin, zmin};
        ivec size = {xsize, ysize, zsize};
        entity.bCloud.init(dist, min, size);
        //Read data
        readBoolCloud(ifile, entity);
    }
    else if(entity.type == 1){
        readTriMesh(ifile, entity);
    }
    else if(entity.type == 2){
        readPointCloud(ifile, entity);
    }
    else{
        setStatus("Error reading file. Unknown entity type: " + std::to_string(entity.type));
    }
    ifile.close();

    addEntity(entity);
}

//Read/write specific data
void MainWindow::writeBoolCloud(std::ofstream& ofile, internalEntity& entity){
    for(int i = 0; i < entity.bCloud.xsize; ++i){
    for(int j = 0; j < entity.bCloud.ysize; ++j){
    for(int k = 0; k < entity.bCloud.zsize; ++k){
        ivec index = {i,j,k};
        if(entity.bCloud.getState(index)){
            ofile << "1";
        }else{
            ofile << "0";
        }
        ofile << " ";
    }ofile << "\n";
    }ofile << "\n";
    }
}
void MainWindow::writeTriMesh(std::ofstream& ofile, internalEntity& entity){
    for(int i = 0; i < entity.triMesh.size(); ++i){
        TRIANGLE& triangle = entity.triMesh[i];
        ofile << triangle.p[0][0] << " " << triangle.p[0][1] << " " << triangle.p[0][2] << "\n";
        ofile << triangle.p[1][0] << " " << triangle.p[1][1] << " " << triangle.p[1][2] << "\n";
        ofile << triangle.p[2][0] << " " << triangle.p[2][1] << " " << triangle.p[2][2] << "\n";
    }
}
void MainWindow::writePointCloud(std::ofstream& ofile, internalEntity& entity){
    for(int i = 0; i < entity.pointCloud.size(); ++i){
        dvec& point = entity.pointCloud[i];
        ofile << point[0] << " " << point[1] << " " << point[2] << "\n";
    }
}
void MainWindow::readBoolCloud(std::ifstream& ifile, internalEntity& entity){
    for(int i = 0; i < entity.bCloud.xsize; ++i){
    for(int j = 0; j < entity.bCloud.ysize; ++j){
    for(int k = 0; k < entity.bCloud.zsize; ++k){
        std::string binState;
        ivec index = {i,j,k};
        ifile >> binState;
        if(binState[0] == '1'){
            entity.bCloud.setState(index, true);
        }else{
            entity.bCloud.setState(index, false);
        }

    }
    }
    }
}
void MainWindow::readTriMesh(std::ifstream& ifile, internalEntity& entity){
     do{
        dvec pointA(3);
        ifile >> pointA[0] >> pointA[1] >> pointA[2];
        dvec pointB(3);
        ifile >> pointB[0] >> pointB[1] >> pointB[2];
        dvec pointC(3);
        ifile >> pointC[0] >> pointC[1] >> pointC[2];
        TRIANGLE triangle;
        triangle.p[0] = pointA;
        triangle.p[1] = pointB;
        triangle.p[2] = pointC;
        entity.triMesh.push_back(triangle);
    }while(!ifile.eof());
}
void MainWindow::readPointCloud(std::ifstream& ifile, internalEntity& entity){

}

//Generate
void MainWindow::showMBGenerator(){
    mBulbWindow->setWindowModality(Qt::ApplicationModal);
    mBulbWindow->exec();

}

//UI
void MainWindow::displayTools(){
    //Actions
    QAction *actionGenMB = new QAction("New MB", this);
    actionGenMB->setIcon(QIcon(":/icons/cil-plus.svg"));
    actionGenMB->setToolTip("New Mandelbulb");

    //Toolbar
    ui->toolBar->addAction(actionGenMB);
    ui->toolBar->addSeparator();

    //Action menu
    ui->menuGenerate->addAction(actionGenMB);

    //Connections
    connect(actionGenMB, SIGNAL(triggered()), this, SLOT(showMBGenerator()));
}
void MainWindow::updatePropertyViewer(){
    internalEntity entity;
    getSelectedEntity(entity);
    int rowCount = 3 + entity.properties.size();
    ui->tableWidget_objectProperties->setRowCount(rowCount);
    //name, type, id
    setPropertyAtRow(0, "name", entity.name);
    setPropertyAtRow(1, "type", std::to_string(entity.type));
    setPropertyAtRow(2, "id", std::to_string(entity.id));
    //properties
    for(int i = 3; i < entity.properties.size() + 3; ++i){
        setPropertyAtRow(i, entity.properties[i - 3].name, entity.properties[i - 3].value);
    }
}
void MainWindow::setPropertyAtRow(const int row, const std::string& name, const std::string& value){
    ui->tableWidget_objectProperties->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(name), 0));
    ui->tableWidget_objectProperties->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(value), 0));
}

//internal entity management
void MainWindow::addEntity(internalEntity& entity){
    //Entity in memory:
    entityHandler.addEntity(entity);
    //Entry in Object viewer:
    auto *item = new QTreeWidgetItem();
    item->setFlags(item->flags() | Qt::ItemIsSelectable);
    item->setText(0, QString::fromStdString(entity.name));

    item->setText(1, QString::fromStdString(entity.typeName));
    item->setText(2, QString::number(entity.id));
    ui->treeWidget_objectViewer->insertTopLevelItem(0, item);
    ui->treeWidget_objectViewer->setCurrentItem(item);
}
void MainWindow::deleteSelectedEntity(){
    internalEntity entity;
    getSelectedEntity(entity);
    if(entity.isEmpty){return;}
    //Remove viewer entry
    delete ui->treeWidget_objectViewer->currentItem();
    //Delete data
    entityHandler.deleteEntity(entity.id);
}
int MainWindow::getSelectedID(){
    if(!ui->treeWidget_objectViewer->currentItem()){return -1;}
    return ui->treeWidget_objectViewer->currentItem()->text(2).toInt();
}
void MainWindow::getSelectedEntity(internalEntity& entity){
    int id = getSelectedID();
    if(id == -1){
        entity = entityHandler.returnEmptyEntity();
    }else{
        entityHandler.getEntityAtID(id, entity);
    }

}

//Status
void MainWindow::setStatus(const std::string& message){
    ui->statusbar->showMessage(QString::fromStdString(message));
}
