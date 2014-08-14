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

void CTOFNEntityManager::DrawAllEntities()
{

    boost::ptr_vector< CEntityObject > * entityObjs = &m_pRawEntityList.GetEntityObjects();

	for( boost::ptr_vector< CEntityObject >::iterator i = entityObjs->begin();
		 i != entityObjs->end(); i++ )
    {

        CEntity * e = ( *i ).GetContent();

        if( e->IsActive() && !e->GetEntityManagerDrawOverride() )
            e->Draw();


    }

}

void CTOFNEntityManager::RemoveAllDeletedEntities()
{

    BOOST_FOREACH( CEntity * e, m_pDeletedEntities )
    {

        RemoveEntity( e );

    }

    m_pDeletedEntities.clear();

}

void CTOFNEntityManager::DeleteEntity( CEntity * pEntity )
{

    pEntity->SetActive( false );
    m_pDeletedEntities.push_back( pEntity );

}
