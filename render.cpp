#include "render.h"
#include "common.h"
#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "texture.h"
#include <math.h>

static const float XSCALE = 2.5;

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
} animation[ANIM_MAX] = {{0}};

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
  gluLookAt(pos_self.x * XSCALE, pos_self.y, 2,
  	    pos_self.x * XSCALE, pos_self.y, 0,
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
  animation[ANIM_WAIVING] = load_anim("data/waving.png", 17, 25);
  animation[ANIM_WALKING_WEST] = load_anim("data/walk_left.png", 10, 20);
  animation[ANIM_WALKING_EAST] = load_anim("data/walk_right.png", 10, 20);
}

static void render_cat(animation_t* anim, float x, float z, const double dt){
  glColor4f(1,1,1,1);

  if ( anim ){
    anim->texture->bind();
    const unsigned int index = (int)(anim->s * anim->frames);
    Texture::texcoord_t tc = anim->texture->index_to_texcoord(index);
    vertices[ 0] = tc.a[0];
    vertices[ 1] = 1-tc.a[1];
    vertices[ 5] = tc.b[0];
    vertices[ 6] = 1-tc.b[1];
    vertices[10] = tc.c[0];
    vertices[11] = 1-tc.c[1];
    vertices[15] = tc.d[0];
    vertices[16] = 1-tc.d[1];
    anim->s = fmod(anim->s + dt, anim->delay * anim->frames);
  }

  /* render cat */
  glPushMatrix();
  {
    //printf("%f\n", x);
    glTranslatef(x * XSCALE, 0.0, z);
    //glScalef(size, size, 1.0);
    glInterleavedArrays(GL_T2F_V3F, sizeof(float)*5, vertices);
    glDrawArrays(GL_QUADS, 0, num_vertices);
  }
  glPopMatrix();

  if ( anim ){
    anim->texture->unbind();
  }
}

void render(double dt){
  glClear(GL_COLOR_BUFFER_BIT);

  float x = pos_cat.x;
  float z = pos_cat.y;
  bool render = true;

  animation_t* anim = NULL;

  switch ( state ){
  case CAT_WAIVING:
    //case CAT_IDLE:
    anim = &animation[ANIM_WAIVING];
    break;

  case CAT_WALKING:
    {
      const pos_t delta = pos_cat_next - pos_cat;

      x = (float)delta.x * step + pos_cat.x;
      z = (float)delta.y * step + pos_cat.y;

      /* left or right? */
      if ( delta.x > 0 ){
	anim = &animation[ANIM_WALKING_EAST];
      } else if ( delta.x < 0 ){
	anim = &animation[ANIM_WALKING_WEST];
      }
    }
    break;

  case CAT_FROBNICATING:
    render = false;
    break;

  default:
    printf("default\n");
    break;
  }

  if ( render ){
    render_cat(anim, x, z, dt);
  }

#ifdef VSYNC
  unsigned int retraceCount;
  glXGetVideoSyncSGI(&retraceCount);
  glXWaitVideoSyncSGI(2, (retraceCount+1)%2, &retraceCount);
#endif

  SDL_GL_SwapBuffers();
}
