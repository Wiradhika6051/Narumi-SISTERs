section     .data
    STDOUT equ 1 
    STDIN equ 2
    SUBSTITUTION db '{}' ;substr yg akan disubstitusi
    intro     db  'Selamat datang di kripikpasta generator!',0xa,0xd ;intro message
    askTemplate db 'Masukkan path ke file berisi template copypasta: ';perintah masukkan nama file template
    lenAsk equ $ - intro
    maxStringSize equ 100
    maxBufferSize equ 1000
    endlText db 0xa,0xd,0x0
    lenendl equ $ - endlText
    showTemplate db 'Template yang dibaca:',0xa,0xd
    lenShowTemplate equ $ - showTemplate
    askTemplateObject db 'Masukkan objek dari template ini: '
    lenAskTemplateObject equ $ - askTemplateObject

section .bss
    filename resb maxStringSize ;buffer ke nama dari path ke file, max 100 kbytes
    fd_in resd 1  ;file descriptor
    bufferText resb maxBufferSize
    objectName resb maxStringSize

section     .text
    global      _start
_start:
    ;print selamat datang dan meminta nama template
    mov edx, lenAsk 
    mov ecx, intro 
    call print
    ;baca nama file
    mov ecx,filename
    mov edx,maxStringSize
    call readText
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
    ;baca isi file
    mov ebx, [fd_in]
    mov ecx, bufferText
    mov edx,maxBufferSize
    call readFile
    ;tutup file
    mov eax,6
    mov ebx, [fd_in]
    int 0x80
    ;kasih garis kosong sebelum nampilin template
    call endl
    ;tampilin template:
    mov ecx, showTemplate
    mov edx, lenShowTemplate
    call print
    ;cetak templet
    mov ecx,bufferText
    mov edx,maxBufferSize
    call print
    ;pindah ke garis baru
    call endl
    call endl
    ;minta masukkan karakter yang dijadiin objek template
    mov ecx,askTemplateObject
    mov edx,lenAskTemplateObject
    call print
    ;minta masukan
    mov ecx, objectName
    mov edx,maxStringSize
    call readText
    ;debug, cetak namanya
    ;    mov ecx, objectName
    ;mov edx,maxStringSize
    ;call print
    ;substitusi tanda SUBSTITUTION dengan nama di objectName
    ;exit()
    mov     ebx,0
    mov     eax,1
    int     0x80

endl:
  mov ecx,endlText
  mov edx, lenendl
  call print
  ret

print:
  ;edx ->panjang string yang ingin dicetak
  ;ecx->string yang ingin dicetak
  mov eax, 4;syscall number
  mov ebx, STDOUT; tempat nyetak
  int 0x80;buat syscall
  ret
read:
  ;edx->jumlah yang pingin dibaca
  ;ecx->buffer tempat nyimpan yang dibaca
  ;ebx->sumber bacaan
  mov eax,3
  int 0x80
  ret
readText:
  mov ebx, STDIN
  call read
  ret
readFile:
  ;ebx->file descriptor
  call read
  ret

;template/wangy_wangy.txt
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