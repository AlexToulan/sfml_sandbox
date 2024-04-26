# sfml_sandbox
A light-weight C++ game engine and 2D sandbox.
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
Select a game mode by passing `--game_mode [game_mode_name]` (e.g. `GameOfLife`, `Orbito`). Game modes can be changed at run-time with the comma `,` or period `.` key.
### Conway's Game of Life
 - Uses a threadsafe event system to pass messages between the game mode and a pool of thread workers.
 - Scans seed from a `.png` file
 - `Space` pauses the game and allows individual cell toggling with left mouse-click
### Orbito
 - Click and drag a piece from a pile in the top corners to place on the board
 - Cycle the board with `space` or by clicking the button in the center of the board
 - Get four in a row to win
 - Press `enter`/`return` to restart the game
## Console
 - Press `grave` to open the game console
 - The `help` command will list all available commands
 - Navigate console history with `up` and `down` arrow keys
 - Autocomplete commands with `tab`
 - Complete commands will automatially show help text in the console buffer
## Future Work
 - Break engine out into separate repo
 - Windows support
 - PyTorch powered AI
 - Pathfinding for AI training
## Known Bugs
### Build/Setup
 - Need to run `setup.sh` after reboot on some machines
## Footnote
Special thanks to the developers of thirdparty libs/tools used in sfml_sandbox
 - [SFML](https://www.sfml-dev.org/)
 - [ArgParse](https://github.com/morrisfranken/argparse)
 - [GTest](https://github.com/google/googletest)
