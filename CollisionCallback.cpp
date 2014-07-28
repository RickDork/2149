#include "CollisionCallback.h"

void CCollisionCallback::BeginContact( b2Contact * contact )
{

    CEntity * entityA = static_cast< CEntity * >( contact->GetFixtureA()->GetBody()->GetUserData() );
    CEntity * entityB = static_cast< CEntity * >( contact->GetFixtureB()->GetBody()->GetUserData() );

    int entityTypeA = entityA->GetClassTypeID();
    int entityTypeB = entityB->GetClassTypeID();
    int entityTypes = ( entityTypeA | entityTypeB );

    bool involvesPlayer = ( entityTypes & ENTTYPE_PLAYER );
    bool involvesEnemy = ( entityTypes & ENTTYPE_ENEMY );

    if( involvesPlayer )
    {

        CEntity * ply = NULL;
        CEntity * ent = NULL;

        if( entityTypeA & ENTTYPE_PLAYER )
        {

            ply = entityA;
            ent = entityB;

        } else
        {

            ent = entityA;
            ply = entityB;

        }

        m_pContext->HandlePlayerContact( ply, ent );

    } else
    {

        m_pContext->HandleEntityContact( entityA, entityB );

    }



}

void CCollisionCallback::EndContact( b2Contact * contact )
{



}
