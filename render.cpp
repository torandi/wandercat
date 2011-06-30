#include "render.h"
#include "common.h"
#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "texture.h"
#include <math.h>

static const unsigned int num_vertices = 4;
static float vertices[2*num_vertices + 3*num_vertices] = {
  0, 1,   -0.5f, -0.5f, 0.0f,
  0, 0,   -0.5f,  0.5f, 0.0f,
  1, 0,    0.5f,  0.5f, 0.0f,
  1, 1,    0.5f, -0.5f, 0.0f
};
static struct {
  float x;
  float y;
} center;
static struct {
  float w;
  float h;
} window;

enum {
  ANIM_WALKING_EAST,
  ANIM_WALKING_WEST,
  ANIM_WALKING_NORTH,
  ANIM_WALKING_SOUTH,
  ANIM_WAIVING,
 
  ANIM_MAX
};

static struct animation_t {
  Texture* texture;
  unsigned int frames;
  unsigned int current;
  float delay;
  float s;
} anim[ANIM_MAX] = {{0}};

animation_t load_anim(const char* filename, unsigned int frames, unsigned int fps){
  animation_t tmp;
  tmp.texture = new Texture(filename, frames);
  tmp.frames = frames;
  tmp.current = 0;
  tmp.delay = 1.0f / fps;
  tmp.s = 0.0f;
  return tmp;
}

void render_init(int w, int h){
  /* create window */
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_DOUBLEBUF);
  glewInit();

  glMatrixMode(GL_PROJECTION);
  gluPerspective(45, ((float)w)/((float)h), 0.001, 100);

  /* orthographic projection */
  //glOrtho(0, w, 0, h, -1.0, 1.0);
  //glScalef(1, -1, 1);
  //glTranslated(0, -h, 0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(pos_self.x * 3, pos_self.y, 2,
  	    pos_self.x * 3, pos_self.y, 0,
  	    0, 1, 0);
  glViewport(0, 0, w, h);

  center.x = (float)w / 2;
  center.y = (float)h / 2;
  window.w = w;
  window.h = h;

  /* setup opengl */
  glClearColor(1,0,1,1);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* load textures */
  anim[ANIM_WAIVING] = load_anim("data/waving.png", 17, 25);
  anim[ANIM_WALKING_WEST] = load_anim("data/walk_left.png", 10, 20);
  anim[ANIM_WALKING_EAST] = load_anim("data/walk_right.png", 10, 20);
}

void render(double dt){
  glClear(GL_COLOR_BUFFER_BIT);

  float x = pos_cat.x;
  float z = pos_cat.y* 2;

  animation_t* a = NULL;

  switch ( state ){
  case CAT_WAIVING:
    a = &anim[ANIM_WAIVING];
    break;

  case CAT_WALKING:
    {
      const pos_t delta = pos_cat_next - pos_cat;

      x = (float)delta.x * step + pos_cat.x;
      z = (float)delta.y * step + pos_cat.y;

      /* left or right? */
      if ( delta.x > 0 ){
	a = &anim[ANIM_WALKING_EAST];
      } else if ( delta.x < 0 ){
	a = &anim[ANIM_WALKING_WEST];
      }
    }
    break;

  default:
    fprintf(verbose, "nothing to render\n");
    //fprintf(verbose, "step: %f\n", step);
    //render = false;
    break;
  }

  glColor4f(1,1,1,1);

  if ( a ){
    a->texture->bind();
    const unsigned int index = (int)(a->s * a->frames);
    Texture::texcoord_t tc = a->texture->index_to_texcoord(index);
    vertices[ 0] = tc.a[0];
    vertices[ 1] = 1-tc.a[1];
    vertices[ 5] = tc.b[0];
    vertices[ 6] = 1-tc.b[1];
    vertices[10] = tc.c[0];
    vertices[11] = 1-tc.c[1];
    vertices[15] = tc.d[0];
    vertices[16] = 1-tc.d[1];
    a->s = fmod(a->s + dt, a->delay * a->frames);
  }

  /* render cat */
  glPushMatrix();
  {
    //printf("%f\n", x);
    glTranslatef(x * 3, 0.0, z);
    //glScalef(size, size, 1.0);
    glInterleavedArrays(GL_T2F_V3F, sizeof(float)*5, vertices);
    glDrawArrays(GL_QUADS, 0, num_vertices);
  }
  glPopMatrix();

  if ( a ){
    a->texture->unbind();
  }

#ifdef VSYNC
  unsigned int retraceCount;
  glXGetVideoSyncSGI(&retraceCount);
  glXWaitVideoSyncSGI(2, (retraceCount+1)%2, &retraceCount);
#endif

  SDL_GL_SwapBuffers();
}
