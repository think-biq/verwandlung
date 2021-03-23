#
#    Utility to extract blendshape / morph target information from FBX files.
#
#    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
#    See license.md for details.
#
#    https://think-biq.com

DEBUG_FLAG =

ifeq '$(findstring ;,$(PATH))' ';'
	OS = "win"
	STD_FLAG = "/std:c++17"
	ZLIB_DIR = "/C/Program Files/zlib/lib"
	ZLIB_NAME = zlibstatic
	EXE_PATH = build/Release/./Verwandlung.exe
	EXE_PATH_RAW = build/Release/Verwandlung.exe
	LLDB = lldb
else
	OS = "unix-y"
	STD_FLAG = "--std=c++17"
	ZLIB_DIR = 
	ZLIB_NAME = z
	EXE_PATH = build/./Verwandlung
	EXE_PATH_RAW = build/Verwandlung
	LLDB = lldb
endif

setup-env:
	mkdir -p ./tmp ./bin
	mkdir -p ./build
	@echo $(OS) $(STD_FLAG) $(ZLIB_DIR) $(ZLIB_NAME) $(EXE_PATH)

setup-cmake-release:
	pushd ./build; cmake -DCMAKE_CXX_FLAGS=$(STD_FLAG) ..

setup-cmake-debug:
	pushd ./build; cmake -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_CXX_FLAGS=$(STD_FLAG) ..

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