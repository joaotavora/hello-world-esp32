# My CMake template for ESP-IDF and C++

This was inspired directly from

* https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html#using-esp-idf-in-custom-cmake-projects
* The [linked example folder](https://github.com/espressif/esp-idf/tree/17d6768e65/examples/build_system/cmake/idf_as_lib)
* My own
  [hello-world-3000](https://github.com/joaotavora/hello-world-3000)
  C++ starter template, itself based on
  [ModernCppStarter](https://github.com/TheLartians/ModernCppStarter)


From the original README.md, still relevant here:

> This example illustrates using ESP-IDF components as libraries in
> custom CMake projects. The application in this example can run on
> either host or on an ESP32, and the appropriate libraries are linked
> to the executable depending on which target is specified. If the
> target is an ESP32, the libraries created from ESP-IDF components
> are linked. On the other hand, stub libraries are linked if example
> is meant to be run on the host to simulate the same application
> behavior.

## Micro tutorial from scratch in an Arch system

More or less from 0 on an Arch Linux system.  First install the
_Espressif IoT Development Framework_, aka ESP-IDF from AUR (Archlinux
User Repository).  You could install it from anywhere else, I suppose.

```bash
git clone https://aur.archlinux.org/esp-idf.git
cd esp-idf
makepkg -sir
```

Now, you must further ask the framework to download and install the
_toolchains_, via `pip` and a lot of Python I think.  It will be
installed in your `$HOME/.espressif`, which is not terrible.

To do that:

```bash
/opt/esp-idf/install.sh
```

Now install some standard tools (here, `clang` is just so you get the
useful `clangd` LSP tool and `lib32-glibc` is so that clangd trips
less on 32-bit stuff.)

```bash
sudo pacman -S cmake ccache clang
```

On Arch you need to add your user to the `uucp` group:

```bash
sudo usermod -a -G uucp $USER
```

This should give you permissions to `/dev/ttyUSB0` which is where
probably your chip is connected to.

Finally, clone this repository

```bash
git clone https://github.com/joaotavora/hello-world-esp32
```

Now pray to your favourite deity:

```bash
cd hello-world-esp32
make configure build flash monitor
```

### `export.sh` rarely or never needed

At a certain point, after `install.sh` installs the toolchains for the
various ESP32 variants into your `~/.espressif`, it will ask your to
run the `export.sh` to set `IDF_PATH`, `PATH` and a bunch of other
variables everytime you want to build something or use your ESP32
chip.

I've so far found this to be **unnecessary** as long as some of these
settings live in the `Makefile` and you invoke things from there.
Your mileage may vary.  See the `Makefile` and the top-level
`CMakeLists.txt`.

### Python package problems?

Python package problems?? Sometimes it seems that explicitly installing some more
Python modules is necessary (probably the `install.sh` didn't install
them or the `Makefile` isn't looking for them in `~/.espressif`).
It's not immensely serious and may install them directly with your
favourite `pip`.

```bash
pip install kconfiglib idf_component_manager esptool
```


## Clangd is important

This template works with a "normal" `clangd` language server: there's
no need to compile and install an Xtensa's specific version.  Clangd
will not be compiling your code, just analyzing it.  

* The `compile_commands.json` file generated in the `build` directory
  can be linked to the root of the project with `make
  compile_commands.json`.

* There is a hiccup with some flags in `compile_commands.json` that
  Clangd knows nothing about, as well as some "define's" that it can't
  yet guess, such as `-D__XTENSA__`.  The included `.clangd` file
  fixes this.

* You must start `clangd` with the `--query-driver` option to
  whitelist the Gcc cross-compiler in `compile_commands.json` and
  allow `clangd` to invoke that compiler discover the system include.
  `clangd --query-driver=**` is usually enough.

## Understanding CMake components

ESP/IDF "Components" are CMake objects that more or less neatly
encapsulate a reusable library.  They're a wrapper (not sure how thin
of a wrapper yet) around CMake's `add_library` function.  

In this template, components exist in separate directory under the
`src` folder.  So let's say we have two components `foo` and `bar`,
this is a possible folder structure.

```
src/
`-- foo/
|   |-- include/
|   |   `-- foo/
|   |       `-- foo.h
|   |-- main/
|   |   `-- main.cpp
|   |-- CMakeLists.txt
|   `-- foo.cpp
`-- bar/
    |-- include/
    |   `-- bar/
    |       `-- bar.h
    |-- main/
    |   `-- main.cpp
    |-- bar.cpp
    `-- CMakeLists.txt
```


The `CMakeLists.txt` files are automatically added with something
similar to `add_subdirectory` in the top-level `CMakeLists.txt`.  For
example `src/foo/CMakeLists.txt` may look like:

```Cmake
# src/foo/CMakeLists.txt
idf_component_register(
  SRCS "foo.cpp"
  INCLUDE_DIRS "include"
  REQUIRES driver)
```

Where "driver" is a "system" component found in
`/opt/esp-idf/components/driver` which provides access to GPIO pins.

If your own component `bar` depends on your own `foo` then it must
only list `foo` in its own requires:

```Cmake
# src/bar/CMakeLists.txt
idf_component_register(
  SRCS "bar.cpp"
  INCLUDE_DIRS "include"
  REQUIRES foo)
```

After doing this, `bar/bar.cpp` may now `#include "foo/foo.h"` in its
`bar/bar.cpp` file.

### The `main/main.cpp` file of each component

This file contains the `extern "C" void app_main(void)` entry point.
Because only one of these entry points may exist in any given project,
only one component will have this file be the file at any one given time.

That component is called the "main component" and is selected directly
in the top-level `CMakeLists.txt` file.


## Plausible output after `make flash monitor`

```
Hello world!
This is ESP32 chip with 2 CPU cores, WiFi/BT/BLE, silicon revision 0, 4MB external flash
Restarting in 10 seconds...
Restarting in 9 seconds...
Restarting in 8 seconds...
Restarting in 7 seconds...
Restarting in 6 seconds...
Restarting in 5 seconds...
Restarting in 4 seconds...
Restarting in 3 seconds...
Restarting in 2 seconds...
Restarting in 1 seconds...
Restarting in 0 seconds...
```
