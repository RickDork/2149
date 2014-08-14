#ifndef ENEMYAI_H_
#define ENEMYAI_H_

#include "SoraIncludes.h"

class CEnemyAI : public CAIController
{

	float m_Speed;

public:

    void Think();

	void SetSpeed( float spd )
	{

		m_Speed = spd;

	}

	CEnemyAI() : m_Speed( 100.0f )
	{ }

};

#endif
