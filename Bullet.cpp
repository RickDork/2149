#include "Bullet.h"

void CBulletAI::Think()
{

    if( m_pTargetEntity )
    {

        m_pTargetEntity->Displace2( m_BulletSpeed * std::cos( m_Angle  * DEG2RAD), m_BulletSpeed * std::sin( m_Angle * DEG2RAD ) * -1.0f );

    }

}

