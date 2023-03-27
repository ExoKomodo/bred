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
cd bred
```

### Bootstrapping daybreak

Checkout whatever commit or branch you want to use, then run `bootstrap.sh`

```shell
cd daybreak
git checkout some_commit_or_branch
cd ..
./bootstrap.sh
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
