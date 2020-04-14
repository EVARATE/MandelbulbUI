#ifndef MBULBWINDOW_H
#define MBULBWINDOW_H

#include <QDialog>
#include <QAction>
#include <vector>
#include "boolcloud.h"
#include "internalentityhandler.h"

typedef struct {
    std::string name = "default";
    double X1 = 1.75;
    double Y1 = 1.75;
    double Z1 = 1.75;
    double X2 = -1.75;
    double Y2 = -1.75;
    double Z2 = -1.75;
    int iter = 10;
    double power = 8.0;
    double maxLength = 2.0;
    int xres = 50;
    int yres = 50;
    int zres = 50;
} mBulbPreset;

namespace Ui {
class MbulbWindow;
}

class MbulbWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MbulbWindow(QWidget *parent = nullptr);
    ~MbulbWindow();

    void addPreset(mBulbPreset& preset);

    mBulbPreset defaultPreset;
    std::vector<mBulbPreset> presets;


private:
    Ui::MbulbWindow *ui;

private slots:
    void generateMBulb();
    void updateOutput();

    //Manage presets
    void changePreset();
    void getPreset(const std::string& name, mBulbPreset& preset);
    void newPreset();
    void checkPresetNameInput();
signals:
    void transferEntity(internalEntity& entity);
};

#endif // MBULBWINDOW_H
