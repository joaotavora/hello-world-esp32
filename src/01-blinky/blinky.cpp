#include <cstdio>
#include "blinky/blinky.h"
#include "greeter/greeter.h"

void blinky::blinky(void)
{
  printf("Hello world BUT FROM BLINKY!\n");
  greeter::greet_and_reboot();
}
