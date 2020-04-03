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

//Algorithm:
void MainWindow::calcMBulb(){
    //See if 'auto' is checked:
    bool autoHull = ui->checkBox_autoHull->isChecked();
    bool autoMesh = ui->checkBox_autoMesh->isChecked();
    //Adapt UI:
    ui->pushButton_generate->setEnabled(false);
    if(autoHull){
        ui->checkBox_autoHull->setEnabled(false);
    }
    //Get input values:
    int res = ui->spinBox_res->value();
    int iter = ui->spinBox_iter->value();
    double min = ui->doubleSpinBox_min->value();
    double max = ui->doubleSpinBox_max->value();
    double power = ui->doubleSpinBox_power->value();
    double maxLength = ui->doubleSpinBox_maxLength->value();
    double distance = std::abs(max - min) / res;
    dvec vDist(3,distance);
    dvec vMin(3,min);
    ivec vSize(3,res);
    //Save input values:
    inputValues.res = res;
    inputValues.iter = iter;
    inputValues.min = min;
    inputValues.max = max;
    inputValues.power = power;
    inputValues.maxLength = maxLength;
    //Create cloud object:
    boolCloud mBulb(vDist,vMin,vSize);

    ui->label_setPoints->setText("0");
    setStatus("Generating Mandelbulb...");

    //Calculation:
          double density = 0;
          double xpos, ypos, zpos;
          double xpoint, ypoint, zpoint;
          double cx, cy, cz;
          double r, phi, theta;

          //Track progress:
          double progress = 0.0;
          double progdiv = 100.0 / pow(double(res - 3), 3.0);
          int pointCount = 0;

          for(xpos = min; xpos <= max; xpos += distance){
            for(ypos = min; ypos <= max; ypos += distance){
            for(zpos = min; zpos <= max; zpos += distance){
                //reset for next point:
                xpoint = xpos;
                ypoint = ypos;
                zpoint = zpos;
                cx = xpos;
                cy = ypos;
                cz = zpos;

                //Sequence loop:
                for(int i = 0; i <= iter; i++)
                {
                    r = sqrt(xpoint*xpoint + ypoint*ypoint + zpoint*zpoint);
                    phi = atan(ypoint/xpoint);
                    theta = acos(zpoint/r);

                    xpoint = pow(r, power) * sin(power * theta) * cos(power * phi) + cx;
                    ypoint = pow(r, power) * sin(power * theta) * sin(power * phi) + cy;
                    zpoint = pow(r, power) * cos(power * theta) + cz;

                    if(r >= maxLength)
                    {
                        density = 0.0;
                        break;
                    }
                    density = 1.0;

                }

                //Display progress:
                progress += progdiv;
                ui->progressBar->setValue(progress);

                if(density == 1.0)
                {
                    //Update Set:
                    pointCount++;
                    ui->label_setPoints->setText(QString::number(pointCount));
                    dvec coord = {xpos,ypos,zpos};
                    mBulb.setState(coord,true);
                }

            }//zpos loop end
            }//ypos loop end
            }//xpos loop end
          setStatus("Generated Mandelbulb");
          std::string name = "Mandelbulb";
          mBulb.pointCount = pointCount;

          internalEntity mBulbEntity(mBulb,name);
          mBulbEntity.data0 = iter;
          mBulbEntity.data1 = power;
          mBulbEntity.data2 = maxLength;
          entityHandler.addEntity(mBulbEntity);
          createEntry(mBulbEntity.name, mBulbEntity.type, mBulbEntity.id);

          if(autoHull){
              calcHull(mBulb);
          }
          else if(autoMesh){
              generateMesh(mBulb);
          }
}
