# goose64

this is a demake of untitled goose game for the nintendo 64

## get it

download the [most recent N64 ROM build here](src/goose64.n64)

run it in your favorite emulator or flashcart

## build for emulator/console

install wine from https://wiki.winehq.org/Download

make sure the `wine` command is on your PATH. if using Wine Stable on macOS you can symlink it:
```
ln -s /Applications/Wine\ Stable.app/Contents/Resources/wine/bin/wine /usr/local/bin/wine
```

install the n64 sdk into the root of the wine C: drive so you have eg.
```
C:\ultra
C:\nintendo
```
etc.

then build the rom with

```
./build.sh
```

which produces the rom file goose64.n64

environment variables which affect the build:

- `OPTIMIZE=1`: enable gcc optimization and use non-debug versions of sdk libraries
- `ED64=1`: build with everdrive64 logging support (see [ed64log](https://github.com/jsdf/ed64log)). don't use unless running on an everdrive64

you can also create a file called `localdefs.makefile` containing any variables to override in the build, and it will be automatically included by the makefile.

## build for macOS

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

