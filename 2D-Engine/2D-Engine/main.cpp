#include "Engine.h"

Engine* engine = nullptr;

int main(int argc, char* args[])
{
	engine = new Engine();

	if (engine != nullptr) {
		LOG("========== Awake phase ==========");
		if (engine->Awake()) {
			LOG("========== Start phase ==========");
			if (engine->Start()) {
				LOG("========== Update phase ==========");
				while (engine->DoUpdate()) {
				}
				LOG("========== CleanUp phase ==========");
				if (engine->CleanUp()) {
					delete engine;
					engine = nullptr;
				}
				else {
					LOG("Error doing CleanUp");
				}
			}
			else {
				LOG("Error doing Start");
			}
		}
		else {
			LOG("Error doing Awake");
		}
	}

	if (engine != nullptr) {
		delete engine;
		engine = nullptr;
	}
	return 0;
}