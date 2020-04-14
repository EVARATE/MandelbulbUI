#ifndef MBULBWINDOW_H
#define MBULBWINDOW_H

#include <QDialog>
#include <QAction>
#include <vector>
#include "boolcloud.h"
#include "internalentityhandler.h"


namespace Ui {
class MbulbWindow;
}

class MbulbWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MbulbWindow(QWidget *parent = nullptr);
    ~MbulbWindow();

private:
    Ui::MbulbWindow *ui;

    //Default parameters:
    double defX1 = 1.75;
    double defY1 = 1.75;
    double defZ1 = 1.75;
    double defX2 = -1.75;
    double defY2 = -1.75;
    double defZ2 = -1.75;
    int defIter = 10;
    double defPower = 8.0;
    double defMaxLength = 2.0;
    int defXres = 50;
    int defYres = 50;
    int defZres = 50;

private slots:
    void generateMBulb();
    void updateOutput();
signals:
    void transferEntity(internalEntity& entity);
};

#endif // MBULBWINDOW_H
