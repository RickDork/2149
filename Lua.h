#ifndef LUA_H_
#define LUA_H_

#include "SoraIncludes.h"
#include "EntityManager.h"
#include "LuaContext.h"

class CTOFNLua : public CLuaObject
{

protected:

    void CreateLuaHooks();

public:

    void CallEngineFunction( const char * );


};

void SetLuaContext( CLuaContext * );

#endif
