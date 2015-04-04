#ifndef COLLISIONCALLBACK_H_
#define COLLISIONCALLBACK_H_

#include "SoraIncludes.h"
#include "ShipEntity.h"
#include "Entity.h"

class CCollisionCallback : public CCollisionListener
{

    CEntityContext * m_pContext;

public:

    void BeginContact( CCollisionContact & );

    
    void SetContext( CEntityContext * pContext )
    {

        m_pContext = pContext;

    }

    CCollisionCallback() : CCollisionListener(), m_pContext( NULL )
    {



    }

};

#endif
