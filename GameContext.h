#ifndef TOFNCONTEXT_H_
#define TOFNCONTEXT_H_

#include "EnemyAI.h"
#include "SoraIncludes.h"
#include "EntityManager.h"

class CTOFNContext : public CGameContext<CTOFNEntityManager>
{

private:

    CWorldEntity * m_pPlayerEntity;
    int m_MaxEnemyCount;
    int m_CurEnemyCount;

    void DoEnemyGenerator();

public:

    CWorldEntity * GetPlayerEntity()
    {

        return m_pPlayerEntity;

    }

    CWorldEntity * CreatePlayerEntity();
    CWorldEntity * CreateRandomEnemyEntity();

    void GameLogic();

    CTOFNContext();

};

#endif

