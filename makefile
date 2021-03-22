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

build-wandel-lib: setup
	clang++ --std=c++17 $(DEBUG_FLAG) -c -I../src -I./src ./src/wandel.cpp -o ./tmp/wandel.o
	ar rc ./bin/libwandel.a ./tmp/wandel.o

build-wandel-cli: build-wandel-lib
	clang++ --std=c++17 $(DEBUG_FLAG) -I./src -L./bin -L../build/SmallFBX -lwandel -lc++ -lz -lSmallFBX ./src/cli.cpp -o ./bin/wandel

build: build-wandel-lib build-wandel-cli

clean:
	rm -rf ./tmp ./bin

run: build
	bin/./wandel