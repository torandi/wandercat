#include "logic.h"
#include "common.h"
#include <time.h>
#include <cstdio>
#include <cstdlib>

state_t state = CAT_FRIST;
static struct timespec ref;

static void set_state(enum state_t n, struct timespec time){
  fprintf(verbose, "changing state to %d\n", n);
  ref = time;
  state = n;
  step = 0.0f;
}

static double calc_step(struct timespec* ref, struct timespec* time, double max){
  double dt = (time->tv_sec - ref->tv_sec) * 1000000000.0;
  dt += time->tv_nsec - ref->tv_nsec;
  dt /= 1000000000;
  return dt / max;
}

void logic(struct timespec time, double dt){
  switch ( state ){
  case CAT_FRIST:
    fprintf(verbose, "state: FRIST\n");
    step = 0.0f;
    
    if ( owner ){
      set_state(CAT_WAIVING, time);
    } else {
      set_state(CAT_IDLE, time);
    }
    
    break;

  case CAT_WALKING:
    fprintf(verbose, "state: WALKING\n");
    step = calc_step(&ref, &time, WALK_TIME);
    if ( step > 1.0f ){
      set_state(CAT_IDLE, time);
    }
    break;

  case CAT_WAIVING:
    fprintf(verbose, "state: WAIVING\n");
    step = calc_step(&ref, &time, WAIVE_TIME);

    if ( step > 1.0f ){
      if ( owner ){
	set_state(CAT_FROBNICATING, time);
      } else {
	set_state(CAT_IDLE, time);
      }
    }

    break;

  case CAT_IDLE:
    fprintf(verbose, "state: IDLE\n");
    break;

  case CAT_FROBNICATING:
    fprintf(verbose, "state: FROBNICATING\n");
    if ( !owner ){
      fprintf(stderr, "omg wtf? not owner but frobnicating\n");
      exit(1);
    }

    signed int x;
    signed int y;

    do {
      x = pos_cat.x;
      y = pos_cat.y;

      const int dir = rand() % 4;
      switch ( dir ){
      case 0:
	x += 1;
	break;
      case 1:
	x -= 1;
	break;
      case 2:
	y += 1;
	break;
      case 3:
	y -= 1;
	break;
      }
      if ( x < 0 || x >= GRID_WIDTH ){
	continue;
      }
      if ( y < 0 || y >= GRID_WIDTH ){
	continue;
      }
    } while ( 0 );

    pos_cat_next.x = x;
    pos_cat_next.y = y;

    state = CAT_WALKING;
    break;

  case CAT_TEAPOT:
    fprintf(verbose, "state: TEAPOT\n");
    fprintf(stderr, "omg lol wtf? i'm a teapot?\n");
    exit(2);
  }
}
