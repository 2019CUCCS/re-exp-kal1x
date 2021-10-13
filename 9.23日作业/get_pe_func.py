import re 
import os

all_address = []
direct = []
indirect = []
asm = []
count = 0
path = './func'

try: 
    os.makedirs(path)
except OSError:
    if not os.path.isdir(path):
        raise

with open(r'c:\\Users\\Eddie Xu\\Desktop\\class\\3.1\\逆向工程\\9.23日作业\\ping_disasm.txt','r') as f:  
    asm = f.readlines()

def findPos(addr):
    addr += ':'
    for i in range(0, len(asm)):
        pos = asm[i].find(addr)
        if pos != -1:
            return i 
        elif '004039FB:' in asm[i]:
            return i

    return -1

def get_asm_func(address): 
    flag=0
    for i in address:
        result = open('./funcs/'+i+'.txt','w')
        result.write('----------------\n')
        result.write('处理的内存地址'+i+'\n')
        pattern = re.compile(i+':')
        with open(r'c:\\Users\\Eddie Xu\\Desktop\\class\\3.1\\逆向工程\\9.23日作业\\ping_disasm.txt','r') as f:  
            lines = f.readlines()
            for line in lines:
                start = re.findall(pattern, line)
                if start:
                    flag=1
                if flag:    
                    result.write(line)
                    end = re.findall('ret',line)
                    if end:
                        flag=0
                        break
                    elif '004039FB' in line:
                        flag=0
                        break
                    else:
                        continue
                else:
                    continue
            result.write('----------------\n')
        result.close()


def get_func_asm(address_list):
    for i in range(0, len(address_list)):
        if i != len(address_list) - 1:  # i在没有到最后一行前
            start = findPos(address_list[i])  # 第i个地址
            end = findPos(address_list[i + 1])  # 第i+1个地址
            with open(os.path.join(path, address_list[i] + ".txt"), 'w') as f:
                f.write("".join(asm[start:end]))
        else:
            start = findPos(address_list[i])
            with open(os.path.join(path, address_list[i] + ".txt"), 'w') as f:
                f.write("\n".join(asm[start:len(asm)-9]))

def get_address():
    with open(r'c:\\Users\\Eddie Xu\\Desktop\\class\\3.1\\逆向工程\\9.23日作业\\ping_disasm.txt','r') as f:    
        lines = f.readlines()
        for line in lines:
            if 'call' in line:
                # 获取全部的call指令
                all_address.append(line)
                # 获取段内间接调用
                indirect_pattern = re.compile(r'ds:\[(.*)\]')
                indirect_addr = re.search(indirect_pattern,line)
                if indirect_addr:
                    indirect.append(indirect_addr.group(1))
                #获取段内直接调用
                direct_pattern = re.compile(r'call        ([\dA-Z]{8})')
                direct_addr = re.search(direct_pattern,line)
                if direct_addr:
                    direct.append(direct_addr.group(1))   
        
        print('---------------------------------')
        print('段内间接调用情况:')
        print('|个数|函数地址|')
        indirect_address = list(set(indirect))
        for index,add in enumerate(indirect_address):
            print('| '+str(index+1)+' | '+add+' |')
        print('---------------------------------')
        print('段内直接调用情况:')
        print('|个数|函数地址|')
        direct_address = list(set(direct))
        # direct_address.append('00403450')
        direct_address = sorted(direct_address)
        for index,add in enumerate(direct_address):
            print('| '+str(index+1)+' | '+add+' |')
    return direct_address      

def main():
    result = get_address()
    get_func_asm(result)


if __name__ == '__main__':
    main()
