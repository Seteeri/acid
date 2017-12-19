#include <stdatomic.h>
#include "../lib/collectc/include/hashtable.h"
#include "../src/complock.h"
#include "../src/safelist.h"

#ifndef OBJECT_H
#define OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Keeps track of the state of the "which" bit. */
typedef atomic_uint_fast8_t bit_t;

typedef struct Object {
  const unsigned int id;
  complock_t *lock;
  HashTable *links;  // HashTable<String, Link>
  bit_t which;
  bool phantomized;
  int count[3];
  struct collector_t *collector;
  bool phantomization_complete;

  void (*dtor)(void *);
  void *data;
  uint64_t magic;  // magic number to determine if an object is GC-tracked
} Object;

Object *object_create();
Object *object_init(Object *o);
Object *object_create_strong();
Object *object_init_strong(Object *o);
Object *object_get(Object *obj, char *field);
void object_set(Object *obj, char *field, Object *referent);
void object_phantomize_node(Object *obj, struct collector_t *cptr);
void object_recover_node(Object *obj, struct collector_t *cptr);
void object_inc_strong(Object *obj);
void object_dec(Object *obj, bit_t w);
void object_dec_phantom(Object *obj);
void object_dec_strong(Object *obj);
void object_clean_node(Object *obj);
bool object_merge_collectors(Object *obj, Object *target);
void object_set_collector(Object *obj, struct collector_t *c);

#ifdef __cplusplus
}
#endif

#endif  // OBJECT_H
