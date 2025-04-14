GCC = zig c++

run:build

build:
	$(GCC) ./src/*.cpp -lz3
