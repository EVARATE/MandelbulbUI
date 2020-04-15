#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include <QAction>
#include <QFileDialog>
#include <QProgressDialog>
#include <QMessageBox>
#include <QtDataVisualization>
#include <iostream>

#include "utilityFunctions.cpp"
#include "marchingCubes.cpp"
#include "mbulbwindow.h"
#include "ui_mbulbwindow.h"
#include "internalentityhandler.h"

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
    internalEntityHandler entityHandler;
    MbulbWindow *mBulbWindow = new MbulbWindow;

    QtDataVisualization::QScatter3DSeries scatterSeries;
    QtDataVisualization::Q3DScatter scatterGraph;

private slots:
    //File menu
    void actionExit();
    //Help menu
    void actionAbout();
    void actionInfo();
    //Import/Export
    QString getExportPath(const std::string& extension);
    QString getImportPath(const std::string& extension);
    void exportEntity();
    void importEntity();
    //Read/write specific data
    void writeBoolCloud(std::ofstream& ofile, internalEntity& entity);
    void writeTriMesh(std::ofstream& ofile, internalEntity& entity);
    void writePointCloud(std::ofstream& ofile, internalEntity& entity);
    void readBoolCloud(std::ifstream& ifile, internalEntity& entity);
    void readTriMesh(std::ifstream& ifile, internalEntity& entity);
    void readPointCloud(std::ifstream& ifile, internalEntity& entity);

    //Generate
    void showMBGenerator();
    //Object
    void filterSelectedHull();
    void meshCubeMarch();

    //UI
    void displayTools();
    void updatePropertyViewer();
    void setPropertyAtRow(const int row, const std::string& name, const std::string& value);
    void toggleViewport();
    void selectedEntityToGraph();
    void boolCloudToGraph(internalEntity& entity);
    void pointCloudToGraph(internalEntity& entity);

    //internal entity management
    void addEntity(internalEntity& entity);
    void deleteSelectedEntity();
    int getSelectedID();
    void getSelectedEntity(internalEntity& entity);

    //Status
    void setStatus(const std::string& message);


};
#endif // MAINWINDOW_H
