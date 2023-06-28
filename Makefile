DEVICE=/dev/ttyUSB0
TARGET=esp32
export IDF_PATH=/opt/esp-idf
IDF_INSTALLED=$(HOME)/.espressif

export PATH := $(IDF_PATH)/tools$\
               $(IDF_PATH)/components/espcoredump:$\
               $(IDF_PATH)/components/partition_table:$\
               $(IDF_PATH)/components/app_update:$\
               $(IDF_INSTALLED)/tools/xtensa-$(TARGET)-elf/esp-2022r1-11.2.0/xtensa-esp32-elf/bin:$\
               $(IDF_INSTALLED)/python_env/idf5.0_py3.11_env/bin:$\
               $(PATH)
export CPM_SOURCE_CACHE=$(HOME)/.cache/CPM
export CPM_USE_NAMED_CACHE_DIRECTORIES=true

CMAKE_IDF=-DCMAKE_TOOLCHAIN_FILE=$(IDF_PATH)/tools/cmake/toolchain-$(TARGET).cmake \
	  -DTARGET=$(TARGET)

CMAKE_FLAGS = $(CMAKE_IDF) -DUSE_CCACHE=YES -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

build: configure
	cmake --build build

echo:
	echo $$PATH

configure:
	echo $$PATH
	cmake $(CMAKE_FLAGS) -S. -Bbuild

flash: build
	cd build && \
        python $(IDF_PATH)/components/esptool_py/esptool/esptool.py -p $(DEVICE) write_flash @flash_project_args

monitor: build
	python $(IDF_PATH)/tools/idf_monitor.py -p $(DEVICE) build/idf_as_lib.elf

clean:
	rm -rf build*

compile_commands.json:
	ln -sf build/compile_commands.json compile_commands.json
