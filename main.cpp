/* -*- tab-width: 4 -*- */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "common.h"
#include "render.h"
#include "logic.h"
#include "network.h"

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <getopt.h>

#ifdef HAVE_FAM
#       include <fam.h>
#endif /* HAVE_FAM */


#define REF_FPS 30
#define REF_DT (1.0/REF_FPS)

pos_t pos_cat_prev(0,0);
pos_t pos_cat(0,0);
pos_t pos_cat_next(0,0);
pos_t pos_self(0,0);
float step = 0.0f;
int verbose_flag = 0;
FILE* verbose = NULL;
bool owner;
bool frist = true;
int port = PORT;

#ifdef HAVE_FAM
static FAMConnection _fam_connection;
FAMConnection* fam_connection(){
  return &_fam_connection;
}
#endif /* HAVE_FAM */

static void setup(int w, int h){
#ifdef HAVE_FAM
  if ( FAMOpen(&_fam_connection) < 0 ){
    fprintf(stderr, "Failed to open FAM connection\n");
  }
#endif /* HAVE_FAM */

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
  if ( v < 1 || v > max ){
    fprintf(stderr, "invalid grid value\n");
    exit(1);
  }
  return v - 1;
}

static void show_usage(){
  fprintf(stderr, "usage: wandercat [options] X Y\n");
  fprintf(stderr, "  where X is between 1 and %d\n", GRID_WIDTH);
  fprintf(stderr, "        Y is between 1 and %d\n", GRID_HEIGHT);
  fprintf(stderr, "\n");
  fprintf(stderr, "  -s, --spawn     Have this client spawn the cat.\n");
  fprintf(stderr, "  -p, --port=PORT Use PORT for communication (default: %d).\n", PORT);
  fprintf(stderr, "  -v, --verbose   Verbose output.\n");
  fprintf(stderr, "  -h, --help      This help text.\n");
}

int main(int argc, char* argv[]){
  static struct option long_options[] =
  {
		{"spawn",   no_argument,       0, 's' },
		{"port",    required_argument, 0, 'p' },
		{"help",    no_argument,       0, 'h'},
		{"verbose", no_argument, &verbose_flag, 1},
		{0, 0, 0, 0}
  };

  int option_index = 0;
  int c;

  while( (c=getopt_long(argc, argv, "sp:hv", long_options, &option_index)) != -1 ) {
	switch(c) {
		case 0:
			break;
		case 's':
			owner = true;
			break;
		case 'p':
			port = atoi(optarg);
			printf("Set port to %i\n", port);
			break;
		case 'h':
			show_usage();
			exit(0);
		case 'v':
			verbose_flag = 1;
			break;
		default:
			break;
	}
  }

  if ( argc - optind != 2 ){
	show_usage();
    exit(1);
  }

  srand((unsigned int)time(NULL));

  /* store position */
  pos_self.x = read_val(argv[optind++], GRID_WIDTH);
  pos_self.y = read_val(argv[optind++], GRID_HEIGHT);

  /* start cat? */
  if ( owner ){
    fprintf(stderr, "Spawning cat at %d %d\n", pos_self.x+1, pos_self.y+1);
    owner = true;
    pos_cat = pos_self;
	 pos_cat_prev = pos_cat;
	 ++pos_cat_prev.x;
  }

  /* verbose dst */
  if ( verbose_flag ){
	  verbose = stdout;
  } else {
	  verbose = fopen("/dev/null","w");
  }

  setup(800, 600);
  
  bool run = true;
  struct timeval ref;
  gettimeofday(&ref, NULL);

  while ( run ){
    struct timeval ts;
	gettimeofday(&ref, NULL);

    /* calculate dt */
    double dt = (ts.tv_sec - ref.tv_sec) * 1000000.0;
    dt += ts.tv_usec - ref.tv_usec;
    dt /= 1000000;

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
