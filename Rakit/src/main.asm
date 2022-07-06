section     .data
    STDOUT equ 1 
    STDIN equ 2
    SUBSTITUTION db '{}',0 ;substr yg akan disubstitusi
    intro     db  'Selamat datang di kripikpasta generator!',0xa,0xd ;intro message
    askTemplate db 'Masukkan path ke file berisi template copypasta: ';perintah masukkan nama file template
    lenAsk equ $ - intro
    maxStringSize equ 200
    maxBufferSize equ 2048
    endlText db 0xa,0xd,0x0
    lenendl equ $ - endlText
    showTemplate db 'Template yang dibaca:',0xa,0xd
    lenShowTemplate equ $ - showTemplate
    askTemplateObject db 'Masukkan objek dari template ini: '
    lenAskTemplateObject equ $ - askTemplateObject
    ;i dw ? ;buat iterasi template
    ;j dw ? ;buat iterasi result

section .bss
    filename resb maxStringSize ;buffer ke nama dari path ke file, max 100 kbytes
    fd_in resd 1  ;file descriptor
    bufferText resb maxBufferSize
    objectName resb maxStringSize
    result resb maxBufferSize
    ;i resd 1
    ;j resd 1

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
    ;tambahin \0
    mov byte [ecx+eax-1], 0
    ;debug, cetak namanya
    ;    mov ecx, objectName
    ;mov edx,maxStringSize
    ;call print
    ;substitusi tanda SUBSTITUTION dengan nama di objectName
    call subTemplate
    ;cetak hasil substitus
    mov ecx,result
    mov edx,maxBufferSize
    call print
    ;cetak endl
    call endl
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
subTemplate:
  ;inisialisasi i sama j
  ;i: kounter di template, pake ecx
  ;j: kounter di result, pake edx
  mov ecx, bufferText
  mov edx, result
;inisialisasi pembacaan
  mov esi, ecx ;baca template
  mov edi, SUBSTITUTION ; yang mau disubstitusi
search: ;bandingkan char
  mov al, [edi];simpan char yg diperiksa di pola substitusi
  cmp al,0 ; kalau 0 berarti ketemu yang match
  je match
  ;cek apakah udah diperiksa semua di template
  cmp BYTE [esi],0
  je finish
  ;periksa yg di template sama di pola  
   cmp  [esi], al        
   jne  mismatch;ada yg beda
   inc  esi;inkremen kounter di template
   inc  edi ;inkremen kounter di pola
   jmp  search  ;periksa karakter berikutnya
match:
;ketika patternnya ketemu
  ;simpan nilai i
   mov  ecx, esi           
   dec  ecx              
   mov  edi, objectName ;assignedi dengan karakter di objectName
replace:
   mov eax, [edi]  ;simpan karakter esi objectName
   cmp  eax, 0  ;periksa apakah sudah disubstitusi semua?
   je   next ; kalau iya, lanjut ke pemeriksaan di template
   mov  esi, edx  ;assign esi dengan nilai indeks j di result
   mov  [esi], eax  ;salin nilai di al di posisi yang tepat di result
   inc  edx  ;inkemren j
   inc  edi;inkremen kounter objectName
   jmp  replace;ulangi proses replace
mismatch:    
;kalau ada yg gak pas, ya..periksa next char di template dan ulangi pattern
   mov  esi, ecx ;assign esi dengan indeks di template
   mov  edi, edx ;assign di dengan indeks di result
   mov  eax, [esi] ;pindahin karakter di template ke al
   mov  [edi], eax  ;pindahin karakter dari al ke result
   inc  edx ;inkremen  indeks di result
next:
   mov  edi, SUBSTITUTION ;periksa karakter selanjutnya di template. reset pembacaan pattern
   inc  ecx  ;inkremen kounter di template
;periksa lagi apakah next char nya merupakan karakter terakhir di template
   mov  esi, ecx ;assign esi dengan indeks di template
   cmp  byte [esi], 0 ;bandingkan nilai di indeks i, apakah \0?
   jne  search ; kalo bukan lanjutkan pencarian
finish:
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