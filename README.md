# Almost There

A rhythm game from scratch, using OpenGL (with GLFW/GLAD) on desktop and OpenGL ES on mobile devices, with OpenAL Soft for audio.

Currently only in an early stage, time is still required for it to run.

## How To Build

Currently only instructions for desktop is available.

### Prerequisites

- On Microsoft Windows, you'll need to install MSYS2 to install required packages and link everything.

- On macOS, using `brew` to install most packages should work fine. Not verified, though.

- Install the GLFW library, on ArchLinux run:
  
  ```bash
  pacman -S glfw-x11 # For X11
  pacman -S glfw-wayland # For Wayland
  ```
  
  For MSYS2 you'll want to run:
  
  ```bash
  pacman -S mingw-w64-x86_64-glfw
  ```
  
  If you're running a different architecture, replace `x86_64` with suitable value.

- Install required toolchain. This can vary from different platform, but generally you'll need the following. We've also marked our environment info in the brackets.
  
  - A C++ compiler which supports C++23 (G++ 12.2.0)
  
  - A C compiler which supports C11 (GCC 12.2.0)
  
  - Meson Build, along with Ninja Build, at least with version 0.58.0 (Meson 1.0.1 with Python 3.11.2, Ninja 1.11.1)
  
  - (Optional) Gcovr for code coverage analysis (Gcovr 5.2)

- If you're building using MSYS2, please make sure you've installed required packages with the prefix `mingw-w64-<arch>`, e.g. install `mingw-w64-x86_64-gcc` rather than regular `gcc`, the latter one might be missing necessary functions.
  
  AFAIK, this affects `pkg-config`, `gcc`, `g++`.
  
  Please also make sure to install Python natively (using the MSI installer), and run `pip3 install gcovr` from Command Prompt. Also, you'll want to inherit PATH from Windows to MSYS2 for Python scripts to work.

### Build

1. Clone this repository.
   
   ```bash
   git clone https://github.com/skjsjhb/AlmostThere.git --depth 1
   cd AlmostThere
   ```

2. Initialize build.
   
   ```bash
   meson setup build
   cd build
   ```

3. Run build.
   
   ```bash
   ninja # Or `meson compile` for other backends
   ```

4. Optionally run tests.
   
   ```bash
   ln -s ../assets ./assets
   meson test
   ```

5. The library should be available as `libatgame.dll(.a)` (Dynamic) and `libatgame.a` (Static). The build is now finished. The final game distribution is based on these libraries (basically just adding a wrapper).
   
   In future, instructions of how to build the rest parts will be available, please wait for a while :)


