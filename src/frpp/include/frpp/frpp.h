#include <memory>
#include <stdexcept>
#include <string>
#include <chrono>

#include "freertos/portmacro.h"


namespace frpp {
  using native_handle_type = void*;
  namespace detail {
    native_handle_type create(void fun(void*), const std::string&,
                              std::size_t stack_size,
                              void* cookie,
                              uint32_t priority,
                              uint32_t affinity);

    void delete_current_task();
  }

  class task {
    native_handle_type p_{};



  public:
    explicit task(native_handle_type rawtask) : p_{rawtask} {}
    task(const task&) = delete;
    task& operator=(const task&) = delete;
    task(task&&) = default;
    task& operator=(task&&) = default;

    void suspend();
    void resume();
    ~task();
  };


  template <class Callable>
  [[nodiscard]] task async(Callable&& f, size_t priority = 0,
                           const std::string& name = "dummy",
                           size_t stack_size = 2048) {
    using Callable_decay_t = std::decay_t<Callable>;
    auto ptr = std::make_unique<Callable_decay_t>(std::forward<Callable>(f));
    auto fun = [](void* cookie) {
      try {
        std::unique_ptr<Callable_decay_t> pf{
            static_cast<Callable_decay_t*>(cookie)};
        (*pf)();
      } catch (std::exception& e) {
        // fprintf(stderr, "Task '%s' caught '%s'\n", pcTaskGetName(xTaskGetCurrentTaskHandle()), e.what());
        fprintf(stderr, "Task threw '%s'", e.what());
      }
      detail::delete_current_task();
    };
    auto retval=task(detail::create(fun, name, stack_size, ptr.get(), priority, 1));
    ptr.release();
    return retval;
  }

  void sleep_for_ticks(std::size_t ticks);
  extern const std::size_t milliseconds_in_a_tick_period;

  template< class Rep, class Period >
  void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration) {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(sleep_duration);
    sleep_for_ticks(ms.count()/milliseconds_in_a_tick_period);
  }
}  // namespace frpp
