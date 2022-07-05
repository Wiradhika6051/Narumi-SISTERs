compile:	
	nasm -f elf32 ./src/main.asm -o ./bin/main.o;
	ld -m elf_i386 -s -o ./bin/main ./bin/main.o;
run:
	./bin/main
all: compile run
templet:
	nasm -f elf32 ./src/templet.asm -o ./templet.o;
	ld -m elf_i386 -s -o ./templet ./templet.o;