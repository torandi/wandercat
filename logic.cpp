#include "logic.h"
#include "common.h"
#include "network.h"
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

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

static bool can_do_move(const pos_t &mov) {
    int x,y;
    x=pos_cat.x+mov.x;
    y=pos_cat.y+mov.y;

    if ( x < 0 || x >= GRID_WIDTH ){
	return false;
    }
    if ( y < 0 || y >= GRID_HEIGHT){
	return false;
    }
    return true;
}

void logic(struct timespec time, double dt){
    fprintf(verbose, "owner: %d\n", owner);
    switch ( state ){
    case CAT_FRIST:
	fprintf(verbose, "state: FRIST\n");
	step = 0.0f;
    
	if ( owner ){
	    set_state(CAT_WAIVING, time);
	}

	if ( pos_cat_next.x != 0 || pos_cat_next.y != 0 ){
	    set_state(CAT_WALKING, time);
	}
    
	break;

    case CAT_WALKING:
	/* The cat is currently moving from pos_cat to pos_cat_next */

	step = calc_step(&ref, &time, WALK_TIME);
	fprintf(verbose, "state: WALKING [%3d%%] src(%d,%d) -> dst(%d,%d)\n",
		(int)(step*100),
		pos_cat.x+1, pos_cat.y+1,
		pos_cat_next.x+1, pos_cat_next.y+1);
    
	if ( step < 1.0f ){
	    break;
	}

	pos_cat = pos_cat_next;
	set_state(CAT_WAIVING, time);
	break;

    case CAT_WAIVING:
	fprintf(verbose, "state: WAIVING [%3d%%] at (%d,%d)\n", 
		(int)(step*100),
		pos_cat.x+1, pos_cat.y+1);
	step = calc_step(&ref, &time, WAIVE_TIME);

	if ( step > 1.0f ){
	    if ( owner ){
		set_state(CAT_FROBNICATING, time);
	    } else {
		set_state(CAT_WALKING, time);
	    }
	}

	break;

    case CAT_FROBNICATING:
	fprintf(verbose, "state: FROBNICATING\n");
	if ( !owner ){
	    fprintf(stderr, "omg wtf? not owner but frobnicating\n");
	    exit(1);
	}

	{

	    /**
	     * Possible movements
	     */
	    pos_t m_back;
	    m_back.x = pos_cat_prev.x - pos_cat.x;
	    m_back.y = pos_cat_prev.y - pos_cat.y;
	
	    printf("(%d, %d) -> (%d, %d) => (%d, %d)\n", pos_cat.x+1,pos_cat.y+1, pos_cat_prev.x+1,pos_cat_prev.y+1,m_back.x,m_back.y);
	
	    std::vector<pos_t> frobnications;

	    pos_t mov;
	    for(int i=0;i<4;++i) {
		mov.x=0;
		mov.y=0;
		switch(i) {
		case 0:
		    mov.x = 1;
		    break;
		case 1:
		    mov.x = -1;
		    break;
		case 2:
		    mov.y = 1;
		    break;
		case 3:
		    mov.y = -1;
		}

		if(!can_do_move(mov))
		    continue;
		if(mov != m_back) { 
		    //Increase probability for this mov
		    frobnications.push_back(mov);
		    frobnications.push_back(mov);
		    frobnications.push_back(mov);
		} else {
		    frobnications.push_back(mov);
		}
	    }

	    //select move:
	    if(frobnications.size() > 0) {
		int frobb = rand() % frobnications.size();

		pos_cat_next.x = pos_cat.x + frobnications[frobb].x;
		pos_cat_next.y = pos_cat.y + frobnications[frobb].y;
		
		state = CAT_WALKING;

		printf("cat: (%i, %i)\n", pos_cat.x+1, pos_cat.y+1);
		send_cat();
	    } else {
		state = CAT_WAIVING;
	    }

	}

	break;

    case CAT_TEAPOT:
	fprintf(verbose, "state: TEAPOT\n");
	fprintf(stderr, "omg lol wtf? i'm a teapot?\n");
	exit(2);
    }
}
