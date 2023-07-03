#include "frpp/frpp.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdexcept>
#include "FreeRTOSConfig.h"

namespace frpp {
  void task::suspend() { vTaskSuspend(static_cast<TaskHandle_t>(p_)); }
  void task::resume() { vTaskResume(static_cast<TaskHandle_t>(p_)); }
  task::~task() {
    vTaskDelete(static_cast<TaskHandle_t>(p_));
  }

  native_handle_type frpp::detail::create (void (*fun)(void *),
                                           const std::string& name,
                                           std::size_t stack_size,
                                           void *cookie,
                                           uint32_t priority,
                                           uint32_t affinity) {
    TaskHandle_t raw_handle;
    BaseType_t retval;
    if (affinity) {
      retval = xTaskCreatePinnedToCore(fun,
                                       name.c_str(),
                                       stack_size, cookie,
                                       priority, &raw_handle, affinity);
    } else {
      retval = xTaskCreate(fun,
                           name.c_str(),
                           stack_size, cookie,
                           priority, &raw_handle);
    }
    if (retval != pdPASS) throw std::runtime_error("Couldn't create task");
    return raw_handle;
  }
  void detail::delete_current_task() {vTaskDelete(nullptr);}

  const std::size_t milliseconds_in_a_tick_period = portTICK_PERIOD_MS;

  void sleep_for_ticks(std::size_t ticks) { vTaskDelay(ticks); }
}

