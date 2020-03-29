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
