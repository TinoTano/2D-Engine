#pragma once

#define LOG(format, ...) _log(__FILE__, __LINE__, format, __VA_ARGS__)

void _log(const char file[], int line, const char* format, ...);

typedef unsigned int uint;

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

#define ASSETS_FOLDER "/Assets/"

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

// Disable STL exceptions
#ifndef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#endif
#define _STATIC_CPPLIB