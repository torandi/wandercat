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
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* load textures */
  anim[ANIM_WAIVING] = load_anim("data/waving.png", 17, 25);
}

void render(double dt){
  glClear(GL_COLOR_BUFFER_BIT);

  float x;
  float y;
  float size;
  bool render = true;

  switch ( state ){
  case CAT_WAIVING:
    /* don't render the cat if it isn't in our position (cat is on a
     * grid-position where no client is connected, so we kept the cat) */
    if ( !(pos_self == pos_cat) ){
      render = false;
      break;
    }

    size = 512;
    x = center.x - size * 0.5f;
    y = center.y - size * 0.5f;
    break;

  case CAT_WALKING:
    {
      enum {
	ENTER,
	EXIT,
	OTHER,
      } direction = OTHER;

      /* determine direction */
      if ( pos_self == pos_cat ){
	direction = EXIT;
      } else if ( pos_self == pos_cat_next ){
	direction = ENTER;
      }

      /* don't render the cat if it isn't in our position (cat is on a
       * grid-position where no client is connected, so we kept the cat) */
      if ( direction == OTHER ){
	render = false;
	break;
      }

      /* only render first part if exiting the screen */
      if ( direction == EXIT && step > 0.5 ){
	render = false;
	break;
      }
    
      /* only render second part if enterin the screen */
      if ( direction == ENTER && step < 0.5 ){
	render = false;
	break;
      }

      size = 512;
      x = center.x - (size*0.5);
      y = center.y - (size*0.5);

      const int dx = pos_cat_next.x - pos_cat.x;
      const int dy = pos_cat_next.y - pos_cat.y;

      if ( direction == EXIT ){
	x += window.w * step * 2.0 * dx;
	y -= window.h * step * 2.0 * dy;
      } else {
	const float sinv = 1.0f - step;
	x -= window.w * sinv * 2.0 * dx;
	y += window.h * sinv * 2.0 * dy;
      }
    }
    break;

  default:
    fprintf(verbose, "nothing to render\n");
    fprintf(verbose, "step: %f\n", step);
    render = false;
  }

  animation_t& a = anim[ANIM_WAIVING];

  if ( render ){
    glColor4f(1,1,1,1);

    a.texture->bind();
    const unsigned int index = (int)(a.s * a.frames);
    Texture::texcoord_t tc = a.texture->index_to_texcoord(index);
    vertices[ 0] = tc.a[0];
    vertices[ 1] = tc.a[1];
    vertices[ 5] = tc.b[0];
    vertices[ 6] = tc.b[1];
    vertices[10] = tc.c[0];
    vertices[11] = tc.c[1];
    vertices[15] = tc.d[0];
    vertices[16] = tc.d[1];
    a.s = fmod(a.s + dt, a.delay * a.frames);
    printf("s: %f\n", a.s);

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

#ifdef VSYNC
  unsigned int retraceCount;
  glXGetVideoSyncSGI(&retraceCount);
  glXWaitVideoSyncSGI(2, (retraceCount+1)%2, &retraceCount);
#endif

  SDL_GL_SwapBuffers();
}
