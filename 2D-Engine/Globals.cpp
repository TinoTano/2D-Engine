#include "Globals.h"
#include "Engine.h"
#include "ModuleEditor.h"

void Log(const char file[], const char function[], int line, bool isWarning, bool isError, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "%s\n At: %s => %s() => Line: %d.", tmp_string, file, function, line);
	
	if (isWarning) {
		engine->editorModule->PrintWarningLog(tmp_string2);
	}
	else if (isError) {
		engine->editorModule->PrintErrorLog(tmp_string2);
	}
	else {
		engine->editorModule->PrintLog(tmp_string2);
	}
}
