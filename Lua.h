#ifndef LUA_H_
#define LUA_H_

#include "SoraIncludes.h"
#include "EntityManager.h"
#include "Define.h"
#include "LuaContext.h"

class CTOFNLua : public CLuaObject
{

protected:

    void CreateLuaHooks();

public:

    std::string GetEngineString( const char * );
    void CallEngineFunction( const char * );


};

void SetLuaContext( CLuaContext * );

#endif
