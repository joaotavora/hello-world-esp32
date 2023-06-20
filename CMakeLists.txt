cmake_minimum_required(VERSION 3.16)

project(hello-world CXX)

option(TARGET "ESP-IDF friendly target" "esp32")

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
include(cmake/CPM.cmake)
include(cmake/tools.cmake)

assert(TARGET "Need A TARGET!")
set(EXTRA_COMPONENT_DIRS "./src")
idf_build_component(./src/00-greeter/)

idf_build_process("${TARGET}"
  # although esptool_py does not generate static library,
  # processing the component is needed for flashing related
  # targets and file generation
  COMPONENTS freertos esptool_py 00-greeter
  SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
  BUILD_DIR ${CMAKE_BINARY_DIR})


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(elf_file ${CMAKE_PROJECT_NAME}.elf)
add_executable(${elf_file} main.cpp)

target_link_libraries(${elf_file} idf::freertos idf::spi_flash idf::00-greeter)
# Attach additional targets to the executable file for flashing,
# linker script generation, partition_table generation, etc.
idf_build_executable(${elf_file})
