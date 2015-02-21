#ifndef ENEMYAI_H_
#define ENEMYAI_H_

#include "SoraIncludes.h"
#include "LuaContext.h"
#include "Entity.h"
#include "Util.h"
#include "Define.h"

class CEnemyAI : public CTemplateAIController< CLuaContext >
{
    
    Vector2< float > m_Velocity;
    
    bool m_bOnScreen;

	float m_Speed;
    float m_FlyingAngle, m_FlyingAngleSpeed;
    bool m_bCrazyFlying;
    
    int m_EnemyType;
    
public:
    

    void Think();

    void SetSpeed( float x, float y ) {
     
        m_Speed = y;
        m_Velocity.Set( x, y );
        
    }
    
	void SetSpeed( float spd )
	{

		m_Speed = spd;
        m_Velocity.Set( 0.0f, spd );

	}
    
    void SetEnemyType( int e ) {
     
        m_EnemyType = e;
        
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

	CEnemyAI() : CTemplateAIController(), m_Speed( 100.0f ), m_FlyingAngle( 0.0f ), m_bCrazyFlying( false ), m_FlyingAngleSpeed( 0.0f ), m_EnemyType( 0 ), m_bOnScreen( false )
	{
    
        
    }
    
    virtual ~CEnemyAI() { }

};

#endif
