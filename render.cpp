#include "render.h"
#include <GL/gl.h>
#include <SDL/SDL.h>

static const unsigned int num_vertices = 4;
static const float vertices[2*num_vertices + 3*num_vertices] = {
  0, 1,   0.0f, 0.0f, 0.0f,
  0, 0,   0.0f, 1.0f, 0.0f,
  1, 0,   1.0f, 1.0f, 0.0f,
  1, 1,   1.0f, 0.0f, 0.0f
};
static struct {
  float x;
  float y;
} center;

void render_init(int w, int h){
  /* create window */
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_DOUBLEBUF);

  /* orthographic projection */
  glOrtho(0, w, 0, h, -1.0, 1.0);
  glScalef(1, -1, 1);
  glTranslated(0, -h, 0);
  glViewport(0, 0, w, h);

  center.x = (float)w / 2;
  center.y = (float)h / 2;

  /* setup opengl */
  glClearColor(1,0,1,1);
}

void render(double dt){
  glClear(GL_COLOR_BUFFER_BIT);

  glColor4f(1,1,1,1);

  glPushMatrix();
  {
    glTranslatef(center.x - 256, center.y - 256, 0.0f);
    glScalef(512, 512, 1.0);
    glInterleavedArrays(GL_T2F_V3F, sizeof(float)*5, vertices);
    glDrawArrays(GL_QUADS, 0, num_vertices);
  }
  glPopMatrix();

  SDL_GL_SwapBuffers();
}
