/*                                                                  
 * POSIX Real Time Example
 * using a single pthread as RT thread
 */
 
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "rtpreempt_tools/realtime_check.h"
#include "rtpreempt_tools/realtime_thread_creation.h"
 
void *thread_function(void *data)
{

  rtpreempt_tools::Realtime_check rc(1000.0);

  int a = 0;
  for(int i=0;i<1000;i++){
    rc.tick();
    a++;
    usleep(2000); // microseconds, so 1Ghz
  }

  printf("\n");
  rtpreempt_tools::print_realtime_check(rc);
  printf("\n");

  return NULL;
}
 
int main(int argc, char* argv[]) {

  pthread_t thread;
  rtpreempt_tools::create_realtime_thread(thread,thread_function);
  pthread_join(thread, NULL);
  
}

