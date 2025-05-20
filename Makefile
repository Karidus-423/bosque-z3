GCC = zig c++
OUT = gen.exe

build:
	$(GCC) ./src/*.cpp -lz3 -o $(OUT)

run:build
	./$(OUT) ./tests/datatype.smt2
