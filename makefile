#
#    Utility to extract blendshape / morph target information from FBX files.
#
#    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
#    See license.md for details.
#
#    https://think-biq.com

DEBUG_FLAG =

setup:
	mkdir -p ./tmp ./bin
	mkdir -p ./build
	# -DCMAKE_BUILD_TYPE=Release 
	# -DSmallFBX_DIR=./dep/SmallFBX/src -DCMAKE_EXE_LINKER_FLAGS="-static" 
	pushd ./build; cmake -DCMAKE_CXX_FLAGS="/std:c++17" ..

# build-smallfbx:
# 	pushd dep/SmallFBX/build; cmake -DCMAKE_EXE_LINKER_FLAGS="-static" --build . --config Release

build-wandel-lib:
	echo "Narb"
# 	clang++ --std=c++17 $(DEBUG_FLAG) -c -I./dep/SmallFBX/src -I./src ./src/wandel.cpp -o ./tmp/wandel.o
# 	ar rc ./bin/libwandel.a ./tmp/wandel.o
# 	mv ./bin/libwandel.a ./bin/wandel.lib

# build-wandel-lib-dep: setup build-smallfbx build-wandel-lib
build-wandel-lib-dep:
	echo "Narb"

build-wandel-cli:
	clang++ --std=c++17 $(DEBUG_FLAG) -I./src -L./bin -L"/C/Program Files/zlib/lib" -L./dep/SmallFBX/build/SmallFBX/Release -lwandel -lzlibstatic -lSmallFBX ./src/cli.cpp -o ./bin/wandel

build-wandel-cli-dep: build-wandel-lib build-wandel-cli

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
	build/Release/./Verwandlung.exe