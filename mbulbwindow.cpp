#include "mbulbwindow.h"
#include "ui_mbulbwindow.h"

MbulbWindow::MbulbWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MbulbWindow)
{
    ui->setupUi(this);
    addPreset(defaultPreset);
    checkPresetNameInput();

    //Connections
    connect(ui->ButtonGenerate, SIGNAL(clicked()), this, SLOT(generateMBulb()));

    //RESET BUTTONS
    //X1, Y1, Z1, X2, Y2, Z2
    connect(ui->buttonResetX1, &QToolButton::clicked, this, [this](){ui->X1Input->setValue(defaultPreset.X1);});
    connect(ui->buttonResetY1, &QToolButton::clicked, this, [this](){ui->Y1Input->setValue(defaultPreset.Y1);});
    connect(ui->buttonResetZ1, &QToolButton::clicked, this, [this](){ui->Z1Input->setValue(defaultPreset.Z1);});
    connect(ui->buttonResetX2, &QToolButton::clicked, this, [this](){ui->X2Input->setValue(defaultPreset.X2);});
    connect(ui->buttonResetY2, &QToolButton::clicked, this, [this](){ui->Y2Input->setValue(defaultPreset.Y2);});
    connect(ui->buttonResetZ2, &QToolButton::clicked, this, [this](){ui->Z2Input->setValue(defaultPreset.Z2);});
    //iter, power, maxLength
    connect(ui->buttonResetIter, &QToolButton::clicked, this, [this](){ui->IterInput->setValue(defaultPreset.iter);});
    connect(ui->buttonResetPower, &QToolButton::clicked, this, [this](){ui->powerInput->setValue(defaultPreset.power);});
    connect(ui->buttonResetMaxLength, &QToolButton::clicked, this, [this](){ui->maxLengthInput->setValue(defaultPreset.maxLength);});
    //xres, yres, zres
    connect(ui->buttonResetXres, &QToolButton::clicked, this, [this](){ui->xresInput->setValue(defaultPreset.xres);});
    connect(ui->buttonResetYres, &QToolButton::clicked, this, [this](){ui->yresInput->setValue(defaultPreset.yres);});
    connect(ui->buttonResetZres, &QToolButton::clicked, this, [this](){ui->zresInput->setValue(defaultPreset.zres);});

    //Input change updates output values
    connect(ui->X1Input, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->Y1Input, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->Z1Input, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->X2Input, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->Y2Input, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->Z2Input, SIGNAL(valueChanged(double)), this, SLOT(updateOutput()));
    connect(ui->xresInput, SIGNAL(valueChanged(int)), this, SLOT(updateOutput()));
    connect(ui->yresInput, SIGNAL(valueChanged(int)), this, SLOT(updateOutput()));
    connect(ui->zresInput, SIGNAL(valueChanged(int)), this, SLOT(updateOutput()));

    //Input options
    connect(ui->checkBox_equalValues_P1, SIGNAL(stateChanged(int)), this, SLOT(equalP1Values()));
    connect(ui->checkBox_equalValues_P2, SIGNAL(stateChanged(int)), this, SLOT(equalP2Values()));
    connect(ui->checkBox_equalValues_res, SIGNAL(stateChanged(int)), this, SLOT(equalResValues()));
    connect(ui->checkBox_equidistance, SIGNAL(stateChanged(int)), this, SLOT(evenDistribution()));

    //Preset change
    connect(ui->comboBox_presets, SIGNAL(currentIndexChanged(int)), this, SLOT(changePreset()));
    connect(ui->buttonNewPreset, SIGNAL(clicked()), this, SLOT(newPreset()));
    connect(ui->lineEdit_newPresetName, SIGNAL(textEdited(QString)), this, SLOT(checkPresetNameInput()));
}

MbulbWindow::~MbulbWindow()
{
    delete ui;
}

void MbulbWindow::generateMBulb(){
    //UI
    ui->ButtonGenerate->setEnabled(false);
    ui->pointCountOutput->setText("0");
    //Get input parameters
    double x1 = ui->X1Input->value();
    double y1 = ui->Y1Input->value();
    double z1 = ui->Z1Input->value();
    double x2 = ui->X2Input->value();
    double y2 = ui->Y2Input->value();
    double z2 = ui->Z2Input->value();
    int xres = ui->xresInput->value();
    int yres = ui->yresInput->value();
    int zres = ui->zresInput->value();
    int iter = ui->IterInput->value();
    int power = ui->powerInput->value();
    int maxLength = ui->maxLengthInput->value();
    //Derive further parameters
    double xdist = std::abs(x1 - x2) / xres;
    double ydist = std::abs(y1 - y2) / yres;
    double zdist = std::abs(z1 - z2) / zres;
    double xmin = std::min(x1, x2);
    double ymin = std::min(y1, y2);
    double zmin = std:: min(z1, z2);
    double xmax = std::max(x1, x2);
    double ymax = std::max(y1, y2);
    double zmax = std:: max(z1, z2);
    //initialize boolCloud object
    dvec dist = {xdist, ydist, zdist};
    dvec min = {xmin, ymin, zmin};
    ivec size = {xres, yres, zres};
    boolCloud mBulb(dist, min, size);
    //Generate mandelbulb
    double density = 0;
    double xpos, ypos, zpos;
    double xpoint, ypoint, zpoint;
    double cx, cy, cz;
    double r, phi, theta;
    double progress = 0.0;
    double progdiv = 100.0 / ((xres-1) * (yres-1) * (zres-1));
    int pointCount = 0;
    for(xpos = xmin; xpos < xmax; xpos += xdist){
    for(ypos = ymin; ypos < ymax; ypos += ydist){
    for(zpos = zmin; zpos < zmax; zpos += zdist){
        //reset
        xpoint = xpos;
        ypoint = ypos;
        zpoint = zpos;
        cx = xpos;
        cy = ypos;
        cz = zpos;
        //Sequence
        for(int i = 0; i < iter; ++i){
            r = sqrt(xpoint*xpoint + ypoint*ypoint + zpoint*zpoint);
            phi = atan(ypoint/xpoint);
            theta = acos(zpoint/r);
            xpoint = pow(r, power) * sin(power * theta) * cos(power * phi) + cx;
            ypoint = pow(r, power) * sin(power * theta) * sin(power * phi) + cy;
            zpoint = pow(r, power) * cos(power * theta) + cz;
            if(r >= maxLength){
                density = 0.0;
                break;
            }
            density = 1.0;
        }
        progress += progdiv;
        ui->progressBar->setValue(progress);
        if(density == 1.0){
            pointCount++;
            ui->pointCountOutput->setText(QString::number(pointCount));
            dvec coord{xpos, ypos, zpos};
            mBulb.setState(coord, true);
        }
    }
    }
    }
    //Create entity
    internalEntity entity(mBulb, "Mandelbulb");
    entity.addProperty("pointCount", pointCount);
    entity.addProperty("iter", iter);
    entity.addProperty("power", power);
    entity.addProperty("maxLength", maxLength);
    entity.addProperty("xmin", xmin);
    entity.addProperty("ymin", ymin);
    entity.addProperty("zmin", zmin);
    entity.addProperty("xmax", xmax);
    entity.addProperty("ymax", ymax);
    entity.addProperty("zmax", zmax);
    entity.addProperty("xdist", xdist);
    entity.addProperty("ydist", ydist);
    entity.addProperty("zdist", zdist);
    entity.addProperty("xres", xres);
    entity.addProperty("yres", yres);
    entity.addProperty("zres", zres);


    emit transferEntity(entity);
    ui->ButtonGenerate->setEnabled(true);
    ui->progressBar->setValue(0);
    this->close();
}
void MbulbWindow::updateOutput(){
    double x1 = ui->X1Input->value();
    double y1 = ui->Y1Input->value();
    double z1 = ui->Z1Input->value();
    double x2 = ui->X2Input->value();
    double y2 = ui->Y2Input->value();
    double z2 = ui->Z2Input->value();
    int xres = ui->xresInput->value();
    int yres = ui->yresInput->value();
    int zres = ui->zresInput->value();
    double xdist = std::abs(x1 - x2) / xres;
    double ydist = std::abs(y1 - y2) / yres;
    double zdist = std::abs(z1 - z2) / zres;
    ui->xdistOutput->setText(QString::number(xdist));
    ui->ydistOutput->setText(QString::number(ydist));
    ui->zdistOutput->setText(QString::number(zdist));
    ui->gridSlotsOutput->setText(QString::number(xres*yres*zres));
    ui->progressBar->setValue(0);
}
//Input options
void MbulbWindow::equalP1Values(){

}
void MbulbWindow::equalP2Values(){

}
void MbulbWindow::equalResValues(){

}
void MbulbWindow::evenDistribution(){

}

void MbulbWindow::addPreset(mBulbPreset &preset){
    presets.push_back(preset);
    ui->comboBox_presets->addItem(QString::fromStdString(preset.name));
}

//Manage presets
void MbulbWindow::getPreset(const std::string& name, mBulbPreset& preset){
    auto it = presets.begin();
    do{
        if(it->name == name){
            preset = *it;
            break;
        }
        ++it;
    }while(it != presets.end());
}
void MbulbWindow::newPreset(){
    mBulbPreset preset;
    preset.name = ui->lineEdit_newPresetName->text().toStdString();
    preset.X1 = ui->X1Input->value();
    preset.Y1 = ui->Y1Input->value();
    preset.Z1 = ui->Z1Input->value();
    preset.X2 = ui->X2Input->value();
    preset.Y2 = ui->Y2Input->value();
    preset.Z2 = ui->Z2Input->value();

    preset.iter = ui->IterInput->value();
    preset.power = ui->powerInput->value();
    preset.maxLength = ui->maxLengthInput->value();

    preset.xres = ui->xresInput->value();
    preset.yres = ui->yresInput->value();
    preset.zres = ui->zresInput->value();

    addPreset(preset);

    ui->buttonNewPreset->setEnabled(false);
    ui->comboBox_presets->setCurrentIndex(ui->comboBox_presets->findText(QString::fromStdString(preset.name)));
}
void MbulbWindow::checkPresetNameInput(){
    std::string inputText = ui->lineEdit_newPresetName->text().toStdString();
    bool exists = false;
    //check if name is empty
    if( (inputText.size() == 0) ){
        exists = true;
    }else{
        //check if name already exists
        auto it = presets.begin();
        do{
            if(it->name == inputText){
                exists = true;
                break;
            }
            ++it;
        }while(it != presets.end());
        if(presets.end()->name == inputText){
            exists = true;
        }
    }
    //act accordingly
    if(exists){
        ui->buttonNewPreset->setEnabled(false);
    }else{
        ui->buttonNewPreset->setEnabled(true);
    }
}
void MbulbWindow::changePreset(){
    std::string presetName = ui->comboBox_presets->currentText().toStdString();
    mBulbPreset preset;
    getPreset(presetName, preset);

    ui->checkBox_equidistance->setChecked(false);
    ui->checkBox_equalValues_P1->setChecked(false);
    ui->checkBox_equalValues_P2->setChecked(false);
    ui->checkBox_equalValues_res->setChecked(false);

    ui->X1Input->setValue(preset.X1);
    ui->Y1Input->setValue(preset.Y1);
    ui->Z1Input->setValue(preset.Z1);
    ui->X2Input->setValue(preset.X2);
    ui->Y2Input->setValue(preset.Y2);
    ui->Z2Input->setValue(preset.Z2);

    ui->IterInput->setValue(preset.iter);
    ui->powerInput->setValue(preset.power);
    ui->maxLengthInput->setValue(preset.maxLength);

    ui->xresInput->setValue(preset.xres);
    ui->yresInput->setValue(preset.yres);
    ui->zresInput->setValue(preset.zres);

}
