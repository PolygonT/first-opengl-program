#pragma once

#include <GL/glew.h>

#ifdef WIN32
   // code for Windows systems
#define ASSERT(x) if(!(x)) __debugbreak();
#else
   // code for other systems
#define ASSERT(x) if(!(x)) asm("int3");

#endif

#define GLCall(x) GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();

bool GlLogCall(const char *function, const char *file, int line);
