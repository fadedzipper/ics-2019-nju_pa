#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  struct watchpoint *prev;

  /* TODO: Add more members if necessary */

  char expression[128];
  uint32_t old_val;

} WP;

WP *get_unallocated();
WP *new_wp();
void free_wp(int nr_to_delete);

#endif
