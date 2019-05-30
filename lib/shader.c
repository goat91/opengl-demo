#include "shader.h"
#include <gio/gio.h>

GLuint load_shader_from_string(GLenum shader_type, const char *source)
{
    GLuint shader;
    GLint status;

    shader = glCreateShader(shader_type);
    g_assert(shader != 0);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint len;
        char *err_buf = NULL;


        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        err_buf = g_malloc0(len);
        glGetShaderInfoLog(shader, len, NULL, err_buf);
        g_printerr("Failed to compile shader: %s\n", err_buf);

        g_free(err_buf);
        glDeleteShader(shader);
		return 0;
    }

	return shader;
}

GLuint load_shader_from_file(GLenum shader_type, const char *file_path)
{
    GLuint shader;
    GFile *file;
    GError *error = NULL;
    gchar *buffer;
    size_t buf_len;
    GLint status;

    g_assert(file_path != NULL);
    g_assert(shader_type == GL_VERTEX_SHADER || shader_type == GL_FRAGMENT_SHADER);

    file = g_file_new_for_path(file_path);
    if (!g_file_load_contents(file, NULL, &buffer, &buf_len, NULL, &error)) {
        g_printerr("Failed to read file '%s': %s\n", file_path, error->message);
        g_error_free(error);
        g_object_unref(file);
        return 0;
    }
	
	shader = load_shader_from_string(shader_type, buffer);

    g_free(buffer);
    g_object_unref(file);
    return shader;
}

GLuint create_shader_program(const char* vertex_shader_file, const char* fragment_shader_file)
{
    GLuint shader_program;
    GLuint vertex_shader, fragment_shader;
    int status;

	vertex_shader = load_shader_from_file(GL_VERTEX_SHADER, vertex_shader_file);
	fragment_shader = load_shader_from_file(GL_FRAGMENT_SHADER, fragment_shader_file);

	if (!vertex_shader || !fragment_shader) {
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return 0;
	}
    
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        int log_len;
        char *buffer;

        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_len);
        buffer = g_malloc0(log_len);

        glGetProgramInfoLog(shader_program, log_len, NULL, buffer);
        g_printerr("Failed to link program: '%s'\n", buffer);
        g_free(buffer);

        glDetachShader(shader_program, vertex_shader);
        glDetachShader(shader_program, fragment_shader);
        glDeleteProgram(shader_program);
        shader_program = 0;
    }

    return shader_program;
}
