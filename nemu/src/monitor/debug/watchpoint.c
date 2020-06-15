#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <string.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP allocated = {.NO = -1, .next = NULL, .prev = NULL};
static WP unallocated = {.NO = -1, .next = NULL, .prev = NULL};
static WP *head = NULL, *free = NULL;

void insert_before_wp(WP *next, WP *wp){
	WP *prev = next->prev;
	wp->next = next;
	wp->prev = prev;
	prev->next = wp;
	next->prev = wp;
}

void insert_wp_pool(WP *type, WP *wp){
	insert_before_wp(type, wp);
}

WP *remove_wp(WP *wp){
	WP *prev = wp->prev;
	WP *next = wp->next;
	prev->next = next;
	next->prev = prev;
	wp->prev = wp->next = NULL;
	return wp;
}

void init_wp_pool() {
    int i;
    for (i = 0; i < NR_WP; i ++) {
      wp_pool[i].NO = i;
      memset(wp_pool[i].expression, 0, sizeof(wp_pool[0].expression));
      wp_pool[i].old_val = 0;
	  wp_pool[i].next = NULL;
	  wp_pool[i].prev = NULL;
    }

  	head = &allocated;
	free = &unallocated;
	head->next = head->prev = head;
	free->next = free->prev = free;

	for(i = 0; i <NR_WP; i++){
		insert_before_wp(free, &wp_pool[i]);
	}

}

/* TODO: Implement the functionality of watchpoint */
WP *new_wp(){
	assert(free->next != free);
	WP *temp = remove_wp(free->next);
	insert_wp_pool(head, temp);
	return temp;
}

void free_wp(int nr_to_delete){
	assert(nr_to_delete >= 0 && nr_to_delete <= NR_WP);
	WP *temp = remove_wp(&wp_pool[nr_to_delete]);
	temp->old_val = 0;
	memset(temp->expression, 0, sizeof(temp->expression));
	insert_wp_pool(free, temp);
}

WP *get_unallocated(){
	return head;
}
