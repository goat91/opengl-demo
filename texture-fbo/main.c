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

	/* Clear the whole area with white color */
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
    GLuint texture, fbo;
    uint8_t *pixels = NULL;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    printOpenGLError();


	/* Upload pixels data to texture from unpack buffer */
	glViewport(0, 0, 255, 255);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 255, 255, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/* Create FBO, and attach texture to color0 */
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    printOpenGLError();
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "framebuffer not completed\n");
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, 200, 200);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    draw_texture(texture);


	glDeleteFramebuffers(1, &fbo);
    glutSwapBuffers();
    glFinish();
}

void resize(int width, int height)
{
	printf("got resize event, width %d, height %d\n", width, height);
	glViewport(0, 0, width, height);
	printOpenGLError();
	render();
}


int main(int argc, char *argv[])
{
    int window;

    glutInit(&argc, argv);
    glutInitContextVersion(2, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(800, 500);

    window = glutCreateWindow("OpenGL test");

    //glutDisplayFunc(render);
    glutReshapeFunc(resize);

    glutMainLoop();
    return 0;
}
