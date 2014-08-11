#include "Lua.h"

CLuaContext * gLuaContext = NULL;

void SetLuaContext( CLuaContext * pLuaContext )
{

    gLuaContext = pLuaContext;

}


void CTOFNLua::CallEngineFunction( const char * f )
{

    CallLuaFunction( f, "ENGINE" );

}

LuaCallBackFunction( GenerateEnemy )
{

    gLuaContext->CreateRandomEnemyEntity();

    return 0;

}

void CTOFNLua::CreateLuaHooks()
{

    LuaNameSpace( m_pLuaState, "Game" );
        LuaNameSpaceFunction( m_pLuaState, "Game", GenerateEnemy );

}