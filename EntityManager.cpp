#include "EntityManager.h"

void CTOFNEntityManager::UpdateAllEntities()
{

    boost::ptr_vector< CEntityObject > * entityObjs = m_pRawEntityList->GetEntityObjects();

    BOOST_FOREACH( CEntityObject * o, *entityObjs )
    {

        o->GetContent()->Update();

    }

}

void CTOFNEntityManager::DrawAllEntities()
{

    boost::ptr_vector< CEntityObject > * entityObjs = m_pRawEntityList->GetEntityObjects();

    BOOST_FOREACH( CEntityObject * o, *entityObjs )
    {

        o->GetContent()->Draw();

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

    m_pDeletedEntities.push_back( pEntity );

}
