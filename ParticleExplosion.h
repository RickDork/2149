#ifndef PARTICLEEXPLOSION_H_
#define PARTICLEEXPLOSION_H_

#include "SoraIncludes.h"

#include <cmath>

#define MAX_PARTICLE_VELOCITY 650.0f
#define MAX_EXPLOSION_ARCS_PER_LAYER 8
#define MAX_PARTICLES_PER_ARC 100
#define MAX_EXPLOSION_LAYERS 10

#define DEG2RAD 0.017453f

class CExplosionSegment {
    
private:
    
    bool m_bRingExplosion;
    
    float m_MaxRange;
    float m_VelocityStop;
    float m_FadeSpeed;
    int m_LayerType;
    
    int m_Type[MAX_PARTICLES_PER_ARC];
    Vector2< float > m_AngularRange, m_Origin;
    Vector2< float > m_Size[MAX_PARTICLES_PER_ARC];
    Vector2< float > m_Velocity[MAX_PARTICLES_PER_ARC];
    Vector2< float > m_Pos[MAX_PARTICLES_PER_ARC];
    Vector4< float > m_Color[MAX_PARTICLES_PER_ARC];
    
    bool m_bActive;
    
public:
    
    void Init( float, int, Vector2< float >, float, float );
    bool IsActive() { return m_bActive; }
    bool Think( CInstancedParticleEngine &, int, float );
    
};

class CExplosionLayer {
    
private:
    
    CExplosionSegment m_ExplosionSegments[MAX_EXPLOSION_ARCS_PER_LAYER];
    
    int m_numActiveLayers;
    
public:
    
    int GetNumActiveExplosionLayers() { return m_numActiveLayers; }
    bool Think( CInstancedParticleEngine &, int , float );
    void Init( float, int, Vector2< float > );
    
};

class CParticleExplosion {
    
private:
    
    CInstancedParticleEngine m_ParticleEngine;
    CWorldEntity * m_pExplosionEnt;
    
    Vector2< float > m_Origin;
    
    CExplosionLayer m_ExplosionLayers[MAX_EXPLOSION_LAYERS];
    
    bool m_bKillMe;
    int m_StartTime;
    
    
public:
    
    CWorldEntity * GetExplosionEntity() { return m_pExplosionEnt; }
    void SetExplosionEntity( CWorldEntity * e ) { m_pExplosionEnt = e; }
    
    bool KillMe() { return m_bKillMe; }
    
    void Init( int, float, float, float );
    void Think( float );
    void Draw();
    
};



#endif
