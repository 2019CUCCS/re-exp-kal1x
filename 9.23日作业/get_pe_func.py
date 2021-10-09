import re 
import os

all_address = []
direct = []
indirect = []
count = 0
path = './func'

try: 
    os.makedirs(path)
except OSError:
    if not os.path.isdir(path):
        raise

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

with open(r'c:\\Users\\Eddie Xu\\Desktop\\class\\3.1\\逆向工程\\9.23日作业\\ping_disasm.txt','r') as f:    
    lines = f.readlines()
    for line in lines:
        if 'call' in line:
            # 获取全部的call指令
            count += 1
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
    for index,add in enumerate(direct_address):
        print('| '+str(index+1)+' | '+add+' |')

    get_asm_func(direct_address)
