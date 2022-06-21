compile:	
	nasm -f elf32 ./src/main.asm -o ./bin/main.o;
	ld -m elf_i386 -s -o ./bin/main ./bin/main.o;
run:
	./bin/main
all: compile run