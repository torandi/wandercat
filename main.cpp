#include <SDL/SDL.h>

static void setup(int w, int h){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_DOUBLEBUF);
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
  while ( run ){
    poll(&run);
  }

  cleanup();
}
