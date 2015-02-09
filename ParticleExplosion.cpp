#include "ParticleExplosion.h"

void CExplosionSegment::Init( float range, int type, Vector2< float > pos, float minRange, float maxRange ) {
    
    m_bActive = true;
    
    m_bRingExplosion = ( type == 0 || type == 3 )? true : false;
    
    m_LayerType = type;
    m_Origin = pos;
    m_AngularRange.Set( minRange, maxRange );
    
    if( range < 0.0f )
        m_MaxRange = Util::RandomNumber( 2, 6 );
    else
        m_MaxRange = range;
    
    if( !m_bRingExplosion ) {
        
        m_VelocityStop = Util::RandomNumber( 1200, 1800 );
        m_FadeSpeed = Util::RandomNumber( 300, 350 );
        
    } else {
     
        if( type == 3 )
            m_FadeSpeed = 100;
        else
            m_FadeSpeed = 500;
        m_VelocityStop = Util::RandomNumber( 2000, 2800 );
    
    }
    
    for( int j = 0; j < MAX_PARTICLES_PER_ARC; j++ ) {
        
        float mul = ( !m_bRingExplosion )? ( float )( Util::RandomNumber( 20, 100 ) ) / 100.0f : 1.0f;
        float theta = Util::RandomNumber( minRange, maxRange );
        
        float vx = MAX_PARTICLE_VELOCITY * std::sin(  theta * DEG2RAD ) * mul;
        float vy =  MAX_PARTICLE_VELOCITY * std::cos( theta * DEG2RAD ) * mul;
        float size = ( float )Util::RandomNumber( 7, 12 );
        
        m_Type[j] = Util::RandomNumber( 1, 3 );
        m_Size[j].Set( size, size );
        m_Pos[j].Set( pos.GetX(), pos.GetY() );
        m_Color[j].Set( 255.0f, 255.0f, 255.0f, 128.0f );
        m_Velocity[j].Set( vx, vy );

        
    }
    
}

bool CExplosionSegment::Think( CInstancedParticleEngine & particleEngine, int particleOffset, float dps ) {
    
    bool hasActiveParticles = false;
    
    for( int j = 0; j < MAX_PARTICLES_PER_ARC; j++ ) {
        
        float x, y;
        float vx, vy;
        float r, g, b, a;
        float rD = 0.0f, gD = 0.0f, bD = 0.0f, aD = 0.0f;
        
        m_Color[j].Get( &r, &g, &b, &a );
        m_Velocity[j].Get( &vx, &vy );
        m_Pos[j].Get( &x, &y );
        
        if( a > 0.0f ) {
            
            float newx = x + vx * dps;
            float newy = y + vy * dps;
    
            hasActiveParticles = true;
            
            m_Pos[j].Set( newx, newy + 300.0f * dps );
            int type = m_Type[j];
            
            if( !m_bRingExplosion ) {
                
                if( type == 1 ) {
                
                    rD = 30.0f;
                    gD = 450.0f;
                    bD = 900.0f;
                    aD = m_FadeSpeed * .9f;
                    
                } else if( type == 2 ) {
                    
                    rD = 30.0f;
                    gD = 900.0f;
                    bD = 1800.0f;
                    aD = m_FadeSpeed;
                    
                } else if( type == 3 ) {
                    
                    rD = 20.0f;
                    gD = 250.0f;
                    bD = 550.0f;
                    aD = m_FadeSpeed;
                    
                } else {
                 
                    rD = 0.0f;
                    gD = 0.0f;
                    bD = 0.0f;
                    aD = m_FadeSpeed * .95f;
                    
                }
                
            } else {
                
                if( m_LayerType == 3 ) {
                    
                    rD = 30.0f;
                    gD = 900.0f;
                    bD = 0.0f;
                    aD = m_FadeSpeed;
                    
                } else {
                    
                    rD = 0.0f;
                    gD = 0.0f;
                    bD = 0.0f;
                    aD = m_FadeSpeed;
                }
                    
            }
            
            if( m_LayerType == 2 )
                bD = 0.0f;
        
            if( m_LayerType > 0 && Util::Dist( m_Pos[j], m_Origin ) > m_MaxRange ) {
            
                float vxmul = 1.0f;
                float vymul = 1.0f;
                float tmul = 1.0f;
                
                if( m_Type[j] == 4 )
                    tmul = .9f;
                
                if( vx > 0.0f )
                    vxmul = -1.0f;
                
                if( vy > 0.0f )
                    vymul = -1.0f;
                
                vx = Util::MinMaxF( vx, m_VelocityStop * vxmul * tmul * dps, 0.0f );
                vy = Util::MinMaxF( vy, m_VelocityStop * vymul * tmul * dps, 0.0f );
                
                m_Velocity[j].Set( vx, vy );
            }
            
            /*
            m_ExplosionMaxVelocity[j].Get( &mvx, &mvy );
            
            int elapsed = SDL_GetTicks() - m_StartTime;
            bool core = false;
            
            if( elapsed == 0 )
                elapsed = 1;
            
            if( sqrtf( mvx*mvx + mvy*mvy ) < 100.0f )
                core = true;
            
            if( elapsed > 150 ) {
                
                if( core ) {
                    
                    rD = 0.0f;
                    gD = 0.0f;
                    
                } else {
                    
                    rD = 345.0f;
                    gD = 500.0f;
                    
                }
                
                aD = 605.0f;
                
            } else {
                
                rD = 20.0f;
                gD = 10.0f;
                aD = 10.0f;
                
            }
            
            //if( Util::Dist( x, y, m_Origin.GetX() , m_Origin.GetY() ) < 50.0f )
            //    bD = 300.0f;
            
            if( !core )
                bD = 700.0f;
            
            m_ExplosionPos[j].Set( newx, newy );
            m_ExplosionVelocity[j].Set( mvx * Util::MaxF( ( 1000 / elapsed ), 1.0f ), mvy * Util::MaxF( ( 1000 / elapsed ), 1.0f ) );*/
            
            float newR = Util::MinMaxF( r, -rD * dps, 0.0f );
            float newG = Util::MinMaxF( g, -gD * dps, 0.0f );
            float newB = Util::MinMaxF( b, -bD * dps, 0.0f );
            float newA = Util::MinMaxF( a, -aD * dps, 0.0f );
            
            m_Color[j].Set( newR, newG, newB, newA );

        }

        
        particleEngine.SetParticleSize( particleOffset + j, m_Size[j].GetX(), m_Size[j].GetY() );
        particleEngine.SetParticleColor( particleOffset + j, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f );
        particleEngine.SetParticlePos( particleOffset + j, x, y );
        
    }
    
    if( !hasActiveParticles )
        m_bActive = false;
    
    return m_bActive;
    
}

void CExplosionLayer::Init( float range, int type, Vector2< float > pos ) {
 
    float expTheta = 360.0f / ( float )MAX_EXPLOSION_ARCS_PER_LAYER;
    m_numActiveLayers = MAX_EXPLOSION_ARCS_PER_LAYER;
    
    for( int j = 0; j < MAX_EXPLOSION_ARCS_PER_LAYER; j++ ) {
     
        float minRange = j * expTheta;
        float maxRange = ( j == MAX_EXPLOSION_ARCS_PER_LAYER - 1 )? 360.0f : ( j + 1 ) * expTheta;
        
        m_ExplosionSegments[j].Init( range, type, pos, minRange, maxRange );
        
    }
    
    
    
}

bool CExplosionLayer::Think( CInstancedParticleEngine & particleEngine, int particleOffset, float dps ) {
 
    for( int j = 0; j < MAX_EXPLOSION_ARCS_PER_LAYER; j++ ) {
     
        if( m_ExplosionSegments[j].IsActive() )
            if( !m_ExplosionSegments[j].Think( particleEngine, particleOffset + j * MAX_PARTICLES_PER_ARC, dps ) )
                m_numActiveLayers--;
        
    }
    
    if( m_numActiveLayers )
        return true;
    
    return false;
    
}


void CParticleExplosion::Init( int type, float range, float x, float y ) {

    m_ParticleEngine.Init( MAX_EXPLOSION_LAYERS * MAX_EXPLOSION_ARCS_PER_LAYER * MAX_PARTICLES_PER_ARC, 1 );
    
    m_Origin.Set( x, y );
    m_StartTime = SDL_GetTicks();
    m_bKillMe = false;
    
    int ringType = 0;
    
    if( type == 2 )
        ringType = 3;
    
    for( int j = 0; j < MAX_EXPLOSION_LAYERS; j++ ) {
     
        m_ExplosionLayers[j].Init( range, ( j == 0 ) ? ringType : type, Vector2< float >( x + Util::RandomNumber( -20, 20 ), y + Util::RandomNumber( -20, 20 ) ) );
        
    }

    
}

void CParticleExplosion::Think( float dps ) {
 
    m_bKillMe = true;
    
    for( int j = 0; j < MAX_EXPLOSION_LAYERS; j++ ) {
     
        int pOffset = j * MAX_EXPLOSION_ARCS_PER_LAYER * MAX_PARTICLES_PER_ARC;
        
        if( m_ExplosionLayers[j].Think( m_ParticleEngine, pOffset, dps ) )
            m_bKillMe = false;
        
        
    }
  
    
}


void CParticleExplosion::Draw() {

    m_ParticleEngine.BindVertexBuffers();
    m_ParticleEngine.Draw();
    
}