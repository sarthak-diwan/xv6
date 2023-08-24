#include "thread_spinlock.h"

// copied using acquire function from spinlock.c
struct thread_spinlock {
  uint locked;       // Is the lock held?

  // For debugging
  char *name;        // Name of lock.
};

