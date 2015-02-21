#include "EnemyAI.h"

void CEnemyAI::Think()
{
    
    CLuaContext * p = m_pEntityContext;
    CShipEntity * s = ( CShipEntity * )m_pTargetEntity;
    
    if( m_pTargetEntity )
    {

        float vx = sin( m_FlyingAngle * DEG2RAD ) * Util::MinMaxF( m_Speed, -300.0f, 300.0f );
        float vy = m_Speed;
        
        if( m_bCrazyFlying )
            m_FlyingAngle += p->GetFrameDelta() * m_FlyingAngleSpeed;
        
        if( m_FlyingAngle > 360.0f )
            m_FlyingAngle -= 360.0f;
        
        if( m_FlyingAngle < -360.0f ){
            m_FlyingAngle += 360.0f;
        }
        
        if( s->CanShoot() && m_pEntityContext->GetPlayerEntity() ) {
            
            s->SetNextShotTime( SDL_GetTicks() + Util::RandomNumber( 500, 1500 ) );
            p->FireBulletFrom( ENTTYPE_ENBULLET, s, Util::RandomNumber( 350, 400 ) );
    
        }
        
    
    
        //Bounce around
        if( s->GetShipType() == 5 || s->GetShipType() == 7 ) {
            
            Vector2< float > pos = s->GetPos();
            Vector2< int > size = s->GetSize();
            
            float x1 = pos.GetX();
            float y1 = pos.GetY();
            float x2 = s->GetRightX();
            float y2 = s->GetBottomY();

            
            if( x2 > SCREEN_WIDTH ) {
             
                pos.SetX( SCREEN_WIDTH - size.GetX() );
                m_Velocity.SetX( -m_Speed );
            }
            
            if( y2 > SCREEN_HEIGHT ) {
             
                pos.SetY( SCREEN_HEIGHT - size.GetY() );
                m_Velocity.SetY( -m_Speed );

                
            }
            
            if( x1 < 0.0f ) {
             
                pos.SetX( 0.0f );
                m_Velocity.SetX( m_Speed );

                
            }
            
            if( y1 < 0.0f && m_bOnScreen ) {
                
                pos.SetY( 0.0f );
                m_Velocity.SetY( m_Speed );

                
            }
            
            if( y1 > 0.0f )
                m_bOnScreen = true;
            
            
            vx = m_Velocity.GetX();
            vy = m_Velocity.GetY();
            
            s->SetPos( pos );
            
        }
        
        m_pTargetEntity->Displace2( vx, vy );
        
        
    }

}
