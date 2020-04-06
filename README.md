# goose64

this is a demake of untitled goose game for the nintendo 64


## install dependencies
```
brew install glm lua

```

## build for macOS
open the goose64glut project with xcode, build and run

## build for emulator/console

install wine from https://wiki.winehq.org/Download

it needs to be in your Applications folder, eg.
```

/Applications/Wine Stable.app
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


## generate n64 header files from the OBJ models
```
./rebuild_models.sh
```

## generate n64 header files for sprites
make sure you have pillow installed

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

