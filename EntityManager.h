#ifndef TOFNMANAGER_H_
#define TOFNMANAGER_H_

#include <boost/foreach.hpp>
#include "SoraIncludes.h"

#define DRAW_DEPTH_MAX 10

class CTOFNEntityManager : public CEntityManager
{

    std::vector< CEntity * > m_pDeletedEntities;
    std::vector< CEntity * > m_pDrawList[DRAW_DEPTH_MAX];

public:

    void DeleteEntity( CEntity * );

    void RemoveAllDeletedEntities();
    void UpdateAllEntities();
    void DrawAllEntities();
    void AddToDrawList( CEntity * );

};

#endif
