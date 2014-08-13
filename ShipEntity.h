#ifndef SHIPENTITY_H_
#define SHIPENTITY_H_

#include "SoraIncludes.h"

class CShipEntity : public CAIEntity
{

protected:

    int m_Health;

    std::vector< Vector3< float > > m_GunPositions;
	int m_NextShotTime;

public:

    CShipEntity() : CAIEntity(), m_Health( 100 )
    {

    }

	bool CanShoot()
	{

		return ( SDL_GetTicks() > m_NextShotTime );

	}


	void SetNextShotTime( int t )
	{

		m_NextShotTime = t;

	}

    void AddGun( float x, float y )
    {
		const Vector2< int > & size = m_Sprite.GetSize();
        m_GunPositions.push_back( Vector3< float >( x * size.GetX(), y * size.GetY() ) );

    }

    const std::vector< Vector3< float > > & GetGunPositions()
    {

        return m_GunPositions;

    }

    void SetHealth( int h )
    {

        m_Health = h;

    }

    void Damage( int dmg )
    {

        m_Health -= dmg;

    }

    int GetHealth()
    {

        return m_Health;

    }

};

#endif
