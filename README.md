# bred

Simple text editor written in [Daybreak](https://github.com/ExoKomodo/Daybreak)

## Important Links

- [Repository](https://github.com/ExoKomodo/bred)
- [Daybreak](https://github.com/ExoKomodo/daybreak)
- [Text Editor Creation Guide](https://viewsourcecode.org/snaptoken/kilo/)

## Setup

Clone the repo recursively

```shell
git clone --recursive git@github.com:ExoKomodo/bred
# Bootstrap the daybreak compiler, if necessary
export C_INCLUDE_PATH="./daybreak/src/include:${C_INCLUDE_PATH}"
# Also tested to work with clang and zig compilers
gcc ./bin/main.day.c -o ./bin/daybreak
```

## Build

### Linux

```shell
$ ./build.sh
#####################################
# Welcome to the Daybreak compiler! #
#####################################
[INFO]:Output file: ./out/Linux/bred
```
