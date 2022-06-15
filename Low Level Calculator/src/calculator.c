#include <stdio.h>

#define bool int
#define true 1
#define false 0
#define BUFFER_SIZE 256
#define MAX_INT 0xffffffff
#define NIL '\0'

//enum buat ngecek operan mana yang sedang dicek
enum posisi{KIRI,KANAN};

int solve(char* buffer);//menyelesaikan soal
int compute(int op1,char operator,int op2);//evaluasi operasi
bool Equal(int num1,int num2);//mengembalikan true apabila num1==num2, selain itu false
bool NotEqual(int num1,int num2);//mengembalikan true apabila num1!=num2, selain itu false
bool Not(int num);//logical not -> !0xf==0, !0x0==1,!0xd==0
bool Greater(int num1,int num2);//num1>num2 == true, selain itu false
bool GreaterEqual(int num1,int num2);//num1 >=num2 == true, selain itu false
bool Lesser(int num1,int num2);//num1<num2==true, selain itu false
bool LesserEqual(int num1,int num2);//num1 <=num2 == true, selain itu false
int tambah(int num1,int num2);//num1+num2
int kurang(int num1,int num2);//num1-num2
int CharToInteger(char c);

//TODO
int kali(int num1,int num2);//num1*num2

int main(){
    char buffer[BUFFER_SIZE];
    char response;

    while(true){
        printf("Masukkan persamaan yang ingin diselesaikan. Pisahkan antar operand dan operator dengan spasi\n");
        printf("Contoh: \"200 + 30 / -300\"\n");
        printf("Operator yang valid: +,-,*,/,^\n");
        printf("Persamaan:");
        fgets(buffer,BUFFER_SIZE,stdin);
        printf("Hasil Operasi: %d\n",solve(buffer));
        printf("Apakah Anda ingin melanjutkan perhitungan?(Y/N)\n");
        response = getchar();
        if(Equal(response,'N')|Equal(response,'n')){
            goto END;
        }
        else{
            while  (NotEqual(response = getchar(),'\n') & NotEqual(response,EOF) ) { }
        }
    }
    END:
    return 0;
}
int solve(char* buffer){
    int result=0;
    int operandKanan = 0;
    int i =0;
    char operator=NIL;
    enum posisi curPos = KIRI;
    bool isNegatif = false;
    bool isParseStarted = false;//menandakan apakah token pertama sudah mulai dibaca
    bool isNeedProcessed = false;//menandakan apakah perlu dilakukan operasi
  //  bool op1Filled=false,op2Filled=false;//menandakan apakah operand kiri / operand kanan sudah terisi
    //operator yang valid: +(tambah),-(substract),*(multiply),/(integer division),^(pangkat)
    //parse operasinya
    while(NotEqual(buffer[i],0x0)){//selama isi buffernya gak \0
        //skip spasi
        while(Equal(buffer[i],' ')){
            if(isNegatif){
                if(Equal(curPos,KANAN)){
                    operandKanan = kali(operandKanan,-1);
                }
                else{
                    result = kali(result,-1);
                }
                isNegatif = false;
            }
            if(Equal(curPos,KIRI) & isParseStarted){
                //LHS sudah dibaca
                curPos = KANAN;
            }
            if(isNeedProcessed){
                //proses operasi
                result = compute(result,operator,operandKanan);
                isNeedProcessed = false;
                operator = NIL;
                operandKanan = 0;
            }
            i = tambah(i,1);
        }
        isParseStarted = true;
        //berupa angka 0-9
        if(GreaterEqual(buffer[i],48) & LesserEqual(buffer[1],57)){
            if(Equal(curPos,KIRI)){
                result= tambah(kali(result,10),CharToInteger(buffer[i]));//operand kiri
            }
            else{
                isNeedProcessed = true;
                operandKanan= tambah(kali(operandKanan,10),CharToInteger(buffer[i]));//operand kanan
            }
        }
        //ketemu operator - (bisa negatif atau operator pengurangan)
        else if(Equal(buffer[i],'-')){
            if(NotEqual(buffer[i+1],' ')){
                //kalau karakter sesudahnya bukan spasi, maka ini tanda negatif
                //tandai buat dikasih negatif di akhir
                isNegatif = true;
            }
            else{
                //operator pengurangan
                operator= buffer[i];
            }
        }
        //ketemu operator
        else if(Equal(buffer[i],'+') | Equal(buffer[i],'*') | Equal(buffer[i],'/') | Equal(buffer[i],'^')){
            operator= buffer[i];
        }
        i = tambah(i,1);
    }
    if(isNegatif){
        //buat ngehandle kalau token terakhirnya negatif
        if(Equal(curPos,KANAN)){
            operandKanan = kali(operandKanan,-1);
        }
        else{
            result = kali(result,-1);
        }
        isNegatif = false;
    }
    if(isNeedProcessed){
        //periksa operasi terakhir
        result = compute(result,operator,operandKanan);
    }
    return result;
}
int compute(int op1,char operator,int op2){
    switch(operator){
        case '+':
            return tambah(op1,op2);
        case '-':
            return kurang(op1,op2);
        default:
            return 0;
    }
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
//    printf("angka: %d\n",tambah(~num,1));
    return tambah((num>>31)|((tambah(~num,1))>>31),1);
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
    int difference = ~(tambah(~num2,num1));//selisih num1 dan num2
    return ((difference ^ ((num2 ^ num1) & (difference ^ num2)))>>31) & 0x1 ;
}
bool GreaterEqual(int num1,int num2){
    //return true bila num1>num2 ATAU num1==num2
    return Greater(num1,num2) | Equal(num1,num2);
}
bool Lesser(int num1,int num2){
    return Greater(num2,num1);//kalau num2>num1, maka num1< num2 yang menghasilkan true
}
bool LesserEqual(int num1,int num2){
    //return true bila num1<num2 ATAU num1==num2
    return Lesser(num1,num2) | Equal(num1,num2);
}
int tambah(int num1,int num2){
    int result=0;
    int carry = 0;
    int temp;
    result = num1 ^ num2;
    carry = (num1 & num2)<<1;
check:
    if(Equal(carry,0x0)){
        //rekursifnya kelar
        return result;
    }
    temp = result ^ carry;
    carry = (result & carry) <<1;
    result = temp;
    goto check;
}
int kurang(int num1,int num2){
    int result = 0;
    int borrow = 0;
    int temp;
    printf("%d %d\n",num1,num2);
    result = num1 ^ num2;
    borrow = (~num1 & num2) << 1;//borrow bernilai 1 apabila bit di num1 nya 0 tapi bit di num2 nya 1
loopkurang:
    if(Equal(borrow,0x0)){
        //rekursifnya kelar
        return result;
    }
    temp = result ^ borrow;
    borrow =  (~result & borrow) << 1;
    result = temp;
    goto loopkurang;
}
int kali(int num1,int num2){
    return num1*num2;
}
int CharToInteger(char c){
    return c-48;
}