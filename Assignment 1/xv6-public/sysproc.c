#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_draw(void* buff,int size){
  char* wolf = "     .--..--..--..--..--..--.\n.\' \\  (`._   (_)     _   \\\n.\'    |  \'._)         (_)  |\n\\ _.\')\\      .----..---.   /\n|(_.\'  |    /    .-\\-.  \\  |\n\\     0|    |   ( O| O) | o|\n|  _  |  .--.____.\'._.-.  |\n\\ (_) | o         -` .-`  |\n|    \\   |`-._ _ _ _ _\\ /\n\\    |   |  `. |_||_|   |\n| o  |    \\_      \\     |     -.   .-.\n|.-.  \\     `--..-'   O |     `.`-\' .\'\n_.\'  .\' |     `-.-\'      /-.__   \' .-\'\n.\' `-.` \'.|=\'=.=\'=.=\'=.=\'=|._/_ `-\'.\'\n`-._  `.  |________/\\_____|    `-.\'\n.\'   ).| \'=\' \'=\'\\/ \'=\' |\n`._.`  \'---------------\'\n//___\\   //___\\\n||       ||\nLGB      ||_.-.   ||_.-.\n(_.--__) (_.--__)\n";
  char* buf = (char*)buff;
  argstr(0,&buf);
  argint(1,&size);
  
  int n = strlen(wolf);
  if(n > size)
  return -1;
  for(int i=0;i<n;++i)
  buf[i] = wolf[i];
  return n;
}

int
sys_thread_create(void){
  int fcn;
  char* arg;
  char* stack;
  if(argint(0, &fcn) < 0) return -1;
  if(argint(1, (int*)&arg) < 0) return -1; //changed
  if(argint(2, (int*)&stack)<0) return -1; //changed
  
  return thread_create((void(*)(void*))fcn, arg, stack);
}


int sys_thread_join(void)
{
  int ret_val = thread_join();
  return ret_val;
}

int sys_thread_exit(void){
  thread_exit();
  return 0;
}
