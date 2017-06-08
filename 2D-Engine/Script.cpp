#include "Script.h"

#if USING_CPP
#include //
#elif USING_LUA
#include "LuaScript.h"
#endif


Script::Script()
{
}


Script::~Script()
{
}

void Script::CreateScriptContext(const char* code, GameObject * container)
{
	Script* script = nullptr;
#if USING_LUA
	script = new LuaScript
#endif

	if (script != nullptr) {
		if (script->InitScript(code, container)) {

		}
		else {
			delete script;
			script = nullptr;
		}
	}
}
