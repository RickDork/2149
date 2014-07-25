#ifndef TOFNMANAGER_H_
#define TOFNMANAGER_H_

#include <boost/foreach.hpp>
#include "SoraIncludes.h"

class CTOFNEntityManager : public CEntityManager
{

    std::vector< CEntity * > m_pDeletedEntities;

public:

    void DeleteEntity( CEntity * );

    void RemoveAllDeletedEntities();
    void UpdateAllEntities();
    void DrawAllEntities();

};

#endif
