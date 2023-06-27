#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "freertos/portmacro.h"

namespace frpp {
  class task {
    TaskHandle_t p_;

  public:
    explicit task(TaskHandle_t rawtask) : p_(rawtask) {}
    ~task() {
      vTaskDelete(p_);  // not sure I can even do this
    }
  };

  template <class Callable>
  [[nodiscard]] task async(Callable&& f, size_t priority = 1,
                           std::string name = "dummy",
                           size_t stack_depth = 2048) {
    using Callable_decay_t = std::decay_t<Callable>;
    Callable_decay_t* ptr = new Callable_decay_t(std::forward<Callable>(f));
    TaskHandle_t rawtask{};
    auto fun = [](void* cookie) {
      try {
        std::unique_ptr<Callable_decay_t> pf{
            static_cast<Callable_decay_t*>(cookie)};
        (*pf)();
      } catch (std::exception& e) {
        std::cerr << "Task '" << pcTaskGetName(xTaskGetCurrentTaskHandle())
                  << "'"
                  << " caught '" << e.what() << "'" << std::endl;
      }
      vTaskDelete(NULL);
    };
    auto retval = xTaskCreatePinnedToCore(fun, name.c_str(), stack_depth, ptr,
                                          priority, &rawtask, 1);
    if (!retval) {
      delete ptr;
      throw std::runtime_error("xTaskCreate() failed!");
    }
    return task(rawtask);
  }
}  // namespace frpp
