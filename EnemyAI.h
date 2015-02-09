#ifndef ENEMYAI_H_
#define ENEMYAI_H_

#include "SoraIncludes.h"
#include "LuaContext.h"
#include "Entity.h"
#include "Util.h"

class CEnemyAI : public CTemplateAIController< CLuaContext >
{

	float m_Speed;
    float m_FlyingAngle, m_FlyingDirection;
    bool m_bCrazyFlying;
    
public:
    

    void Think();

	void SetSpeed( float spd )
	{

		m_Speed = spd;

	}

	CEnemyAI() : m_Speed( 100.0f ), m_FlyingAngle( 0.0f )
	{
        
        m_bCrazyFlying = false;
    
        if( Util::RandomNumber( 0, 5 ) == 1 ) {
            
            m_bCrazyFlying = true;
            m_FlyingDirection = Util::RandomNumber( -2, 2 );
            
        }
        
    }

};

#endif
