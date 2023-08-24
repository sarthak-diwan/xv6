#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"

// tried to include all the different structs for mutexes and spinlocks into 
// different files but importing strucure of c created issue in compiling will need to 
// study the importing structure of c in detail
struct balance {
    char name[32];
    int amount;
};

volatile int total_balance = 0;
struct thread_spinlock lock;
// copied spinlock
struct thread_spinlock {
  uint locked;       // Is the lock held?
};
void thread_spin_init(struct thread_spinlock *lk) {
  lk->locked = 0;
}

void thread_spin_lock(struct thread_spinlock* lk) {

    // The xchg is atomic.
  while(xchg(&lk->locked, 1) != 0)
    ;
  __sync_synchronize();
}


void thread_spin_unlock(struct thread_spinlock* lk) {

  __sync_synchronize();

  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

struct thread_mutex mutex_lock;

struct thread_mutex{
  uint locked;
};

void thread_mutex_init(struct thread_mutex* lk) {
   lk->locked = 0;
}

void thread_mutex_lock(struct thread_mutex* lk) {
  while(xchg(&lk->locked, 1) != 0)
    sleep(1);
  // sleep ensures that the current process does not hog the processor it is working on and allows
  // context switching on the processor preventing the deadlock of a processor 
  __sync_synchronize();
}

void thread_mutex_unlock(struct thread_mutex* lk) {
  __sync_synchronize();

  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

volatile unsigned int delay (unsigned int d) {
   unsigned int i;
   for (i = 0; i < d; i++) {
       __asm volatile( "nop" ::: );
   }

   return i;
}

void do_work(void *arg){
    int i;
    int old;

    struct balance *b = (struct balance*) arg;
    printf(1, "Starting do_work: s:%s\n", b->name);

    for (i = 0; i < b->amount; i++) {
        //  thread_spin_lock(&lock);
         thread_mutex_lock(&mutex_lock);
         old = total_balance;
         delay(100000);
         total_balance = old + 1;
        //  thread_spin_unlock(&lock);
         thread_mutex_unlock(&mutex_lock);
    }

    printf(1, "Done s:%s\n", b->name);

    thread_exit();
    return;
}

int main(int argc, char *argv[]) {

  struct balance b1 = {"b1", 3200};
  struct balance b2 = {"b2", 2800};

  void *s1, *s2;
  int t1, t2, r1, r2;

  s1 = malloc(4096);
  s2 = malloc(4096);

  // Initialize thread_spin_lock
  // thread_spin_init(&lock);
  thread_mutex_init(&mutex_lock);

  t1 = thread_create(do_work, (void*)&b1, s1);
  t2 = thread_create(do_work, (void*)&b2, s2);

  r1 = thread_join();
  r2 = thread_join();

  printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n",
      t1, r1, t2, r2, total_balance);

  exit();
}
