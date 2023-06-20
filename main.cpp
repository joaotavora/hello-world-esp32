/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */


namespace greeter {
  void greet_and_reboot();
}


extern "C" void app_main(void) {
  greeter::greet_and_reboot();
}
