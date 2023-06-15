cmake_minimum_required(VERSION 3.16)

project(idf_as_lib CXX)

option(TARGET "ESP-IDF friendly target" "esp32")

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
include(cmake/CPM.cmake)
include(cmake/tools.cmake)

assert(TARGET "Need A TARGET!")

# Create idf::{target} and idf::freertos static libraries
idf_build_process("${TARGET}"
  # try and trim the build; additional components
  # will be included as needed based on dependency tree
  #
  # although esptool_py does not generate static library,
  # processing the component is needed for flashing related
  # targets and file generation
  COMPONENTS freertos esptool_py
  SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
  BUILD_DIR ${CMAKE_BINARY_DIR})


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(elf_file ${CMAKE_PROJECT_NAME}.elf)
add_executable(${elf_file} main.cpp)

target_link_libraries(${elf_file} idf::freertos idf::spi_flash)
# Attach additional targets to the executable file for flashing,
# linker script generation, partition_table generation, etc.
idf_build_executable(${elf_file})
