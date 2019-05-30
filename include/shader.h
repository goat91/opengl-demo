#ifndef SHADER_LOADER_INCLUDE_H
#define SHADER_LOADER_INCLUDE_H

#include <epoxy/gl.h>

GLuint load_shader_from_string(GLenum shader_type, const char *source);
GLuint load_shader_from_file(GLenum shader_type, const char *file);
GLuint create_shader_program(const char* vertex_shader_file, const char* fragment_shader_file);

#endif
