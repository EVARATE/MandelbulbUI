#include "mbulbwindow.h"
#include "ui_mbulbwindow.h"

MbulbWindow::MbulbWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MbulbWindow)
{
    ui->setupUi(this);
    updateOutput();

    //Connections
    connect(ui->ButtonGenerate, SIGNAL(clicked()), this, SLOT(generateMBulb()));


    //RESET BUTTONS
    //X1, Y1, Z1, X2, Y2, Z2
    connect(ui->buttonResetX1, &QToolButton::clicked, this, [this](){ui->X1Input->setValue(defX1);});
    connect(ui->buttonResetY1, &QToolButton::clicked, this, [this](){ui->Y1Input->setValue(defY1);});
    connect(ui->buttonResetZ1, &QToolButton::clicked, this, [this](){ui->Z1Input->setValue(defZ1);});
    connect(ui->buttonResetX2, &QToolButton::clicked, this, [this](){ui->X2Input->setValue(defX2);});
    connect(ui->buttonResetY2, &QToolButton::clicked, this, [this](){ui->Y2Input->setValue(defY2);});
    connect(ui->buttonResetZ2, &QToolButton::clicked, this, [this](){ui->Z2Input->setValue(defZ2);});
    //iter, power, maxLength
    connect(ui->buttonResetIter, &QToolButton::clicked, this, [this](){ui->IterInput->setValue(defIter);});
    connect(ui->buttonResetPower, &QToolButton::clicked, this, [this](){ui->IterInput->setValue(defPower);});
    connect(ui->buttonResetMaxLength, &QToolButton::clicked, this, [this](){ui->IterInput->setValue(defMaxLength);});
    //xres, yres, zres
    connect(ui->buttonResetXres, &QToolButton::clicked, this, [this](){ui->xresInput->setValue(defXres);});
    connect(ui->buttonResetYres, &QToolButton::clicked, this, [this](){ui->yresInput->setValue(defYres);});
    connect(ui->buttonResetZres, &QToolButton::clicked, this, [this](){ui->zresInput->setValue(defZres);});

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
}
