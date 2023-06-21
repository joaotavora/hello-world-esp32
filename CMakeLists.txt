cmake_minimum_required(VERSION 3.16)

project(hello-world CXX)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
include(cmake/CPM.cmake)
include(cmake/tools.cmake)

list(APPEND EXTRA_COMPONENT_DIRS "./src")
file(GLOB my_components CONFIGURE_DEPENDS "./src/*")
foreach (comp ${my_components})
  get_filename_component(comp_name ${comp} NAME_WLE)
  message(STATUS "HEYHEY: this component ${comp_name}")
  idf_build_component(${comp})
endforeach()

if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(main_component "00-greeter")

  idf_component_get_property(requires ${main_component} REQUIRES)
  message(STATUS "HEYHEY: The '${main_component}' requires: ${requires}")

  option(TARGET "ESP-IDF friendly target" "esp32")
  assert(TARGET "Need A TARGET!")
  idf_build_process("${TARGET}"
    # although esptool_py does not generate static library,
    # processing the component is needed for flashing related
    # targets and file generation
    COMPONENTS freertos esptool_py ${main_component}
    SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
    BUILD_DIR ${CMAKE_BINARY_DIR})

  set(elf_file ${CMAKE_PROJECT_NAME}.elf)
  add_executable(${elf_file} ./src/${main_component}/main.cpp)

  idf_build_get_property(component_aliases BUILD_COMPONENT_ALIASES)
  message(STATUS "HEYHEY: Linking main executable to: ${component_aliases}")
  # string(REPLACE ";" " " x ${x})
  # message(STATUS "HEYHEY: About to link to ${x}")

  target_link_libraries(${elf_file} idf::freertos idf::spi_flash ${component_aliases})
  # Attach additional targets to the executable file for flashing,
  # linker script generation, partition_table generation, etc.
  idf_build_executable(${elf_file})
endif()
