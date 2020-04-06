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
        QAction *MeshAction = new QAction("Generate Mesh", this);
        QAction *SaveAction = new QAction("Export (.txt, .bin)", this);

        contextMenu.addAction(ViewAction);
        contextMenu.addAction(DeleteAction);
        contextMenu.addSeparator();
        contextMenu.addAction(HullAction);
        contextMenu.addAction(MeshAction);
        contextMenu.addSeparator();
        contextMenu.addAction(SaveAction);


        connect(ViewAction, SIGNAL(triggered()), this, SLOT(selectedToGraph()));
        connect(DeleteAction, SIGNAL(triggered()), this, SLOT(deleteEntry()));
        connect(HullAction, SIGNAL(triggered()), this, SLOT(calcHull()));
        connect(MeshAction, SIGNAL(triggered()), this, SLOT(generateMesh()));
        connect(SaveAction, SIGNAL(triggered()), this, SLOT(actionSaveBoolCloud()));
    }
    else if(type == 1){
        //triMesh
        QAction *DeleteAction = new QAction("Delete", this);
        QAction *ExportAction = new QAction("Export (.obj)", this);
        QAction *ExportActionOld = new QAction("Export (old) (.obj)", this);

        contextMenu.addAction(DeleteAction);
        contextMenu.addAction(ExportAction);
        contextMenu.addAction(ExportActionOld);

        connect(DeleteAction, SIGNAL(triggered()), this, SLOT(deleteEntry()));
        connect(ExportAction, SIGNAL(triggered()), this, SLOT(actionExportMesh()));
        connect(ExportActionOld, SIGNAL(triggered()), this, SLOT(actionSaveTriMesh()));
    }
    else if(type == 2){
        //pointCloud
        QAction *ViewAction = new QAction("View", this);

        contextMenu.addAction(ViewAction);

        connect(ViewAction, SIGNAL(triggered()), this, SLOT(selectedToGraph()));
    }
    else{
        return;
    }

    contextMenu.exec(tree->mapToGlobal(pos));
}
