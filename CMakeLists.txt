cmake_minimum_required(VERSION 3.16)

project(hello-world CXX)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
include(cmake/CPM.cmake)
include(cmake/tools.cmake)

list(APPEND EXTRA_COMPONENT_DIRS "./src")
file(GLOB my_components CONFIGURE_DEPENDS "./src/*")
foreach (comp ${my_components})
  get_filename_component(comp_name ${comp} NAME_WLE)
  idf_build_component(${comp})
endforeach()

if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(main_component "03-prioritization-ex")

  idf_component_get_property(requires ${main_component} REQUIRES)
  message(STATUS "Main component's '${main_component}' requires: ${requires}")

  option(TARGET "ESP-IDF friendly target" "esp32")
  assert(TARGET "Need A TARGET!")
  idf_build_process("${TARGET}"
    COMPONENTS freertos esptool_py ${main_component}
    SDKCONFIG_DEFAULTS ${CMAKE_SOURCE_DIR}/sdkconfig.defaults
    BUILD_DIR ${CMAKE_BINARY_DIR})

  set(elf_file ${CMAKE_PROJECT_NAME}.elf)
  add_executable(${elf_file} ./src/${main_component}/main/main.cpp)

  idf_build_get_property(component_aliases BUILD_COMPONENT_ALIASES)
  target_link_libraries(${elf_file} idf::freertos idf::spi_flash ${component_aliases})
  idf_build_executable(${elf_file})
endif()
