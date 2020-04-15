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

    //ObjectViewer selection change updates property viewer:
    connect(ui->treeWidget_objectViewer, SIGNAL(itemSelectionChanged()), this, SLOT(updatePropertyViewer()));
    //ObjectViewer tools
    connect(ui->buttonDeleteObject, SIGNAL(clicked()), this, SLOT(deleteSelectedEntity()));

    //Scatter graph
    connect(ui->buttonShowObject, SIGNAL(clicked()), this, SLOT(selectedEntityToGraph()));
    connect(ui->buttonToggleViewport, SIGNAL(clicked()), this, SLOT(toggleViewport()));
    QWidget *container = QWidget::createWindowContainer(&scatterGraph, this);
    ui->vL_Viewport->addWidget(container);
    scatterGraph.setAspectRatio(1.0);
    scatterGraph.setHorizontalAspectRatio(1.0);


}

MainWindow::~MainWindow()
{
    delete ui;
}

//File menu
void MainWindow::actionExit(){
    this->close();
}
//Help menu
void MainWindow::actionAbout(){
    QMessageBox aboutBox;
    aboutBox.about(this,"MandelbulbUI", "Made with Qt Creator\n\nLicensed under the GNU General Public License v3.0\n\nCopyright 2019, 2020 Sebastian Motzet");
}
void MainWindow::actionInfo(){
    QMessageBox infoBox;
    infoBox.setWindowTitle("Info");
    infoBox.setText("Please refer to the <a href='https://github.com/EVARATE/MandelbulbUI/wiki'>wiki</a> for detailed information.");
    infoBox.exec();
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
    //Check if any entity is selected
    if(!ui->treeWidget_objectViewer->currentItem()){return;}
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

    if(std::stoi(entityType) == -1){
        setStatus("Error: invalid entity type: -1");
        ifile.close();
        return;
    }

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
        double xmin = std::stod(entity.properties[4].value);
        double ymin = std::stod(entity.properties[5].value);
        double zmin = std::stod(entity.properties[6].value);
        double xdist = std::stod(entity.properties[10].value);
        double ydist = std::stod(entity.properties[11].value);
        double zdist = std::stod(entity.properties[12].value);
        int xsize = std::stoi(entity.properties[13].value);
        int ysize = std::stoi(entity.properties[14].value);
        int zsize = std::stoi(entity.properties[15].value);
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
//Object
void MainWindow::filterSelectedHull(){
    internalEntity cloudEntity;
    getSelectedEntity(cloudEntity);
    if(cloudEntity.type != 0){return;}
    boolCloud& cloud = cloudEntity.bCloud;

    ivec size = {cloudEntity.bCloud.xsize, cloudEntity.bCloud.ysize, cloudEntity.bCloud.zsize};
    dvec min = {cloudEntity.bCloud.xmin, cloudEntity.bCloud.ymin, cloudEntity.bCloud.zmin};
    dvec dist = {cloudEntity.bCloud.xdistance, cloudEntity.bCloud.ydistance, cloudEntity.bCloud.zdistance};
    boolCloud hull(dist, min, size);

    QProgressDialog progDialog;
    progDialog.setLabelText("Filtering Hull...");
    progDialog.setCancelButton(0);
    double progress = 0.0;
    double progdiv = 100.0 / ((hull.xsize-1)*(hull.ysize-1)*(hull.zsize-1));
    int pC = 0;

    for(int i = 0; i < hull.xsize; ++i){
    for(int j = 0; j < hull.ysize; ++j){
    for(int k = 0; k < hull.zsize; ++k){
        ivec active = {i,j,k};
         dvec coord(3);
         hull.convIndexToCoord(active,coord);
         if(cloud.getState(active)){
                  if(!cloud.getState(i-1,j-1,k-1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i-1,j-1,k  )){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i-1,j-1,k+1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i-1,j  ,k-1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i-1,j  ,k  )){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i-1,j  ,k+1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i-1,j+1,k-1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i-1,j+1,k  )){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i-1,j+1,k+1)){hull.setState(active,true);pC++;}

             else if(!cloud.getState(i  ,j-1,k-1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i  ,j-1,k  )){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i  ,j-1,k+1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i  ,j  ,k-1)){hull.setState(active,true);pC++;}

             else if(!cloud.getState(i  ,j  ,k+1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i  ,j+1,k-1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i  ,j+1,k  )){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i  ,j+1,k+1)){hull.setState(active,true);pC++;}

             else if(!cloud.getState(i+1,j-1,k-1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i+1,j-1,k  )){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i+1,j-1,k+1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i+1,j  ,k-1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i+1,j  ,k  )){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i+1,j  ,k+1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i+1,j+1,k-1)){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i+1,j+1,k  )){hull.setState(active,true);pC++;}
             else if(!cloud.getState(i+1,j+1,k+1)){hull.setState(active,true);pC++;}
         }
         //Display progress:
         progress += progdiv;
         progDialog.setValue(progress);
    }
    }
    }
    setStatus("Filtered Hull");
    internalEntity hullEntity(hull, "Hull");
    addEntity(hullEntity);
}

//UI
void MainWindow::displayTools(){
    //Actions
    QAction *actionGenMB = new QAction("Generate Mandelbulb", this);
    actionGenMB->setIcon(QIcon(":/icons/cil-plus.svg"));
    actionGenMB->setToolTip("New Mandelbulb");

    QAction *actionFilterHull = new QAction("Filter Hull", this);
    actionFilterHull->setIcon(QIcon(":/icons/icon_hull.svg"));
    actionFilterHull->setToolTip("Filter Hull of boolCloud entity");

    QAction *actionExportEntity = new QAction("Export entity (.dat)");
    actionExportEntity->setIcon(QIcon(":/icons/cil-save.svg"));
    actionExportEntity->setToolTip("Export selected entity");

    QAction *actionImportEntity = new QAction("Import entity (.dat)");
    actionImportEntity->setIcon(QIcon(":/icons/cil-folder-open.svg"));
    actionImportEntity->setToolTip(("Import entity (.dat)"));

    QAction *actionExit = new QAction("Exit");
    actionExit->setToolTip("Exit the program");

    QAction *actionInfo = new QAction("Info");
    actionInfo->setToolTip("Show Info");

    QAction *actionAbout = new QAction("About");
    actionAbout->setToolTip("About the program");

    //Toolbar
    ui->toolBar->addAction(actionGenMB);
    ui->toolBar->addAction(actionFilterHull);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionExportEntity);
    ui->toolBar->addAction(actionImportEntity);

    //Generate Menu
    ui->menuGenerate->addAction(actionGenMB);
    //File menu
    ui->menuFile->addAction(actionExportEntity);
    ui->menuFile->addAction(actionImportEntity);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(actionExit);
    //Object menu
    ui->menuObject->addAction(actionFilterHull);
    //Help menu
    ui->menuHelp->addAction(actionInfo);
    ui->menuHelp->addAction(actionAbout);

    //Connections
    connect(actionGenMB, SIGNAL(triggered()), this, SLOT(showMBGenerator()));
    connect(actionFilterHull, SIGNAL(triggered()), this, SLOT(filterSelectedHull()));
    connect(actionExportEntity, SIGNAL(triggered()), this, SLOT(exportEntity()));
    connect(actionImportEntity, SIGNAL(triggered()), this, SLOT(importEntity()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(actionExit()));
    connect(actionInfo, SIGNAL(triggered()), this, SLOT(actionInfo()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(actionAbout()));
}
void MainWindow::updatePropertyViewer(){
    internalEntity entity;
    getSelectedEntity(entity);
    if(entity.id == -1){
        ui->tableWidget_objectProperties->setRowCount(0);
        return;
    }
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
void MainWindow::toggleViewport(){
    static bool checked = false;
    if(checked){
        scatterGraph.show();
        ui->buttonToggleViewport->setIcon(QIcon("qrc://icons/cil-screen-desktop_filled_custom.svg"));
        checked = false;
    }else{
        scatterGraph.hide();
        ui->buttonToggleViewport->setIcon(QIcon("qrc://icons/cil-screen-desktop.svg"));
        checked = true;
    }
}
void MainWindow::selectedEntityToGraph(){
    if(!ui->treeWidget_objectViewer->currentItem()){
        scatterSeries.dataProxy()->removeItems(0, scatterSeries.dataProxy()->itemCount());
        //scatterGraph.addSeries(&scatterSeries);
        return;
    }
    internalEntity entity;
    getSelectedEntity(entity);
    if(entity.type == 0){
        //boolCloud
        boolCloudToGraph(entity);
    }
    else if(entity.type == 1){
        //triMesh
        //not supported
        return;
    }
    else if(entity.type == 2){
        //pointCloud
        pointCloudToGraph(entity);
    }
    else{
        return;
    }
    scatterGraph.seriesList().at(0)->setMesh(QtDataVisualization::QAbstract3DSeries::MeshCube);
}
void MainWindow::boolCloudToGraph(internalEntity& entity){
    scatterSeries.dataProxy()->removeItems(0,scatterSeries.dataProxy()->itemCount());
    QtDataVisualization::QScatterDataArray scatterData;
    boolCloud& cloud = entity.bCloud;
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
void MainWindow::pointCloudToGraph(internalEntity& entity){
    scatterSeries.dataProxy()->removeItems(0, scatterSeries.dataProxy()->itemCount());
    QtDataVisualization::QScatterDataArray scatterData;
    std::vector<dvec>& pointCloud = entity.pointCloud;
    for(int i = 0; i < pointCloud.size(); ++i){
        scatterData << QVector3D(pointCloud[i][0], pointCloud[i][1], pointCloud[i][2]);
    }
    scatterSeries.dataProxy()->addItems(scatterData);
    scatterGraph.addSeries(&scatterSeries);
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
    scatterSeries.dataProxy()->removeItems(0, scatterSeries.dataProxy()->itemCount());
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
