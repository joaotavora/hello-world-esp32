export CPM_SOURCE_CACHE=${HOME}/.cache/CPM
export CPM_USE_NAMED_CACHE_DIRECTORIES=true

DEVICE=/dev/ttyUSB0
TARGET=esp32
IDF_PATH=/opt/esp-idf
IDF_COMPONENTS=${IDF_PATH}/components
IDF_INSTALLED=/home/capitaomorte/.espressif

export PATH := ${IDF_COMPONENTS}/espcoredump:${IDF_COMPONENTS}/partition_table:${IDF_COMPONENTS}/app_update:${IDF_INSTALLED}/tools/xtensa-${TARGET}-elf/esp-2022r1-11.2.0/xtensa-esp32-elf/bin:${IDF_INSTALLED}/python_env/idf5.0_py3.11_env/bin:/opt/esp-idf/tools:${PATH}
export IDF_PATH=/opt/esp-idf

CMAKE_IDF=-DCMAKE_TOOLCHAIN_FILE=${IDF_PATH}/tools/cmake/toolchain-${TARGET}.cmake \
	  -DTARGET=${TARGET}

CMAKE_FLAGS = ${CMAKE_IDF} -DUSE_CCACHE=YES -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

configure:
	echo $$PATH
	cmake ${CMAKE_FLAGS} -S. -Bbuild

build: configure
	cmake --build build

flash: build
	cd build && \
        python ${IDF_PATH}/components/esptool_py/esptool/esptool.py -p ${DEVICE} write_flash @flash_project_args

monitor: build
	python ${IDF_PATH}/tools/idf_monitor.py -p ${DEVICE} build/idf_as_lib.elf

clean:
	rm -rf build*
