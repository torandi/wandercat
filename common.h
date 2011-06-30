#ifndef COMMON_H
#define COMMON_H

#include <cstdio>
#include <stdint.h>

#define GRID_WIDTH 4
#define GRID_HEIGHT 1

#define WALK_TIME 3
#define WAIVE_TIME 1

struct pos_t {
  int16_t x;
  int16_t y;

  pos_t()
  : x(0)
  , y(0){

  }

  pos_t(int16_t x, int16_t y)
  : x(x)
  , y(y){

  }

  bool operator==(const pos_t &other) const {
		return (x == other.x && y == other.y);
  }

  bool operator!=(const pos_t &other) const {
		return !(*this == other);
  }

  pos_t operator-(const pos_t& rhs) const {
    return pos_t(x - rhs.x, y - rhs.y);
  }
};

/**
 * Current position of the cat.
 */
extern pos_t pos_cat;

/**
 * Last position of cat.
 */
extern pos_t pos_cat_prev;

/**
 * Next position of cat.
 */
extern pos_t pos_cat_next;

/**
 * This clients position.
 */
extern pos_t pos_self;

/**
 * Is owner of cat
 */
extern bool owner;

/**
 * Current animation step.
 * 0.0 -> 1.0
 */
extern float step;

enum state_t {
  CAT_FRIST,
  CAT_WALKING,
  CAT_WAIVING,
  CAT_IDLE,
  CAT_FROBNICATING,
  CAT_TEAPOT,
};

extern state_t state;

extern FILE* verbose;

#endif /* COMMON_H */
