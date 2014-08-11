#ifndef SHIPENTITY_H_
#define SHIPENTITY_H_

#include "SoraIncludes.h"

class CShipEntity : public CAIEntity
{

protected:

    int m_Health;

    std::vector< Vector3< float > > m_GunPositions;

public:

    CShipEntity() : CAIEntity(), m_Health( 100 )
    {

    }

    void AddGun( float x, float y )
    {

        m_GunPositions.push_back( Vector3< float >( x, y ) );

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
