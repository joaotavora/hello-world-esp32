#include "frpp/frpp.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"

#include <iostream>
#include <stdexcept>
#include <sys/_stdint.h>

extern "C" void app_main(void) {
  static const auto BLINK_GPIO = GPIO_NUM_2;

  gpio_reset_pin(BLINK_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


  auto t = frpp::async([]{
    uint16_t s_led_state = 0;
    while (true) {
      gpio_set_level(BLINK_GPIO, s_led_state);
      s_led_state = !s_led_state;
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  });

  auto t2 = frpp::async([]{
    uint16_t s_led_state = 0;
    while (true) {
      gpio_set_level(BLINK_GPIO, s_led_state);
      s_led_state = !s_led_state;
      vTaskDelay(700 / portTICK_PERIOD_MS);
    }
  });

  for (int i = 3600; i >= 0; i--) {
    if (i%10 == 0)
      printf("Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart();
}
