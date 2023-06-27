idf_component_register(
  SRCS "frpp.cpp"
  INCLUDE_DIRS "include"
  REQUIRES freertos)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++20)

