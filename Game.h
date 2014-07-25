#ifndef GAME_H_
#define GAME_H_

#include "SoraIncludes.h"
#include "GameState.h"
#include "GameContext.h"
#include "EntityManager.h"

class CGame
{

private:

    CGraphicsContext m_GraphicsContext;
    CDrawContext m_DrawContext;
    CTOFNContext m_GameContext;
    CStateMachine m_StateMachine;
    CTextureFactory m_TextureFactory;
	CTOFNEntityManager m_EntityManager;

public:

    CGame();
    ~CGame();

};

#endif
