#include "EntityManager.h"

void CTOFNEntityManager::UpdateAllEntities()
{

    boost::ptr_vector< CEntityObject > * entityObjs = &m_pRawEntityList.GetEntityObjects();

	for( boost::ptr_vector< CEntityObject >::iterator i = entityObjs->begin();
		 i != entityObjs->end(); i++ )
    {

        CEntity * e = ( *i ).GetContent();

        if( e->IsActive() )
            e->Update();

        if( e->KillMe() )
            DeleteEntity( e );

    }

}


