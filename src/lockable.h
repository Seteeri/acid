#ifndef LOCKABLE_H
#define LOCKABLE_H

typedef struct lockable_t {
  struct prioritylock_t lock;
} lockable_t;

#endif  // LOCKABLE_H
