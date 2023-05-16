# K-D Tree

This is some kind of space subdivision demo / game implemented in C.

## Toolchain Setup (Windows)

### C Compiler

I picked `mingw` as opposed to `msvc` as it probably offers better portability.
To download this I first sought out
<https://github.com/niXman/mingw-builds-binaries/releases>
as I had no intention of building this myself.
I picked the least microsoft-like sounding name out of the 64 bit releases,
which was `x86_64-12.2.0-release-posix-seh-ucrt-rt_v10-rev2.7z`.
The final download URL was
<https://github.com/niXman/mingw-builds-binaries/releases/download/12.2.0-rt_v10-rev2/x86_64-12.2.0-release-posix-seh-ucrt-rt_v10-rev2.7z>

If you would prefer an alternative installation method you can go to
<https://sourceforge.net/projects/mingw-w64/> to obtain some kind of installer
for the same purpose.
It will ask you questions that
<https://stackoverflow.com/questions/29947302/meaning-of-options-in-mingw-w64-installer>
can help you to answer, but not by much,
and I'm not sure the answers matter much either.
However this will be an older version of the compiler, which among other things
may mean that random number generation is broken, or maybe thats only for C++.

I recommend opting for the first option and using 7-Zip to unzip it somewhere.
Either way you __must__ add the `bin` directory to your PATH,
or else gcc and g++ will never generate output files, and fail silently.
Then restart your terminal, or whatever program it is integrated into.

### C Graphics Libraries

- Go to <https://glew.sourceforge.net/> and download binaries for Windows
32-bit and 64-bit. I downloaded the latest which was 2.1.0.
- Go to <https://www.glfw.org/download.html> and download 64-bit Windows
binaries. I downloaded the latest which was 3.3.8.

GLEW enables OpenGL functionality, and GLFW lets us manage windows and input.

### C Linear Algebra Libraries

I've used GLM for C++ before, and for C <https://github.com/recp/cglm>
looks promising. I used `git clone https://github.com/recp/cglm` to clone into
the same folder as the other libraries. To keep things simple I made a folder
for my C projects and inside it I put this project, these libraries,
and the compiler. I didn't rename anything, so the subfolders are as follows:

```text
cglm
glew-2.1.0
glfw-3.3.8.bin.WIN64
kdtree
mingw
```

## Building and Running on Windows

There are `.cmd` files in the root project directory:

- `build.cmd` will build and run in debug mode
- `build_release.cmd` will make a release build
- You may need to create `custom.cmd` to override paths in `common.cmd`

Both of these build types require `glew32.dll` in the folder of the executable,
which is important for distribution. The build scripts take care of this. I've
discovered a nice way to use Visual Studio Code for this project, which is
straightforward to set up and which I am happy to share.

## Building and Running on Linux / macOS

Use `build.sh` to build and run in debug mode

## Next Steps

- Support drawing of partial world
- Determine when the world should redrawn / regerated and do so
- Let Graph draw a variable number of points / tris / rects
- Either add checks for the correct shader file extension, or remove extensions
