#include "sleep.h"

void my_sleep(int secs) {
  #define STEPS_PER_SEC 16000000 //650 000 000
  unsigned int i,s;
  for (s=0; s < secs; s++) {
    for (i=0; i < STEPS_PER_SEC; i++) {
       // skip CPU cycle or any other statement(s) for making loop
       // untouched by C compiler code optimizations
       __asm volatile("nop");
    }
  }
}

