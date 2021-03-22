#
#    Utility to extract blendshape / morph target information from FBX files.
#
#    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
#    See license.md for details.
#
#    https://think-biq.com

DEBUG_FLAG = -g

setup:
	mkdir -p ./tmp ./bin
	mkdir -p dep/SmallFBX/build
	pushd dep/SmallFBX/build; cmake ../src;

build-smallfbx:
	pushd dep/SmallFBX/build; cmake --build .

build-wandel-lib: setup build-smallfbx
	clang++ --std=c++17 $(DEBUG_FLAG) -c -I./dep/SmallFBX/src -I./src ./src/wandel.cpp -o ./tmp/wandel.o
	ar rc ./bin/libwandel.a ./tmp/wandel.o

build-wandel-cli: build-wandel-lib
	clang++ --std=c++17 $(DEBUG_FLAG) -I./src -L./bin -L./dep/SmallFBX/build/SmallFBX -lwandel -lc++ -lz -lSmallFBX ./src/cli.cpp -o ./bin/wandel

build: build-wandel-lib build-wandel-cli

clean-smallfbx:
	rm -rf ./dep/SmallFBX/build

clean:
	rm -rf ./tmp ./bin

clean-all: clean-smallfbx clean 

run: build
	bin/./wandel