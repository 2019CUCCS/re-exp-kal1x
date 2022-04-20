# 逆向工程第二次实验 9.23

## 先验知识

1. 除了入口函数（main函数和回调函数）之外，所有的函数都会被调用，如果不被调用，这个函数在编辑阶段，就会被排除在外。也就是说，一个函数的地址，必然出现在这一段汇编指令的某条或者某几条call指令之后


2. call指令记录下一条指令位置的方式，对不同类型的call是不一样的。汇编里面的call指令，对应多条不同的机器指令。根据跳转的目标地址在不在目前CS所指的段内，分为段间（FAR）和段内（NEAR）；根据目标地址的寻址方式不同，分为直接寻址（直接在指令里面写目标地址）和间接寻址（在寄存器或内存中存储目标地址）；对于段内调转，目标地址的格式也有2种可能：可以写目标地址IP的绝对数值，也可以写相对下一个指令的相对偏移。汇编器会在生成机器码的时候，帮我们选择一种可行的方式，因此我们在写汇编的时候，这几种可能可以都用一个call指令代替。


```asm
CALL BX             ;子程序地址由BX给出
CALL WORD PTR[SI]   ;子程序地址在存储器中
```

3. 汇编语言中call和ret指令不一定成对出现

> call 指令功能是： 将下条指令的地址值压到栈顶上，然后将PC值设置为操作数
> ret指令功能是：将栈顶值弹出到PC上，就完事了

所以在之后编写的代码中，需要加入

```py
def findPos(addr):
    addr += ':'
    for i in range(0, len(asm)):
        pos = asm[i].find(addr)
        if pos != -1:
            return i 
        elif '004039FB:' in asm[i]:
            return i
    return -1
```

用来返回进程（函数）结束时的地址


## 实验要求

找出一个exe文件中有多少个函数，并且知道这些函数的位置：
1. 已知，vs的dumpbin工具，可以获得一个exe文件的全部反汇编指令（/disasm）,Linux下的Objdump可以实现类似功能。
   - 问题：那么是否可以编写一段程序（python），获得一个exe文件中有多少个函数，以及每个函数的地址？
2. 将每个函数的汇编指令，单独保存为一个文本文件。
   - 问题：main函数在哪儿？
    **注：在dumpbin /headers 命令的结果中，可以看到EntryPoint**

---

## 实验过程

1. 使用objdump工具导出 `.text` 文件内容
   
```bash
objdump -d 'C:\Windows\SysWOW64\PING.EXE' > 'ping_disasm.txt'
```

注：需要以管理员权限打开

- [ping_disasm.txt](./ping_disasm.txt)

1. 编写脚本

注：`call esi` 这种call与寄存器的组合也算`段内间接调用`,但是此处无法获取到具体地址，所以在脚本中并未取出

```py
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
```

结果链接：[result.txt](./result.txt)

3. 查找函数入口点

- 方法一
```bash
dumpbin /headers C:/Windows/SysWOW64/PING.EXE | findstr entry

# 结果
 3450 entry point (00403450)
```
- 方法二
```bash
objdump -f 'C:\Windows\SysWOW64\PING.EXE'

# 结果
C:\Windows\SysWOW64\PING.EXE:     file format pei-i386
architecture: i386, flags 0x0000012f:
HAS_RELOC, EXEC_P, HAS_LINENO, HAS_DEBUG, HAS_LOCALS, D_PAGED
start address 0x00403450
```

函数的入口点为 ： **0x00403450**


---

## 参考资料
- [What does "DS:[40207A]" mean in assembly?](https://stackoverflow.com/questions/3819699/what-does-ds40207a-mean-in-assembly)
- [8086的寻址方式和指令系统](http://home.ustc.edu.cn/~ytwang/3_5.pdf)
- [汇编语言中call和ret指令必须成对出现吗?
](https://www.zhihu.com/question/48549704)
