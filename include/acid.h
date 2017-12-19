#ifndef ACID_H
#define ACID_H
#include <stdbool.h>
#include <stddef.h>
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

void acid_setup();
void acid_teardown_nonblocking();
void acid_teardown();

#define _cleanup_acid_dissolve_ __attribute__((cleanup(acid_dissolve_cleanup)))
#define acid_set_field(type, field, acid_val)                               \
  object_set(_acid_get_base_object(type), #field,                            \
             ((acid_val) == NULL) ? NULL : _acid_get_base_object(acid_val)); \
  (type)->field = acid_val;

#define acid_set(acid_l, acid_r) \
  _acid_set_raw((void **)&(acid_l), acid_r);

void *acid_malloc(size_t alloc_size);
void *acid_malloc_dtor(size_t alloc_size, void(*dtor)(void*));
void acid_dissolve(void *acid_ptr);
void acid_dissolve_cleanup(void *acid_ptr);

void _acid_set_raw(void **var, void *acid_val);
bool acid_is_managed(void *ptr);
struct Object *_acid_get_base_object(void *acid_ptr);

#ifdef __cplusplus
}
#endif

#endif  // ACID_H
