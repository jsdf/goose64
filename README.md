# goose64

this is a demake of untitled goose game for the nintendo 64

## get it

download the [most recent N64 ROM build here](src/goose64.n64)

run it in your favorite emulator or flashcart

## build for emulator/console

### install the n64 sdk

windows instructions: https://n64squid.com/homebrew/n64-sdk/
macOS/linux instructions: https://www.retroreversing.com/n64-sdk-setup
on linux/macOS you'll need to make sure the `wine` command is available on your PATH

install the n64 sdk into the root of the wine C: drive so you have eg.
```
C:\ultra
C:\nintendo
```
etc.


### building the game

to build the rom (all commands should be run in the `src` directory of this repo):

linux/macOS:

```
./build.sh
```

windows:

```
compile.bat
```

this produces the rom file goose64.n64

environment variables which affect the build:

- `OPTIMIZE=1`: enable gcc optimization and use non-debug versions of sdk libraries
- `ED64=1`: build with everdrive64 logging support (see [ed64log](https://github.com/jsdf/ed64log)). don't use unless running on an everdrive64

you can also create a file called `localdefs.makefile` containing any variables to override in the build, and it will be automatically included by the makefile.

## build for macOS native executable

this repo also includes an opengl based version of the game, which runs on macOS

install dependencies:
```
# eg. using homebrew on macOS
brew install glm
```

then open the goose64glut project with xcode, build and run


## regenerate n64 header files from the OBJ models
if you update .obj model files:

first make sure you have lua installed

```
# eg. using homebrew on macOS
brew install lua
```

then, to rebuild model header files

```
./rebuild_models.sh
```

## regenerate n64 header files for sprites
if you update any texture files:

make sure you have python and pillow installed

```bash
pip install pillow
```

then, to rebuild sprites

```bash
./sprites.sh 
```

## export map object data

- open blender (or use `./blender.sh` to see console output)
- in the blender text editor editor, open and run export_positions.py then open and run export_collision_mesh.py
- see header files are created

