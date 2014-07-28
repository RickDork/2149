#include "Bullet.h"

void CBulletAI::Think()
{

    if( m_pTargetEntity )
    {

        m_pTargetEntity->Displace2( 0.0f, m_BulletSpeed * m_VelMul );

    }

}

