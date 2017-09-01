#pragma once

//#include <vld.h>

#define LOG(format, ...) Log(__FILE__, __func__,__LINE__, false, false, format, __VA_ARGS__)
#define LOG_WARNING(format, ...) Log(__FILE__, __func__,__LINE__, true, false, format, __VA_ARGS__)
#define LOG_ERROR(format, ...) Log(__FILE__, __func__, __LINE__, false, true, format, __VA_ARGS__)

void Log(const char file[], const char function[], int line, bool isWarning, bool isError, const char* format, ...);

typedef unsigned int uint;
typedef unsigned long long UID;

// Useful macros
#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

#define ASSETS_FOLDER "../Assets"
#define H_TEMPLATE_FILE_PATH "../Data/Templates/C++Script_h.txt"
#define CPP_TEMPLATE_FILE_PATH "../Data/Templates/C++Script_cpp.txt"
#define LUA_TEMPLATE_FILE_PATH "../Data/Templates/LuaScript.txt"