#include "frpp/frpp.h"
#include <iostream>
#include <string>
#include "driver/uart.h"

const static std::string msg = "BARKA................................................RUM";
using namespace std::chrono_literals;

extern "C" void app_main(void) {
  std::cout.setf(std::ios::unitbuf);
  // needs:
  //
  // stty -F /dev/ttyUSB0 300 -icrnl
  // socat stdin /dev/ttyUSB0
  uart_set_baudrate(CONFIG_ESP_CONSOLE_UART_NUM, 300);

  frpp::sleep_for(1s);

  auto task1 = frpp::async([]{
    auto len = msg.size();
    while (true) {
      for (const auto c : msg) {
        std::cout << c;
      }
      std::cout << std::endl;
      frpp::sleep_for(1000ms);
    }
  }, 1, "task1");

  {
    auto task2 = frpp::async([]{
      while (true) {
        std::cout << "*";
        frpp::sleep_for(100ms);
      }
    }, 2, "task2");

    frpp::sleep_for(10s);
  }
  std::cout << "Only the barkadeer remains\n";

  while (true) {
    frpp::sleep_for(10s);
    std::cout << "Main loop going around\n";
  }
}
