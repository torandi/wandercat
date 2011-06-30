#include "common.h"
#include "render.h"
#include "logic.h"

#include <time.h>
#include <unistd.h>
#include <GL/gl.h>
#include <SDL/SDL.h>

#define REF_FPS 30
#define REF_DT (1.0/REF_FPS)

pos_t pos_cat;
pos_t pos_self;

static void setup(int w, int h){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_DOUBLEBUF);

  glClearColor(1,0,1,1);
}

static void cleanup(){
  SDL_Quit();
}

static void poll(bool* run){
  SDL_Event event;
  while ( SDL_PollEvent(&event) ){
    switch (event.type){
    case SDL_QUIT:
      *run = false;
      break;
    }
  }
}

int main(int argc, const char* argv[]){
  setup(800, 600);
  
  bool run = true;
  struct timespec ref;
  clock_gettime(CLOCK_REALTIME, &ref);

  while ( run ){
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    /* calculate dt */
    double dt = (ts.tv_sec - ref.tv_sec) * 1000000000.0;
    dt += ts.tv_nsec - ref.tv_nsec;
    dt /= 1000000000;

    /* do stuff */
    poll(&run);
    logic(dt);
    render(dt);
    
    /* framelimiter */
    const int delay = (REF_DT - dt) * 1000000;
    if ( delay > 0 ){
      usleep(delay);
    }

    /* store reference time */
    ref = ts;
  }

  cleanup();
}
