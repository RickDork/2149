#include "EnemyAI.h"

void CEnemyAI::Think()
{
    
    CLuaContext * p = m_pEntityContext;
    CShipEntity * s = ( CShipEntity * )m_pTargetEntity;
    
    if( m_pTargetEntity )
    {

        m_pTargetEntity->Displace2( sin( m_FlyingAngle * DEG2RAD ) * Util::MinMaxF( m_Speed, -300.0f, 300.0f ), m_Speed );
        
        if( m_bCrazyFlying )
            m_FlyingAngle += p->GetFrameDelta() * m_FlyingAngleSpeed;
        
        if( m_FlyingAngle > 360.0f )
            m_FlyingAngle -= 360.0f;
        
        if( m_FlyingAngle < -360.0f ){
            m_FlyingAngle += 360.0f;
        }
        
        if( s->CanShoot() ) {
            
            s->SetNextShotTime( SDL_GetTicks() + Util::RandomNumber( 500, 1500 ) );
            p->FireBulletFrom( ENTTYPE_ENBULLET, s, Util::RandomNumber( 350, 400 ) );
    
        }
        
    }

}
