section     .data
    STDOUT equ 1 
    STDIN equ 2
    intro     db  'Selamat datang di kripikpasta generator!',0xa,0xd ;intro message
    askTemplate db 'Masukkan path ke file berisi template copypasta:',0xa,0xd;perintah masukkan nama file template
    lenAsk equ $ - intro
    maxStringSize equ 100
    maxBufferSize equ 1000

section .bss
    filename resb maxStringSize ;buffer ke nama dari path ke file, max 100 kbytes
    fd_in resd 1  ;file descriptor
    bufferText resb 1000

section     .text
    global      _start
_start:
    ;print selamat datang dan meminta nama template
    mov edx, lenAsk ;panjang string yang ingin dicetak
    mov ecx, intro ;string yang ingin dicetak
    mov ebx,STDOUT ;output place, 1 = stdout
    mov eax,4;interupt number ( 4= nyetak string)
    int 0x80;panggil interrupt
    ;syscall buat baca nama file
    ;mov edx,100 ;maxsize
    mov eax,3   
    mov ebx,STDIN 
    mov ecx,filename
    mov edx,maxStringSize
    int 0x80
    ;cari panjang string
    ;mov ecx,filename;
    ;call strlen;
    ;tambahin \0 di akhir string yang dibaca
    mov byte [ecx+eax-1], 0
    ;open file
    mov eax,5
    mov ebx,filename
    mov ecx, 0 ;read only access
    mov edx,0777h ; read,write execute by all
    int 0x80
    ;simpan file descriptor
    mov [fd_in], eax
    ;baca file syscall read()
    mov eax,3
    mov ebx, [fd_in]
    mov ecx, bufferText
    mov edx,maxBufferSize
    int 0x80
    ;tutup file
    mov eax,6
    mov ebx, [fd_in]
    int 0x80
    ;cetak templet
    mov eax,4
    mov ebx,STDOUT
    mov ecx,bufferText
    mov edx,maxBufferSize
    int 0x80
    ;mov edx,5
    ;mov ecx, filename ; nama file yang ingin dibaca
    ;mov ecx,num
    ;mov ebx,STDIN
    ;mov eax,3 ; sys_read
    ;int 0x80
    ;kasih null terminated ke string
   ; mov ebx,filename
    ;add ebx,ecx
    ;add ebx,eax
  ;  sub ebx, 1
  ;  mov byte [ebx],0 ; ecx-> buffer, eax->length read
  ;  xor ebx,ebx;bersihin register

    ;debug
    ;cetak hasilnya
    ;mov edx, maxStringSize ;panjang string yang ingin dicetak
    ;mov ecx, filename ;string yang ingin dicetak
    ;mov ebx,STDOUT ;output place, 1 = stdout
    ;mov eax,4;interupt number ( 4= nyetak string)
    ;int 0x80;panggil interrupt
  ;  add eax,1
  ;  mov edx, eax ;panjang string yang ingin dicetak
   ; mov ecx, filename ;string yang ingin dicetak
    ;mov ebx,STDOUT ;output place
    ;mov eax,4;interupt number ( 4= nyetak string)
    ;int 0x80;panggil interrupt
    ;exit()
    mov     ebx,0
    mov     eax,1
    int     0x80


;strlen:
;  ;nyari panjang string
;  push ecx
;  dec ecx
;  .loop:
;    inc ecx
;    cmp byte [ecx],0
;    jne .loop
;    ;hitung panjangnya
;    pop eax
;    sub ecx,eax
;    mov eax,ecx
;    ret
;    ;return panjangnya di eax