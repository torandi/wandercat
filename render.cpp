#include "render.h"
#include "common.h"
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
static struct {
  float w;
  float h;
} window;

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
  window.w = w;
  window.h = h;

  /* setup opengl */
  glClearColor(1,0,1,1);
}

void render(double dt){
  glClear(GL_COLOR_BUFFER_BIT);

  float x;
  float y;
  float size;
  bool render = true;

  switch ( state ){
  case CAT_WAIVING:
    size = 512;
    x = center.x - size * 0.5f;
    y = center.y - size * 0.5f;
    break;

  case CAT_WALKING:
    if ( step > 0.5 ){
      render = false;
      break;
    }
    size = 512;
    {
      int dx = pos_cat_next.x - pos_cat.x;
      int dy = pos_cat_next.y - pos_cat.y;
      x = center.x - (size * 0.5) + (window.w * step * 2.0 * dx); 
      y = center.y - (size * 0.5) + (window.h * step * 2.0 * dy); 
    }
    break;

  default:
    fprintf(verbose, "nothing to render\n");
    fprintf(verbose, "step: %f\n", step);
    render = false;
    return;
  }

  if ( render ){
    glColor4f(1,1,1,1);
    
    /* render cat */
    glPushMatrix();
    {
      glTranslatef(x, y, 0.0f);
      glScalef(size, size, 1.0);
      glInterleavedArrays(GL_T2F_V3F, sizeof(float)*5, vertices);
      glDrawArrays(GL_QUADS, 0, num_vertices);
    }
    glPopMatrix();
  }

  SDL_GL_SwapBuffers();
}
