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
    
    if ( pos_cat.x == pos_self.x && pos_cat.y == pos_self.y ){
      set_state(CAT_WAIVING, time);
    } else {
      set_state(CAT_WAIVING, time);
    }
    
    break;

  case CAT_WALKING:
    fprintf(verbose, "state: WALKING\n");
    //if ( step > 0.3f && step < 0.7 ){
    //  state = CAT_WAIVING;
    //}
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
    state = CAT_WALKING;
    break;

  case CAT_TEAPOT:
    fprintf(verbose, "state: TEAPOT\n");
    fprintf(stderr, "omg lol wtf? i'm a teapot?\n");
    exit(2);
  }
}
