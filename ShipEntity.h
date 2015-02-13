#ifndef SHIPENTITY_H_
#define SHIPENTITY_H_

#include "SoraIncludes.h"

class CShipEntity : public CAIEntity
{

protected:

    float m_MaxHealth, m_Health, m_Armor;

    std::vector< Vector3< float > > m_GunPositions;
    std::vector< float > m_GunDmg;
    bool m_bCountAsEnemy;
	int m_NextShotTime;

public:

    CShipEntity() : CAIEntity(), m_Health( 100.0f ), m_MaxHealth( 100.0f ), m_Armor( 100.0f ), m_bCountAsEnemy( false ), m_NextShotTime( 0 )
    {

    }
    
    virtual ~CShipEntity() { }
    
	bool CanShoot()
	{

		return ( SDL_GetTicks() > m_NextShotTime );

	}

    bool CountAsEnemy()
    {

        return m_bCountAsEnemy;

    }

    void SetCountAsEnemy( bool b )
    {

        m_bCountAsEnemy = b;

    }

	void SetNextShotTime( int t )
	{

		m_NextShotTime = t;

	}

    void AddGun( float x, float y, float dmg )
    {
		const Vector2< int > & size = m_Sprite.GetSize();
        m_GunPositions.push_back( Vector3< float >( x * size.GetX(), y * size.GetY() ) );
        m_GunDmg.push_back( dmg );

    }
    
    float GetGunDamage( int j )
    {
        
        return m_GunDmg[j];
        
    }

    const std::vector< Vector3< float > > & GetGunPositions()
    {

        return m_GunPositions;

    }

    void SetHealth( float h )
    {

        m_Health = h;

    }
    
    void SetArmor( float a )
    {
        
        m_Armor = a;
        
    }
    
    void Heal( float h ) {
     
        m_Health += h;
        
        if( m_Health > m_MaxHealth )
            m_Health = m_MaxHealth;
        
    }

    void Damage( float dmg )
    {
        
        m_Armor = Util::MinF( m_Armor, -dmg, 0.0f );
        
        if( dmg > m_Armor )
            m_Health -= ( dmg - m_Armor );

    }
    
    float GetArmor()
    {
        
        return m_Armor;
        
    }

    float GetHealth()
    {

        return m_Health;

    }

};

#endif
