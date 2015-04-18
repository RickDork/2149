#ifndef ENEMYDATA_H_
#define ENEMYDATA_H_

#include "Vector.h"

#include <string>
#include <vector>

class CEnemyData
{

public:

	int m_Health;
	int m_SpeedMin, m_SpeedMax;
	int m_Width, m_Height;
    bool m_bCanBeInFormation;
	int m_CollisionBoxWidth, m_CollisionBoxHeight;
	std::string m_Sprite;

    std::vector< float > m_GunDmg;
	std::vector< Vector2< float > > m_GunPos;
    std::vector< Vector2< float > > m_TrailPos;
    std::vector< Vector3< float > > m_TrailColor;

};

#endif
