#include <stdio.h>

#define bool unsigned char
#define true 1
#define false 0
#define BUFFER_SIZE 256

int solve(char* buffer);

int main(){
    char buffer[BUFFER_SIZE];
    char response;

    while(true){
        fgets(buffer,BUFFER_SIZE,stdin);
        printf("Hasil Operasi: %d\n",solve(buffer));
        printf("Apakah Anda ingin melanjutkan perhitungan?(Y/N)");
        response = getchar();
        if(response=='N'||response=='n'){
            break;
        }
        else{
            while ( (response = getchar()) != '\n' && response != EOF ) { }
        }
    }
    return 0;
}
int solve(char* buffer){
    return 0;
}