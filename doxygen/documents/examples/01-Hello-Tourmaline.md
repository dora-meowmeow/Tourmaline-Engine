\page 01-Hello-Tourmaline 01. Getting Started - Installing and Preparing Tourmaline

This example is meant to guide you on how to set up Tourmaline. This guide will assume that you already have the following:

- A linux machine (Not a must, but this guide won't explain how to use Tourmaline on Windows.).
- A Text Editor.
- A C++ Language Server (For the purposes of this guide it will be assumed that you are using clangd).

Additionally, the steps here are subject to change. Currently, Tourmaline only generate a pkg-config file (as of 2026-09-24). However as other build systems get supported, this page will be updated accordingly.

## 1. Requirements

You must install the following software on your computer.

- git
- gcc/g++ 14.2 or older (or any compiler that supports C++23)
- cmake
- make
- GLFW (with development/header files)
- pkg-config or pkgconf (Optional but recommended)

### If you want to generate the documentation, install these as well

- doxygen
- texlive
- texlive-dvi

## 2. Preparing Tourmaline Engine for Compiling

Run the following commands:

```bash
git clone https://git.thenight.club/cat/TourmalineEngine
mkdir TourmalineEngine/build
cd TourmalineEngine/build
cmake ..
```

This will prepare the project to be built. If you get any errors, please fix them first before proceeding to step 3.

\note If you want the files to be installed somewhere else, you should specify it by setting `CMAKE_INSTALL_PREFIX` at the cmake step. Example: `cmake -DCMAKE_INSTALL_PREFIX=/opt/ ..` .

## 3. Compiling and Installing Tourmaline Engine

Run `make` if you just want to compile Tourmaline. If you want to compile and install, please run `sudo make install`.

You can also run `make -jX` where X is the amount of threads to compile Tourmaline with. We suggest 8, but if your pc is powerful, you could set it higher.

## 4. Setting Up a Project

After installing Tourmaline, You can use `pkg-config` (or `pkgconf`) to link and compile your program with Tourmaline.

To test if Tourmaline runs correctly, compile this simple test program:

\include 01-Hello-Tourmaline/main.cpp

To compile your project, you just need to run the following:

```bash
g++ main.cpp `pkg-config Tourmaline --cflags --libs` -o program
```

\note You do not need to use GCC/G++. Clang and other compilers should work fine as long as the compiler supports C++23 features.

## 5. You are ready

That's it! You can now start using Tourmaline Engine. We highly suggest customising any part of the engine to your needs.

You can continue learning more about Tourmaline Engine at \ref examples.
