#include <iostream>
#include <list>
#include <vector>
#include<string>
#include<filesystem>
using namespace std;

int main(){
    //dapetin inputnya, yakni:
    //1. String yang mau dicari, ya inti dari program ini buat nyari string di tumpukan file. (#anjay pas banget peribahasanya)
    //2. starting directory pencarian
    //3.karena File dicari secara multiprocessing, maka perlu dibuat proses tersendiri untuk setiap file. Sebenarnya bisa aja gak dibatasin
    //tapi kalau filenya banyak bisa buanyaak banget entar child processnya yang malah membuat prose pencarian menjadi kurang efisien karena
    //overhead context switch
    //4.Karena string dicari dengan multithreading...ya, dibikin thread. Sama kek kasus file, sebaiknya dibatasin karena kalau terlalu banyak malah overhead pad
    //context switch
    string str,start_dir;
    long num_thread, num_process;
    cin>>str>>start_dir>>num_process>>num_thread;
    //dapetin data semua file di direktori yang ingin dicari
    list<int> listOfFiles;
    for(const auto & file: recursive_directory_iterator(start_dir)){
        cout<<file.path()<<endl;
    }

    return 0;
}