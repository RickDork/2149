#ifndef ENEMYAI_H_
#define ENEMYAI_H_

#include "SoraIncludes.h"
#include "LuaContext.h"
#include "Entity.h"
#include "Util.h"

class CEnemyAI : public CTemplateAIController< CLuaContext >
{

	float m_Speed;
    float m_FlyingAngle, m_FlyingAngleSpeed;
    bool m_bCrazyFlying;
    
public:
    

    void Think();

	void SetSpeed( float spd )
	{

		m_Speed = spd;

	}
    
    void SetCrazyFlying( bool b ) {
     
        m_bCrazyFlying = b;
        
    }
    
    void SetFlyingAngle( float f ) {
     
        m_FlyingAngle = f;
        
    }
    
    void SetFlyingAngleSpeed( float f ) {
     
        m_FlyingAngleSpeed = f;
        
    }

	CEnemyAI() : CTemplateAIController(), m_Speed( 100.0f ), m_FlyingAngle( 0.0f ), m_bCrazyFlying( false ), m_FlyingAngleSpeed( 0.0f )
	{
    
        
    }
    
    virtual ~CEnemyAI() { }

};

#endif
