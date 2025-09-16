#include "Macros.h"

#include <iostream>


void GlClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char *function, const char *file, int line) {
    while (GLenum err = glGetError()) {
        std::cout << "[OpenGL Error] (" << err << "):" << function << " " << file
            << ":" << line << std::endl;
        return false;
    }

    return true;
}

