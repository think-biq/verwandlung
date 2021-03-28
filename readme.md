# Verwandlung

Library and tools to inspect and transform FBX files.

## Setup

### Prerequisits

* python >3.9
* cmake >3.19
* make >3.81 (checkout [scoop](https://scoop.sh/) for [make](https://github.com/ScoopInstaller/Main/blob/master/bucket/make.json) on windows)
* c++17 compatible build environment

### Platforms

* windows 10
* macOS 10.15.*

### Get the source

Fetch repository and dependencies / submodules.
```bash
git clone --recursive https://github.com/think-biq/verwandlung.git
```

### Build

```bash
make clean build test
```

Will configure and setup cmake build script generation, create a python virtual environment, install dependencies, build the dependencies, build pybind wrapper modules and verwandlung tools.

## Components

### wandel

Command line tool, to extract animation curves for blendshapes and bone animations to json (meta)data files, so it may be used with [LLV](https://github.com/think-biq/LLV).

### python_smallfbx

Python binding (via [pybind11](https://github.com/pybind/pybind11)) to the FBX management library [SmallFBX](https://github.com/i-saint/SmallFBX) by [i-saint](https://github.com/i-saint).

### python_wandel

Python binding (via [pybind11](https://github.com/pybind/pybind11)) for the wandel tool.

## Use as Library

Coming soon ...

## Use Command-line tools

Coming soon ...

## License

* [Verwandlung](license.md) (Copyright (C) blurryroots innovation qanat OÜ)

## Third-party licenses

* [pybind11](dep/pybind11/LICENSE) (Copyright (C) Wenzel Jakob)
* [SmallFBX](dep/SmallFBX/LICENSE.txt) (Copyright (C) i-saint)
* [zlib](dep/zlib/README) (Copyright (C) Jean-loup Gailly and Mark Adler)
