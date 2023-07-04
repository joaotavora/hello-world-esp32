#include "frpp/frpp.h"
#include <chrono>
#include <cstddef>
#include <string>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"

using namespace std::chrono_literals;


// See https://github.com/espressif/esp-idf/issues/9692 for how I got this snippet
esp_err_t example_configure_stdin_stdout(void)
{
    static bool configured = false;
    if (configured) {
      return ESP_OK;
    }
    // Initialize VFS & UART so we can use std::cout/cin
    setvbuf(stdin, NULL, _IONBF, 0);
    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install( (uart_port_t)CONFIG_ESP_CONSOLE_UART_NUM,
            256, 0, 0, NULL, 0) );
    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);
    configured = true;
    return ESP_OK;
}

extern "C" void app_main(void) {
  static const auto BLINK_GPIO = GPIO_NUM_2;

  example_configure_stdin_stdout();
  gpio_reset_pin(BLINK_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  std::chrono::milliseconds blink_period=1s;

  auto t1 = frpp::async([&]{
    uint16_t s_led_state = 0;
    while (true) {
      gpio_set_level(BLINK_GPIO, s_led_state);
      s_led_state = !s_led_state;
      frpp::sleep_for(blink_period/2);
    }
  });


  std::size_t period;

  printf("Period: ");

  while (fscanf(stdin, "%ud", &period)) {
    blink_period = std::chrono::milliseconds{period};
    printf("New period (in ms): %lld \nPeriod: ", blink_period.count());
  }
  printf("Bye bye!");
}
