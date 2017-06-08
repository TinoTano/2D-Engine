#include "Engine.h"

Engine* engine = nullptr;

int main(int argc, char* args[])
{
	engine = new Engine();

	if (engine != nullptr) {
		if (engine->Awake()) {
			if (engine->Start()) {
				while (engine->DoUpdate()) {
				}
				if (engine->CleanUp()) {
					delete engine;
					engine = nullptr;
				}
			}
		}
	}

	if (engine != nullptr) {
		delete engine;
		engine = nullptr;
	}
	return 0;
}