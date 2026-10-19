all: bin/asm bin/emu
bin:
	mkdir bin
bin/asm: assembler/src/*.cpp assembler/include/*.h | bin
	clang++ -std=c++20 assembler/src/*.cpp -o bin/asm
bin/emu: emulator/src/*.cpp emulator/include/*.h | bin
	clang++ emulator/src/*.cpp -I /opt/homebrew/include -L /opt/homebrew/lib -lsdl3 -o bin/emu
