#include "render.h"
#include <GL/gl.h>
#include <SDL/SDL.h>

void render(double dt){
  glClear(GL_COLOR_BUFFER_BIT);

  SDL_GL_SwapBuffers();
}
