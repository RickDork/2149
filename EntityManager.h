#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

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
