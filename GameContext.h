#ifndef GAMECONTEXT_H_
#define GAMECONTEXT_H_

#include "SoraIncludes.h"

class CTOFNContext : public CGameContext
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

