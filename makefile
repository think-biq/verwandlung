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
else
	OS = "unix-y"
	STD_FLAG = "--std=c++17"
	ZLIB_DIR = 
	ZLIB_NAME = z
	EXE_PATH = build/./Verwandlung
endif

setup:
	mkdir -p ./tmp ./bin
	mkdir -p ./build
	@echo $(OS) $(STD_FLAG) $(ZLIB_DIR) $(ZLIB_NAME) $(EXE_PATH)
	pushd ./build; cmake -DCMAKE_CXX_FLAGS=$(STD_FLAG) ..

# build-wandel-cli:
# 	clang++ --std=c++17 $(DEBUG_FLAG) -I./src -L./bin -L$(ZLIB_DIR) -L./dep/SmallFBX/build/SmallFBX/Release -lwandel -l$(ZLIB_NAME) -lSmallFBX ./src/cli.cpp -o ./bin/wandel

# build-wandel-cli-dep: build-wandel-lib build-wandel-cli

#build: build-wandel-lib-dep build-wandel-cli-dep
build: setup
	pushd ./build; cmake --build . --config Release

clean-smallfbx:
	rm -rf ./dep/SmallFBX/build

clean:
	rm -rf ./tmp ./bin
	rm -rf ./build

clean-all: clean-smallfbx clean 

run: build
	$(EXE_PATH) --list ./etc/fbx/OldFace.fbx