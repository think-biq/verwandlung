#
#    Utility to extract blendshape / morph target information from FBX files.
#
#    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
#    See license.md for details.
#
#    https://think-biq.com

FILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_DIR := $(shell dirname $(FILE_PATH))
PROJECT_NAME := $(notdir $(patsubst %/,%,$(dir $(FILE_PATH))))

DEBUG_FLAG :=
PYTHON := python3

ifeq '$(findstring ;,$(PATH))' ';'
	OS = "win"
	STD_FLAG = "/std:c++17 /EHa"
	EXE_PATH = build/Release/./Verwandlung.exe
	EXE_PATH_RAW = build/Release/Verwandlung.exe
	LLDB = lldb
	PYTHON_EXECUTABLE := $(shell $(PYTHON) -c "import sys; print(sys.executable)")
else
	OS = "unix-y"
	STD_FLAG = "--std=c++17"
	EXE_PATH = build/./Verwandlung
	EXE_PATH_RAW = build/Verwandlung
	LLDB = lldb
	PYTHON_EXECUTABLE := $(shell which $(PYTHON))
endif

.default: setup-env

setup-env:
	mkdir -p ./tmp ./bin
	mkdir -p ./build
	@echo PROJECT_DIR:$(PROJECT_DIR)
	@echo OS:$(OS)
	@echo STD_FLAG:$(STD_FLAG)
	@echo EXE_PATH: $(EXE_PATH)
	@echo PYTHON_EXECUTABLE: $(PYTHON_EXECUTABLE)

setup-cmake-release:
	pushd ./build; cmake -DPYTHON_EXECUTABLE=$(PYTHON_EXECUTABLE) -DCMAKE_CXX_FLAGS=$(STD_FLAG) ..

setup-cmake-debug:
	pushd ./build; cmake -DPYTHON_EXECUTABLE=$(PYTHON_EXECUTABLE) -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_CXX_FLAGS=$(STD_FLAG) ..

setup-release: setup-env setup-cmake-release

setup-debug: setup-env setup-cmake-debug

build-release: setup-release
	pushd ./build; cmake --build . --config Release

build-debug: setup-debug
	pushd ./build; cmake --build . --config Debug

clean-smallfbx:
	rm -rf ./dep/SmallFBX/build

clean:
	rm -rf ./tmp ./bin
	rm -rf ./build

clean-all: clean-smallfbx clean 

run: build-release
	$(EXE_PATH) --list ./etc/fbx/OldFace.fbx

debug: build-debug
	$(LLDB) -- $(EXE_PATH_RAW) --list ./etc/fbx/Gunan_animiated.fbx