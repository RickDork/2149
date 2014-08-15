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

	for( int i = 0; i < DRAW_DEPTH_MAX; i++ )
    {

        std::vector< CEntity * >::iterator iter = m_pDrawList[i].begin();

        for( ; iter != m_pDrawList[i].end(); iter++ )
        {

            CEntity * e = ( *i );

            if( e->IsActive() && !e->GetEntityManagerDrawOverride() )
                e->Draw();

        }

        m_pDrawList[i].clear();


    }

}

void CTOFNEntityManager::AddToDrawList( CEntity * ent )
{

    m_pDrawList[ent->GetDrawDepth()].push_back( ent );

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
