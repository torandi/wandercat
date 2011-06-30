#ifndef COMMON_H
#define COMMON_H

#include <cstdio>
#include <stdint.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 10

#define WAIVE_TIME 10

typedef struct {
  uint16_t x;
  uint16_t y;
} pos_t;

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
