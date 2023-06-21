#include "greeter/greeter.h"

extern "C" void app_main(void) {
  greeter::greet_and_reboot();
}
