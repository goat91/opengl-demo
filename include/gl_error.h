#ifndef GL_ERROR_INCLUDE_H
#define GL_ERROR_INCLUDE_H

#include <epoxy/gl.h>
#include <glib.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)

static int printOglError(char *file, int line)
{

    GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        g_printerr("glError in file %s @ line %d: errno 0x%x\n",
               file, line, glErr);
        retCode = 1;
    }
    return retCode;
}


#endif