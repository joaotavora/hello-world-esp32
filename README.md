# My CMake template for ESP-IDF (ESP32 in this case)

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

## Micro tutorial

More or less from 0 on an Arch Linux system.  First install the
"Espressif IoT Development Framework", aka ESP-IDF from AUR (Archlinux
User Repository).  You could install it from anywhere else, I suppose.

```
git clone https://aur.archlinux.org/esp-idf.git
cd esp-idf
makepkg -sir
```

Now, you must further ask the framework to download and install the
_toolchain_, via `pip` and a lot of Python I think.  It will be
installed in your `$HOME/.espressif`, which is not terrible.

Now install some standard tools

```
sudo pacman -S cmake ccache clangd
```

On Arch you need to add your user to the `uucp` group:

```
sudo usermod -a -G uucp $USER
```

This should give you permissions to `/dev/ttyUSB0` which is where
probably your chip is connected to.




Finally, clone this repository

```
git clone https://github.com/joaotavora/hello-world-esp32
```

Now pray to your favourite deity:

```
cd hello-world-esp32
make configure build flash monitor
```

### Interesting

At a certain point, after `install.sh` installs the toolchain, it will
ask your to run the `export.sh` to set `IDF_PATH`, `PATH` and a bunch
of other variables everytime you want to build something or use your
ESP32 chip.

I've so far found this to be **unnecessary** as long as some of these
settings live in the `Makefile` and you invoke things from there.
Your mileage may vary.  See the `Makefile` and the top-level
`CMakeLists.txt`.

### Output

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

## Configuring this Example

To modify the example ESP-IDF project configuration, first create the
CMake build directory. 

This is done with `make configure`.

Then execute the menuconfig build target in the build directory:

```bash
cmake --build build -- menuconfig
```
