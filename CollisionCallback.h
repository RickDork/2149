#ifndef COLLISIONCALLBACK_H_
#define COLLISIONCALLBACK_H_

#include <Box2D/Box2D.h>
#include "SoraIncludes.h"
#include "ShipEntity.h"
#include "Entity.h"

class CCollisionCallback : public b2ContactListener
{

    void BeginContact( b2Contact * );
    void EndContact( b2Contact * );

    CEntityContext * m_pContext;

public:

    void SetContext( CEntityContext * pContext )
    {

        m_pContext = pContext;

    }

    CCollisionCallback() : b2ContactListener(), m_pContext( NULL )
    {



    }

};

#endif
