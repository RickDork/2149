#include "EnemyAI.h"

void CEnemyAI::Think()
{

    if( m_pTargetEntity )
    {

        m_pTargetEntity->Displace2( 0.0f, 200.0f );

    }

}
