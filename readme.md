# Verwandlung

Library and tools to inspect and transform FBX files.

## Setup

### Prerequisits

* python >3.9
* cmake >3.19
* c++ build environment

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
