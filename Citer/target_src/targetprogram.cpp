#include <iostream>
#include <stdio.h>
#include <string.h>
int main() {
	//int buf = 0xDEADBEEF;
	//char buf = 'A';
	char buf[100];
	memset(buf,0x0,100);
	strcpy(buf,"I am safe now!");
	printf("Target value : %s\nTarget address : 0x%p\nInput any number to continue\n", buf,&buf);
	int prompt_buf;
	std::cin >> prompt_buf;
	printf("aman\n");
	printf("Target value : %s\n", buf);
	std::cin>>prompt_buf;
	return 0;
}