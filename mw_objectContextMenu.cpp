/*
Copyright 2020 Sebastian Motzet

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

void MainWindow::createContextMenu(const QPoint& pos){
    QTreeWidget *tree = ui->treeWidget_objects;
    if(!tree->currentItem()){return;}

    int type = getSelectedType();
    QMenu contextMenu;

    if(type == 0){
        //bCloud
        QAction *ViewAction = new QAction("View", this);
        QAction *DeleteAction = new QAction("Delete", this);
        QAction *HullAction = new QAction("Calculate Hull", this);
        QAction *MeshAction = new QAction("Calculate Mesh", this);

        contextMenu.addAction(ViewAction);
        contextMenu.addAction(DeleteAction);
        contextMenu.addAction(HullAction);
        contextMenu.addAction(MeshAction);

        connect(HullAction, SIGNAL(triggered()), this, SLOT(calcHull()));
        //CONTINUE HERE===========================================================================
    }
    else if(type == 1){

    }
    else if(type == 2){

    }
    else{
        return;
    }

    contextMenu.exec(tree->mapToGlobal(pos));
}
