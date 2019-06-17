all: build

build:
	mkdir -p bin
	nasm -f elf64 -o bin/forth.o part_2/forthress.asm
	ld -o bin/forth bin/forth.o

clean:
	rm -rf bin/
