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

#ifndef INTERNALENTITYHANDLER_H
#define INTERNALENTITYHANDLER_H

/*This class handles a set of internalEntity objects.
 *It takes care of assigning the correct id, storing, deleting, etc.
 *
 *
 */

#include "internalentity.h"
#include <list>

class internalEntityHandler
{
public:
    internalEntityHandler() {};

    void addEntity(internalEntity& entity);
    void deleteEntity(int id);

    void getEntityAtID(int id, internalEntity& entity);
    int getID();

private:
    std::list<internalEntity> entityList;
    int nextID = 0;
};

#endif // INTERNALENTITYHANDLER_H
