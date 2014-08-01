#include "CollisionCallback.h"

void CCollisionCallback::BeginContact( b2Contact * contact )
{

    CEntity * entityA = static_cast< CEntity * >( contact->GetFixtureA()->GetBody()->GetUserData() );
    CEntity * entityB = static_cast< CEntity * >( contact->GetFixtureB()->GetBody()->GetUserData() );

    CEntity * entities[] = { entityA, entityB };

    int entityTypeA = entityA->GetClassTypeID();
    int entityTypeB = entityB->GetClassTypeID();
    int entityTypes = ( entityTypeA | entityTypeB );

    bool involvesPlayer = ( entityTypes & ENTTYPE_PLAYER );
    bool involvesEnemy = ( entityTypes & ENTTYPE_ENEMY );

    if( involvesPlayer || involvesEnemy )
    {

        CEntity * mainEntity = NULL;
        CEntity * subEntity = NULL;

        int mainEntityType = 0;
        int subEntityType = 0;

        for( int j = 0; j < 2; j++ )
        {

            int curEntType = entities[j]->GetClassTypeID();

            if( ( involvesPlayer && curEntType & ENTTYPE_PLAYER ) ||
                ( !involvesPlayer && curEntType & ENTTYPE_ENEMY ) )
            {

                mainEntity = entities[j];
                subEntity = entities[1-j];
                mainEntityType = curEntType;
                subEntityType = entities[1-j]->GetClassTypeID();
                break;

            }

        }

        m_pContext->HandleEntityContact( mainEntity, mainEntityType, subEntity, subEntityType );

    }


}

void CCollisionCallback::EndContact( b2Contact * contact )
{



}
