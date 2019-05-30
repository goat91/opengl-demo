#include <stdio.h>
#include <epoxy/gl.h>
#include <GL/freeglut.h>
#include "shader.h"
#include "gl_error.h"
#include "image.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

static GLfloat verticles[] = {
    -1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, -1.0f,
    -1.0f, -1.0f
};

static GLfloat texcoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
};

static GLushort indices[] = {
    0, 1, 2,
    0, 2, 3
};

void draw_texture(GLuint texture)
{
    GLuint vao, vbo, ebo, loc;
    GLuint program;

	/* Clear the whole area with black color */
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 8, verticles);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, sizeof(GLfloat) * 8, texcoords);
    printOpenGLError();

	/* Generate elements buffer */
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * 6, indices, GL_STATIC_DRAW);
    printOpenGLError();

    program = create_shader_program("vertex.glsl", "fragment.glsl");
    glUseProgram(program);
    printOpenGLError();

	/**
	 * Enable vertex attrs and specify attr data location
	 */
    loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    printOpenGLError();

    loc = glGetAttribLocation(program, "qt_MultiTexCoord0");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat) * 8));
    printOpenGLError();

    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);

    glutSwapBuffers();
    glFinish();
}

void render(void)
{
    GLuint texture, fbo, pbo;
    int width, height, channels, stride;
    uint8_t *pixels = NULL, *pbo_buf;

	/**
	 * Create Pixel unpack buffer and alloc 256 x 256 pixels with RGBA color
	 */
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, 256 * 256 * 4, NULL, GL_STATIC_READ);

	/* Map to application memory space with rw permission */
    pbo_buf = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE);
    printOpenGLError();

	/* Load image data into unpacked buffer */
    read_image_pixels(pbo_buf, "./dog.jpeg", &width, &height, &channels, &stride);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

	/* Set pixels layout for unpacked buffer, byte alignment for each row, row length may not equal to image width, since row padding may exist */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, stride/channels);
    printOpenGLError();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    printOpenGLError();

	/* Upload pixels data to texture from unpack buffer */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/* Restore default, or the following steps may be affected */
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    draw_texture(texture);

    glutSwapBuffers();
    glFinish();
    glDeleteBuffers(1, &pbo);
}


int main(int argc, char *argv[])
{
    int window;

    glutInit(&argc, argv);
    glutInitContextVersion(2, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(800, 500);

    window = glutCreateWindow("OpenGL test");

    glutDisplayFunc(render);

    glutMainLoop();
    return 0;
}
