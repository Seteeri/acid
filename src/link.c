#include "link.h"
#include <assert.h>
#include <stdio.h>
#include "collector.h"
#include "locker.h"
#include "memory.h"
#include "object.h"
#include "task.h"

link_t *link_create(Object *s, Object *t) {
  link_t *link = xmalloc(sizeof(link_t), "link_create");
  *(Object **)&link->src = s;
  link->target = t;
  link->which = 0;
  link->phantomized = false;
  return link;
}

/**
 * Destroys a link_t. This frees the memory allocated to the link,
 * but does not effect the link's src or target.
 *
 * Generally, destroying a link only decrements the reference count.
 * @param link the link to destroy
 */
void link_destroy(link_t *link) {
  locker_start1(link->src);  // lock on the source object
  bool target_null = (link->target == NULL);
  locker_end();
  if (!target_null) {
    link_dec(link);
  }
  free(link);
}

// Pseudo: PhantomizeLink
void link_phantomize(link_t *link) {
  locker_start2(link->target, link->src);
  if (link->target == NULL || link->phantomized) {
    locker_end();
    return;
  }
  link->target->count[2]++;
  link_dec(link);
  assert(link->target->collector != NULL || link->src->collector != NULL);
  object_merge_collectors(link->src, link->target);
  link->phantomized = true;
  assert(collector_equals(
      link->target->collector,
      link->src
          ->collector));  // : "diff1: t="+target.collector+" s="+src.collector;
  locker_end();
}

// Pseudo: LinkFree
void link_dec(link_t *link) {
  if (link->target == NULL) return;

  bool deleted;
  locker_start2(link->src, link->target);
  if (link->phantomized) {
    deleted = object_dec_phantom(link->target);
  } else {
    deleted = object_dec(link->target, link->which);
  }
  if (deleted)
    link->target = NULL;
  locker_end();
}
