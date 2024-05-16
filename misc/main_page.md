@mainpage Rishka SDK
@tableofcontents

<div align="center">
    <img src="rishka-logo.png" width="140" />
</div>

![Arduino CI](https://github.com/nthnn/rishka/actions/workflows/arduino_ci.yml/badge.svg) ![Arduino Lint](https://github.com/nthnn/rishka/actions/workflows/arduino_lint.yml/badge.svg) ![Rust Build CI](https://github.com/nthnn/rishka/actions/workflows/rust_ci.yml/badge.svg) ![SDK Build CI](https://github.com/nthnn/rishka/actions/workflows/sdk_ci.yml/badge.svg) ![GitHub repo size](https://img.shields.io/github/repo-size/nthnn/rishka?logo=git&label=Repository%20Size)

Rishka is a RISC-V virtual runtime in C/C++ made for ESP32-WROVER with PSRAM specifically developed as kernel for [Jessy OS](https://github.com/nthnn/Jessy-OS).

<div align="center">
    <img src="rishka-ili9341.gif" />
    <br/><br/>
    <i>Rishka running shell example with an ILI9341 TFT LCD and FabGL.</i>
</div>

## How Does It Work?

The Rishka virtual runtime is a system designed to run special RISC-V binaries smoothly on ESP32-WROVER microcontroller units. Under the hood, the virtual machine serves as the main engine, while the kernel acts as its core component. The kernel handles system calls efficiently, providing a range of interfaces for accessing different system features and services.

<p align="center">
    <img src="assets/rishka-flow.png" width="65%" />
    <br/>
    <i>Simple comparison of Rishka-based OS and Linux-based OS (with an Ubuntu logo, lol).</i>
</p>

These include basic functions like managing files, as well as more complex tasks like controlling GPIO pins, attaching and detaching interrupts, and communication protocols such as I2C and SPI. Additionally, the kernel manages system utilities specific to the Jessy OS, handling tasks like sub-process runtime and memory allocation.

## Installation

### Rishka VM

Rishka VM is a lightweight and efficient virtual runtime environment designed for RISC-V binaries on ESP32-WROVER microcontrollers. Follow these steps to integrate Rishka into your Arduino projects:

1. Clone Rishka to your Arduino libraries, by typing the command below:

```bash
cd ~/Arduino/libraries
git clone --depth 1 https://github.com/nthnn/rishka.git
```

2. Open your Arduino IDE, then navigate to `File > Examples > rishka` and select an example suitable for your project.

3. You're all set! Integrate Rishka into your project and start leveraging its capabilities.

### Compiling Examples

#### Using rishka-cc tool

To use `rishka-cc`, you can get it from the release page if available or compile it yourself by typing the following on your terminal. Just make sure you have installed Rust compiler and Cargo package manager on your system.

```bash
cargo build --release
```

Alternatively, you can install the `rishka-cc` tool by typing the following below on your system terminal:

```bash
sh -c "$(curl -fsSL https://raw.githubusercontent.com/nthnn/rishka/main/support/install_rishka_cc.sh)"
```

Before using `rishka-cc`, you must configure two (2) environment variables, as shown below.

```bash
export RISHKA_LIBPATH=<path to sdk library folder>
export RISHKA_SCRIPTS=<path to scripts folder>
```

The `RISHKA_LIBPATH` must be a folder where the `librishka.h` header file is located, while the `RISHKA_SCRIPTS` should be a folder where both `launch.s` and the `link.ld` files are located. For example, assuming Rishka was moved to the libraries folder on Arduino IDE:

```bash
export RISHKA_LIBPATH=/Arduino/libraries/rishka/sdk
export RISHKA_SCRIPTS=/Arduino/libraries/rishka/scripts
```

If no problems occured and was configured as instructed, you can now seamlessly use the `rishka-cc`.

<p align="center">
  <img src="rishka-cc.png" width="90%" />
</p>

#### Manually Compiling

To compile SDK examples provided with Rishka, follow these steps:

1. If you haven't already, install Qrepo by following the instructions available [here](https://github.com/nthnn/Qrepo).

2. Ensure you have the RISC-V64 GCC toolchain installed by running:

```bash
sudo apt install gcc-riscv64-unknown-elf
```

3. Open a terminal and navigate to the directory where you cloned the Rishka repository.

4. Use Qrepo to compile the examples by executing the following command:

```bash
qrepo run compile <source-file> <output-name>
```

Replace `<source-file>` with the path to the source file of the example you want to compile and `<output-name>` with the desired name for the output binary.

Examples:

```bash
qrepo run compile examples/sdk/hello.cpp hello
# This will output the binary file to dist/hello.bin
```

Now you have successfully compiled the example and can proceed with using the generated binary file.

## Dumping Raw Binaries

Dumping raw binary files can be helpful in debugging programs, traditionally. Hence, a simple script in Qrepo is available to dump instructions from a raw binary file of Rishka. You can utilize it by typing the following:

```bash
qrepo run dump <filename>
```

## Example

This example demonstrates the usage of Rishka virtual machine on an ESP32-WROVER microcontroller. It initializes serial communication and SD card, waits for user input via serial port, loads the specified file into the Rishka VM, executes it, and then waits for the next input.

```cpp
#include <fabgl.h>
#include <rishka.h>
#include <SD.h>
#include <SPI.h>

#define TFT_CS     5            // TFT SPI select pin
#define TFT_SCK    18           // TFT SPI clock pin
#define TFT_MOSI   23           // TFT SPI MOSI pin
#define TFT_DC     15           // TFT data/command pin
#define TFT_RESET  4            // TFT reset pin
#define TFT_SPIBUS VSPI_HOST    // TFT SPI bus

#define SD_CS      2            // SD card chip select pin
#define SD_SCK     14           // SD card SPI clock pin
#define SD_MOSI    13           // SD card SPI MOSI pin
#define SD_MISO    12           // SD card SPI MISO pin

// TFT display controller and Terminal instance
fabgl::ILI9341Controller DisplayController;
fabgl::Terminal Terminal;

// SPI instance for SD card
SPIClass sdSpi(HSPI);

void setup() {
    // Initialize TFT display
    DisplayController.begin(TFT_SCK, TFT_MOSI, TFT_DC, TFT_RESET, TFT_CS, TFT_SPIBUS);
    DisplayController.setResolution("\"TFT_320x240\" 320 240");

    // Initialize terminal
    Terminal.begin(&DisplayController);
    Terminal.loadFont(&fabgl::FONT_8x14);
    Terminal.enableCursor(true);

    // Initialize SD card
    sdSpi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if(!SD.begin(SD_CS, sdSpi, 80000000)) {
        Terminal.println("Card \e[94mMount\e[97m Failed");
        return;
    }

    // Rishka virtual machine instance
    RishkaVM* vm = new RishkaVM();
    // Initialize Rishka VM
    vm->initialize(&Terminal);

    if(!vm->loadFile("/sysinfo.bin"))
        vm->panic("Failed to \e[94mload\e[97m specified file.");

    // Run loaded program
    vm->run(0, NULL);
    // Reset VM after program execution
    vm->reset();
}

void loop() {
    // Delay to prevent continuous execution
    vTaskDelay(10);
}
```

## Contributing

Contributions to Rishka are highly encouraged and appreciated! To contribute new features, bug fixes, or enhancements, please adhere to the following guidelines:

1. Fork the Rishka repository.
2. Create a new branch for your changes: `git checkout -b feature-name`.
3. Implement your changes and commit them: `git commit -m "Added new feature"`.
4. Push your changes to the branch: `git push origin feature-name`.
5. Submit a pull request for review and inclusion.

## License

Rishka is distributed under the GNU General Public License v3.0. For further details, refer to the [LICENSE](https://github.com/nthnn/rishka/blob/main/LICENSE) file.

```
This program is free software: you can redistribute it and/or modify  
it under the terms of the GNU General Public License as published by  
the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program. If not, see <http://www.gnu.org/licenses/>.
```