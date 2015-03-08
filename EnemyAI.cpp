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
        
        if( s->GetShipType() != 9 && s->CanShoot() && m_pEntityContext->GetPlayerEntity() ) {
            
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
    
    if( s->GetShipType() == 9 ) {
        
        if( s->GetY() > 0.0f ) {
         
            s->SetPos( s->GetX(), 0.0f );
            m_bBossModeEnable = true;
            
            p->ToggleBossMode( true );
            
        }
        
        if( m_bBossModeEnable ) {
            
            if( s->CanShoot() ) {
                
                switch( m_BossShootType ) {
             
                    case 0:
                    case 1: {
                        
                        s->SetNextShotTime( SDL_GetTicks() + 50 );
                     
                        CWorldEntity * e = p->FireBulletFrom( ENTTYPE_ENBULLET, m_BossCurGun * 50.0f, 100.0f, 10.0f, 900.0f, 270.0f );
                        e->SetSize( 100, 100 );
                        
                        if( m_BossShootType == 0 ) {
                        
                            m_BossCurGun++;
                            
                            if( m_BossCurGun == 8 ) {
                             
                                m_BossCurGun = 11;
                                
                            }
                            
                            if( m_BossCurGun >= 20 ) {
                             
                                s->SetNextShotTime( SDL_GetTicks() + 300 );
                                m_BossCurGun = 19;
                                
                                m_BossShootType = Util::RandomNumber( 1, 3 );
                                m_BossShootIter = 0;
                            }
                            
                        } else {
                         
                            m_BossCurGun--;
                            
                            if( m_BossCurGun == 11 ) {
                                
                                m_BossCurGun = 8;
                                
                            }
                            
                            if( m_BossCurGun < 0 ) {
                                
                                s->SetNextShotTime( SDL_GetTicks() + 1000 );
                                m_BossCurGun = 0;
                                m_BossShootType = 0;
                                
                            }
                            
                            
                        }
                        
                        break;
                        
                    }
                    case 2:
                    case 3: {
                        
                        float x = 0.0f;
                        float y = 100.0f;
                        float startang = 270.0f;
                        float spreadang = 10.0f;
                        
                        for( int i = 0; i < 3; i++ ) {
                        
                            if( i == 1 ) {
                                
                                x = SCREEN_WIDTH;
                                startang = 270.0f;
                                spreadang = -10.0f;
                                
                            } else if( i == 2 ) {
                             
                                x = SCREEN_WIDTH * .5f;
                                startang = 225.0f;
                                spreadang = 20.0f;
                                
                            }
                            
                            for( int j = 0; j < 5; j++ ) {
                                
                                CWorldEntity * e = p->FireBulletFrom( ENTTYPE_ENBULLET, x, y, 10.0f, 250.0f, startang );
                                startang += spreadang;
                                e->SetSize( 100, 100 );
                                
                            }
                            
                        }
                        
                        bool nextStage = false;
                        
                        if( m_BossShootType == 3 ) {
                            m_BossShootIter++;
                            
                            if( m_BossShootIter >= 6 ) {
                             
                                nextStage = true;
                                
                            } else
                                s->SetNextShotTime( SDL_GetTicks() + 400 );
                            
                        } else {
                            
                            nextStage = true;
                        }
                        
                        if( nextStage ) {
                         
                            s->SetNextShotTime( SDL_GetTicks() + 1000 );
                            m_BossShootType = 4;
                            m_BossShootIter = 0;
                            
                        }
                        
                        break;
                        
                    }
                    case 4: {
                        for( int j = 0; j < 80; j++ )
                            p->FireBulletFrom( ENTTYPE_ENBULLET,  j * 10, 100.0f, 2.0f, Util::RandomNumber( 200, 350 ) );
                        
                        s->SetNextShotTime( SDL_GetTicks() + 1000 );
                        m_BossShootIter++;
                        
                        if( m_BossShootIter >= 4 ) {
                         
                            m_BossShootType = Util::RandomNumber( 0, 5 );
                            m_BossShootIter = 0;
                            
                        }
                        
                    }
                    case 5: {
                        for( int j = 0; j < 6; j++ ) {
                            CWorldEntity * e = p->FireBulletFrom( ENTTYPE_ENBULLET, 200 * j, 100.0f, 10.0f, 400.0f, 270.0f );
                            e->SetSize( 100, 100 );
                        }
                        
                        m_BossShootIter++;
                        
                        if( m_BossShootIter < 5 )
                            s->SetNextShotTime( SDL_GetTicks() + 400 );
                        else {
                            
                            m_BossShootIter = 0;
                            m_BossShootType = Util::RandomNumber( 0, 4 );
                            s->SetNextShotTime( SDL_GetTicks() + 1200 );
                            
                        }
                        
                    }
                        
                }
                
            }
            
        } else {
         
            s->SetHealth( s->GetMaxHealth() );
            
        }
        
        p->SetBossHealthPercent( ( float )s->GetHealth() / ( float )s->GetMaxHealth() );
        
    }

}
