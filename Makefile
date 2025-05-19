GCC = zig c++
OUT = gen.exe

run:build
	./$(OUT) ./tests/datatype.smt2

build:
	$(GCC) ./src/*.cpp -lz3 -o $(OUT)

