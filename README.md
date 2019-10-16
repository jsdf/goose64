# goose64

## install dependencies
```
brew install glfw3 glm lua

```

## build for macOS

``
./glbuild.sh
``

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

which produces the rom file nu1.n64