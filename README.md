# sfml_sandbox
A simple 2D sandbox for testing algorithms and game design with C++ on Linux.
## Build
### Ubuntu/Debian
 - Install [Clang 17](https://ubuntuhandbook.org/index.php/2023/09/how-to-install-clang-17-or-16-in-ubuntu-22-04-20-04/)
   ```
   wget https://apt.llvm.org/llvm.sh
   chmod u+x llvm.sh
   ./llvm.sh 17
   ```
 - Install cmake `apt install build-essential cmake`
 - (optional) Install [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) VSCode extension
   - Configure CMake Tools to use Clang 17 with Debug selected
 - Install [SFML Dependencies](https://www.sfml-dev.org/tutorials/2.6/compile-with-cmake.php)
    ```
    apt install \
    libfreetype6-dev \
    libx11-dev \
    libxrandr-dev \
    libgl1-mesa-dev \
    libudev-dev \
    libopenal-dev \
    libflac-dev \
    libogg-dev \
    libvorbis-dev -y
    ```
 - Run `./setup.sh`
   - (optional) For normal build development press Ctrl+Shift+B
### Windows
Untested
### Mac
Not supported
## Game Modes
 - Conway's Game of Life
 - Orbito
## Future Work
 - Windows support (if I find another reason to install it)
 - PyTorch powered AI
 - Pathfinding for AI training
## Known Bugs
### Build/Setup
 - Need to run `setup.sh` after reboot
