#ifndef BULLET_H_
#define BULLET_H_

#include "SoraIncludes.h"

enum
{

    DIR_UP = 1,
    DIR_DOWN

};

class CBulletAI : public CAIController
{

    int m_Direction;
    float m_Angle;
    float m_Damage, m_VelMul, m_BulletSpeed;

 public:

    void Think();

    void SetDirection( int d )
    {

        m_Direction = d;

        switch( m_Direction )
        {

        case DIR_UP:
            m_VelMul = -1.0f;
            break;
       default:
            m_VelMul = 1.0f;

        }

    }

	float GetDamage()
	{

		return m_Damage;

	}

    void SetDamage( float d )
    {

        m_Damage = d;

    }

    void SetSpeed( float s )
    {

        m_BulletSpeed = s;

    }
    
    void SetAngle( float a ) {
     
        m_Angle = a;
        
    }

    CBulletAI() : CAIController(), m_VelMul( 1.0f ), m_Direction( DIR_DOWN ), m_BulletSpeed( 900.0f ), m_Damage( 0.0f ), m_Angle( 0.0f )
    {

    }

};

#endif
