#include "common.h"
#include "render.h"
#include "logic.h"
#include "network.h"

#include <time.h>
#include <unistd.h>
#include <SDL/SDL.h>

#define REF_FPS 30
#define REF_DT (1.0/REF_FPS)

pos_t pos_cat_prev = {0,0};
pos_t pos_cat = {0,0};
pos_t pos_cat_next = {0,0};
pos_t pos_self = {0,0};
float step = 0.0f;
FILE* verbose = NULL;
bool owner;

static void setup(int w, int h){
  render_init(w, h);
  init_network();
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

static uint16_t read_val(const char* str, int max){
  const int v = atoi(str);
  if ( v < 0 || v > max ){
    fprintf(stderr, "invalid grid value\n");
    exit(1);
  }
  return v - 1;
}

int main(int argc, const char* argv[]){
  if ( argc < 3 ){
    fprintf(stderr, "usage: wandercat X Y\n");
    fprintf(stderr, "  where X is between 1 and %d\n", GRID_WIDTH);
    fprintf(stderr, "        Y is between 1 and %d\n", GRID_HEIGHT);
    exit(1);
  }

  srand((unsigned int)time(NULL));

  /* store position */
  pos_self.x = read_val(argv[1], GRID_WIDTH);
  pos_self.y = read_val(argv[2], GRID_HEIGHT);

  /* start cat? */
  owner = argc >= 4;
  if ( owner ){
    fprintf(stderr, "Spawning cat at %d %d\n", pos_self.x+1, pos_self.y+1);
    pos_cat = pos_self;
  }

  /* verbose dst */
#ifdef EXT_VERBOSE
  verbose = stdout;
#else
  verbose = fopen("/dev/null","w");
#endif

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
	 network();
    logic(ts, dt);
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
