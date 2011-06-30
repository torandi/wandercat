#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 10

struct pos_t {
  uint16_t x;
  uint16_t y;

  bool operator==(const pos_t &other) {
		return (x == other.x && y == other.y);
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

#endif /* COMMON_H */
