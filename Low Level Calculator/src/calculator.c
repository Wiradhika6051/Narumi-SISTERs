#include <stdio.h>

#define bool int
#define true 1
#define false 0
#define BUFFER_SIZE 256
#define MAX_INT 0xffffffff

int solve(char* buffer);//menyelesaikan soal
bool Equal(int num1,int num2);//mengembalikan true apabila num1==num2, selain itu false
bool NotEqual(int num1,int num2);//mengembalikan true apabila num1!=num2, selain itu false
bool Not(int num);//logical not -> !0xf==0, !0x0==1,!0xd==0
bool Greater(int num1,int num2);//num1>num2 == true, selain itu false
bool GreaterEqual(int num1,int num2);//num1 >=num2 == true, selain itu false;
int add(int num1,int num2);//num1+num2

int main(){
    char buffer[BUFFER_SIZE];
    char response;

    while(true){
        fgets(buffer,BUFFER_SIZE,stdin);
        printf("Hasil Operasi: %d\n",solve(buffer));
        printf("Apakah Anda ingin melanjutkan perhitungan?(Y/N)");
        response = getchar();
        if(Equal(response,'N')|Equal(response,'n')){
            break;
        }
        else{
            while  (NotEqual(response = getchar(),'\n') & NotEqual(response,EOF) ) { }
        }
    }
    return 0;
}
int solve(char* buffer){
    int result;
    int op1=0,op2=0;
    int i =0;
    char operator='\0';
    //operator yang valid: +(add),-(substract),*(multiply),/(integer division),^(pangkat)
    //parse operasinya
    while(NotEqual(buffer[i],0x0)){//selama isi buffernya gak \0
        //skip spasi
        while(Equal(buffer[i],' ')){
            i++;
        }
        //berupa angka 0-9
        if(buffer[i]>=48 && buffer[1]<=57){
            if(operator==NULL){
                op1= (op1*10)+buffer[i];//operand kiri
            }
            else{
                op2 = (10*op2)+buffer[i];//operand kanan
            }
        }
        //berupa operator tambah
        else if(buffer[i]=='+'||buffer[i]=='-'){
            operator='+';
        }
        i++;
    }
    return result;
}
bool Equal(int num1,int num2){
    if(num1^num2){
        //num1^num2 !=0: berarti ada yang beda
        return false;
    }
    return true;
}
bool NotEqual(int num1,int num2){
    return Not(Equal(num1,num2));//num1!=num2
}
bool Not(int num){
    return add((num>>31)|((~num+1)>>31),1);
    //sama aja dengan : (num>>31 | -num>>31)+1
    // OR antara signed bit + dan - bilangan tersebut, kalau salah satunya angka 1, maka hasilnya adalah 0xff.ff yang kalau 
    //ditambah 1 pasti hasilnya 0. Satu satunya cara biar hasilnya gak 0 yakni bila hasil OR nya bukan 0xfffff yang 
    //hanya bisa didapatkan bila bilangannya 0
    //+0: 0x000..0
    //-0: ~0+1 = 0xffff.fff+1 = 0x000..00
    //+0 | -0 = 0 +1 = 0x1
}
bool Greater(int num1,int num2){
    //return true bila num1>num2 (signed)
    int difference = ~(add(~num2,num1));//selisih num1 dan num2
    return ((difference ^ ((num2 ^ num1) & (difference ^ num2)))>>31) & 0x1 ;
}
bool GreaterEqual(int num1,int num2){
    //return true bila num1>num2 ATAU num1==num2
    return Greater(num1,num2) | Equal(num1,num2);
}
int add(int num1,int num2){
    int result = num1 ^ num2;
    int carry = (num1 & num2)<<1;
    if(isEqual(carry,0x0)){
        //rekursifnya kelar
        return result;
    }
    return add(result,carry);
}