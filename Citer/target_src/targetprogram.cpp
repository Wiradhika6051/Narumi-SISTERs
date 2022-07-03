#include <iostream>
#include <stdio.h>
int main() {
	int buf = 0xDEADBEEF;
	printf("Target value : 0x%x\nTarget address : 0x%p\nInput any number to continue\n", buf,&buf);
	int prompt_buf;
	std::cin >> prompt_buf;
	printf("Target value : 0x%x\n", buf);
	return 0;
}