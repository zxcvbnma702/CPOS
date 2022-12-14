参考资料



## 关于x86内存分配

![image.png](https://s2.loli.net/2022/08/20/qbXits7dkPEnhCN.png)

## 关于AT&T

[AT&T简明语法](https://blog.csdn.net/qq_53144843/article/details/120346586?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522165978364016781685355542%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=165978364016781685355542&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~pc_rank_34-1-120346586-null-null.142^v39^pc_rank_34_1,185^v2^control&utm_term=at%26t%E6%B1%87%E7%BC%96%E6%B3%A8%E9%87%8A&spm=1018.2226.3001.4187)

## 关于启动引导

使用grud进行操作系统启动引导，详见 [grud及multiboot规范](https://blog.csdn.net/wuhui_gdnt/category_856724.html)

关于纯intel汇编进行启动引导，详见另一篇笔记

## 关于中断

[中断](https://blog.csdn.net/Wang_Dou_Dou_/article/details/122037133?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166080074516782350886034%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=166080074516782350886034&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~pc_rank_34-8-122037133-null-null.142^v41^pc_rank_34,185^v2^control&utm_term=8259A&spm=1018.2226.3001.4187)

    1.初始化8259A：为设备分配中断号。

    2.建立ITD表

    2.1 定位ITD：通过ITDR把ITD描述符表写进内存（定义位置和大小）。

    2.2 新建ITD描述符：映射设备中断号和中断服务程序地址之间的关系。

    3.编写中断服务程序：这是中断发起的最终归宿，程序需放在应用代码段之内。

## 关于8259A

[8259A芯片使用](https://blog.csdn.net/longintchar/article/details/79439466)

## 关于键盘中断处理

#### 键盘硬件概述

>对于驱动来说，和键盘相关的最重要的硬件是两个芯片。一个是 intel 8042 芯片，位于主板上，CPU 通过 IO 端口直接和这个芯片通信，获得按键的扫描码或者发送各种键盘命令。
另一个是 intel 8048 芯片或者其兼容芯片，位于键盘中，这个芯片主要作用是从键盘的硬件中得到被按的键所产生的扫描码，与 i8042 通信，控制键盘本身。

>当键盘上有键被按下时，i8048 直接获得键盘硬件产生的扫描码。i8048 也负责键盘本身的控制，比如点亮 LED 指示灯，熄灭 LED 指示灯。i8048 通过 ps/2 口和 i8042 通信，
把得到的扫描码传送给 i8042。CPU 通过读写端口，可以直接把 i8042 中的数据读入到 CPU 的寄存器中，或者把 CPU 寄存器中的数据写入 i8042 中。ps/2 口一共有 6 个引脚，可以拔下 ps/2 插头看一下，这 6 个引脚分别为，时钟，数据，电源地，电源+5V，还有2个引脚没有被用到。由于只有一个引脚传输数据，所以 ps/2 口上的数据传输是串行的。

>键盘驱动直接读写 i8042 芯片，通过 i8042 间接的向键盘中的 i8048 发命令。所以对于驱动来说，直接发生联系的只有 i8042 ，因此我们只介绍 i8042 ，不介绍 i8048。

>i8042 有 4 个 8 bits 的寄存器，他们是 Status Register（状态寄存器），Output Buffer（输出缓冲器），Input Buffer（输入缓冲器），Control Register（控制寄存器）。使用两个 IO 端口，60h 和 64h。

#### Status Register（状态寄存器）

状态寄存器是一个8位只读寄存器，任何时刻均可被cpu读取。其各位定义如下

* Bit7: PARITY-EVEN(P_E): 从键盘获得的数据奇偶校验错误

* Bit6: RCV-TMOUT(R_T): 接收超时，置1

* Bit5: TRANS_TMOUT(T_T): 发送超时，置1

* Bit4: KYBD_INH(K_I): 为1，键盘没有被禁止。为0，键盘被禁止。

* Bit3: CMD_DATA(C_D): 为1，输入缓冲器中的内容为命令，为0，输入缓冲器中的内容为数据。

* Bit2: SYS_FLAG(S_F): 系统标志，加电启动置0，自检通过后置1

* Bit1: INPUT_BUF_FULL(I_B_F): 输入缓冲器满置1，i8042 取走后置0

* BitO: OUT_BUF_FULL(O_B_F): 输出缓冲器满置1，CPU读取后置0

#### Output Buffer（输出缓冲器)
输出缓冲器是一个8位只读寄存器。驱动从这个寄存器中读取数据。这些数据包括，扫描码，发往 i8042 命令的响应，间接的发往 i8048 命令的响应。

#### Input Buffer（输入缓冲器）
输入缓冲器是一个8位只写寄存器。缓冲驱动发来的内容。这些内容包括，发往 i8042 的命令，通过 i8042 间接发往 i8048 的命令，以及作为命令参数的数据。

#### Control Register（控制寄存器）
也被称作 Controller Command Byte （控制器命令字节）。其各位定义如下

* Bit7: 保留，应该为0
* Bit6: 将第二套扫描码翻译为第一套
* Bit5: 置1，禁止鼠标
* Bit4: 置1，禁止键盘
* Bit3: 置1，忽略状态寄存器中的 Bit4
* Bit2: 设置状态寄存器中的 Bit2
* Bit1: 置1，enable 鼠标中断
* BitO: 置1，enable 键盘中断


>驱动中把 0x60 叫数据端口
>驱动中把 0x64 叫命令端口



#### 发给i8042的命令

驱动对键盘控制器发送命令是通过写端口64h实现的，共有12条命令，分别为

* 20h
准备读取8042芯片的Command Byte；其行为是将当前8042 Command Byte的内容放置于Output Register中，下一个从60H端口的读操作将会将其读取出来。

* 60h
准备写入8042芯片的Command Byte；下一个通过60h写入的字节将会被放入Command Byte。

* A4h
测试一下键盘密码是否被设置；测试结果放置在Output Register，然后可以通过60h读取出来。测试结果可以有两种值：FAh=密码被设置；F1h=没有密码。

* A5h
设置键盘密码。其结果被按照顺序通过60h端口一个一个被放置在Input Register中。密码的最后是一个空字节（内容为0）。

* A6h
让密码生效。在发布这个命令之前，必须首先使用A5h命令设置密码。

* AAh
自检。诊断结果放置在Output Register中，可以通过60h读取。55h=OK。

* ADh
禁止键盘接口。Command Byte的bit-4被设置。当此命令被发布后，Keyboard将被禁止发送数据到Output Register。

* AEh
打开键盘接口。Command Byte的bit-4被清除。当此命令被发布后，Keyboard将被允许发送数据到Output Register。

* C0h
准备读取Input Port。Input Port的内容被放置于Output Register中，随后可以通过60h端口读取。

* D0h
准备读取Outport端口。结果被放在Output Register中，随后通过60h端口读取出来。

* D1h
准备写Output端口。随后通过60h端口写入的字节，会被放置在Output Port中。

* D2h
准备写数据到Output Register中。随后通过60h写入到Input Register的字节会被放入到Output Register中，此功能被用来模拟来自于Keyboard发送的数据。如果中断被允许，则会触发一个中断。

#### 发给i8048的命令

共有10条命令，分别为

* EDh
设置LED。Keyboard收到此命令后，一个LED设置会话开始。Keyboard首先回复一个ACK（FAh），然后等待从60h端口写入的LED设置字节，如果等到一个，则再次回复一个ACK，然后根据此字节设置LED。然后接着等待。。。直到等到一个非LED设置字节(高位被设置)，此时LED设置会话结束。

* EEh
诊断Echo。此命令纯粹为了检测Keyboard是否正常，如果正常，当Keyboard收到此命令后，将会回复一个EEh字节。

* F0h
选择Scan code set。Keyboard系统共可能有3个Scan code set。当Keyboard收到此命令后，将回复一个ACK，然后等待一个来自于60h端口的Scan code set代码。系统必须在此命令之后发送给Keyboard一个Scan code set代码。当Keyboard收到此代码后，将再次回复一个ACK，然后将Scan code set设置为收到的Scan code set代码所要求的。

* F2h
读取Keyboard ID。由于8042芯片后不仅仅能够接Keyboard。此命令是为了读取8042后所接的设备ID。设备ID为2个字节，Keyboard ID为83ABh。当键盘收到此命令后，会首先回复一个ACK，然后，将2字节的Keyboard ID一个一个回复回去。

* F3h
设置Typematic Rate/Delay。当Keyboard收到此命令后，将回复一个ACK。然后等待来自于60h的设置字节。一旦收到，将回复一个ACK，然后将Keyboard Rate/Delay设置为相应的值。

* F4h
清理键盘的Output Buffer。一旦Keyboard收到此命令，将会将Output buffer清空，然后回复一个ACK。然后继续接受Keyboard的击键。

* F5h
设置默认状态(w/Disable)。一旦Keyboard收到此命令，将会将Keyboard完全初始化成默认状态。之前所有对它的设置都将失效——Output buffer被清空，Typematic Rate/Delay被设置成默认值。然后回复一个ACK，接着等待下一个命令。需要注意的是，这个命令被执行后，键盘的击键接受是禁止的。如果想让键盘接受击键输入，必须Enable Keyboard。

* F6h
设置默认状态。和F5命令唯一不同的是，当此命令被执行之后，键盘的击键接收是允许的。

* FEh
Resend。如果Keyboard收到此命令，则必须将刚才发送到8042 Output Register中的数据重新发送一遍。当系统检测到一个来自于Keyboard的错误之后，可以使用自命令让Keyboard重新发送刚才发送的字节。

* FFh
Reset Keyboard。如果Keyboard收到此命令，则首先回复一个ACK，然后启动自身的Reset程序，并进行自身基本正确性检测（BAT-Basic Assurance Test）。等这一切结束之后，将返回给系统一个单字节的结束码（AAh=Success, FCh=Failed），并将键盘的Scan code set设置为2。

#### 读到的数据

* 00h/FFh
当击键或释放键时检测到错误时，则在Output Bufer后放入此字节，如果Output Buffer已满，则会将Output Buffer的最后一个字节替代为此字节。使用Scan code set 1时使用00h，Scan code 2和Scan Code 3使用FFh。

* AAh
BAT完成代码。如果键盘检测成功，则会将此字节发送到8042 Output Register中。

* EEh
Echo响应。Keyboard使用EEh响应从60h发来的Echo请求。

* F0h
在Scan code set 2和Scan code set 3中，被用作Break Code的前缀。

* FAh
ACK。当Keyboard任何时候收到一个来自于60h端口的合法命令或合法数据之后，都回复一个FAh。

* FCh
BAT失败代码。如果键盘检测失败，则会将此字节发送到8042 Output Register中。

* FEh
Resend。当Keyboard任何时候收到一个来自于60h端口的非法命令或非法数据之后，或者数据的奇偶交验错误，都回复一个FEh，要求系统重新发送相关命令或数据。

* 83ABh
当键盘收到一个来自于60h的F2h命令之后，会依次回复83h，ABh。83AB是键盘的ID。

Scan code
除了上述那些特殊字节以外，剩下的都是Scan code。

#### 端口操作
>首先介绍一下端口的读写操作，驱动中使用函数 READ_PORT_UCHAR 进行读操作，READ_PORT_UCHAR 中使用CPU读端口指令，in。驱动中使用函数 WRITE_PORT_UCHAR 进行写操作，WRITE_PORT_UCHAR 中使用CPU写端口指令，out。

#### 读取状态寄存器

读取状态寄存器的方法，对64h端口进行读操作。

#### 读数据

需要读取的数据有，i8042从i8048得到的按键的扫描码，i8042命令的ACK，i8042从i8048得到的i8048命令的ACK，需要命令重发的RESEND，一些需要返回结果的命令得到的结果。

当有数据需要被驱动读走的时候，数据被放入输出缓冲器，同时将状态寄存器的bit0（OUTPUT_BUFFER_FULL）置1，引发键盘中断（键盘中断的IRQ为1）。由于键盘中断，引起由键盘驱动提供的键盘中断服务例程被执行。在键盘中断服务例程中，驱动会从i8042读走数据。一旦数据读取完成，状态寄存器的bit0会被清0。

读数据的方法，首先，读取状态寄存器，判断bit0，状态寄存器bit0为1，说明输出缓冲器中有数据。保证状态寄存器bit0为1，然后对60h端口进行读操作，读取数据。

这里我们要谈一点很有用的题外话，前面提到的 IRQ，是 Interrupt Request line，中断请求线，是一个硬件线，它和中断向量是不同的。中断向量是用来在中断描述符表(IDT)中查找中断服务例程的那个序号。键盘的 IRQ 是1，键盘中断服务例程对应的中断向量可不是1。这点要弄清楚。

#### 向i8042发命令

当命令被发往i8042的时候，命令被放入输入缓冲器，同时引起状态寄存器的 Bit1 置1，表示输入缓冲器满，同时引起状态寄存器的 Bit2 置1，表示写入输入缓冲器的是一个命令。

向i8042发命令的方法，首先，读取状态寄存器，判断bit1，状态寄存器bit1为0，说明输入缓冲器为空，可以写入。保证状态寄存器bit1为0，然后对64h端口进行写操作，写入命令。

#### 间接向i8048发命令

向i8042发这些命令，i8042会转发i8048，命令被放入输入缓冲器，同时引起状态寄存器的Bit1 置1，表示输入缓冲器满，同时引起状态寄存器的 Bit2 置1，表示写入输入缓冲器的是一个命令。这里我们要注意，向i8048发命令，是通过写60h端口，而后面发命令的参数，也是写60h端口。i8042如何判断输入缓冲器中的内容是命令还是参数呢，我们在后面发命令的参数中一起讨论。

向i8048发命令的方法，首先，读取状态寄存器，判断bit1,状态寄存器bit1为0，说明输入缓冲器为空，可以写入。保证状态寄存器bit1为0，然后对60h端口进行写操作，写入命令。

#### 发命令的参数

某些命令需要参数，我们在发送命令之后，发送它的参数，参数被放入输入缓冲器，同时引起状态寄存器的Bit1 置1，表示输入缓冲器满。这里我们要注意，向i8048发命令，是通过写60h端口，发命令的参数，也是写60h端口。i8042如何判断输入缓冲器中的内容是命令还是参数呢。i8042是这样判断的，如果当前状态寄存器的Bit3 为1，表示之前已经写入了一个命令，那么现在通过写60h端口放入输入缓冲器中的内容，就被当做之前命令的参数，并且引起状态寄存器的 Bit3 置0。如果当前状态寄存器的 Bit3 为0，表示之前没有写入命令，那么现在通过写60h端口放入输入缓冲器中的内容，就被当做一个间接发往i8048的命令，并且引起状态寄存器的 Bit3 置1。

向i8048发参数的方法，首先，读取状态寄存器，判断bit1,状态寄存器bit1为0，说明输入缓冲器为空，可以写入。保证状态寄存器bit1为0，然后对60h端口进行写操作，写入参数。

## 关于鼠标中断处理

[详见](http://icodeguru.com/Embedded/asm/Appendix/App32-3.html)

## 关于外部设备互联总线

[详见](https://wiki.osdev.org/PCI)

![image.png](https://s2.loli.net/2022/08/24/nPlxVruIHaqDoTh.png)
![image.png](https://s2.loli.net/2022/08/24/TziS7xjbnPA85ef.png)
![image.png](https://s2.loli.net/2022/08/24/qjPVAERwDOBxZsy.png)

### PCI设备扫描方法
![image.png](https://s2.loli.net/2022/08/24/hna5j41vLTwZKIk.png)

### 关于ATA

28 bit PIO
Assume you have a sectorcount byte and a 28 bit LBA value. A sectorcount of 0 means 256 sectors = 128K.

Notes: When you send a command byte and the RDY bit of the Status Registers is clear, you may have to wait (technically up to 30 seconds) for the drive to spin up, before DRQ sets. You may also need to ignore ERR and DF the first four times that you read the Status, if you are polling.

An example of a 28 bit LBA PIO mode read on the Primary bus:

- Send 0xE0 for the "master" or 0xF0 for the "slave", ORed with the highest 4 bits of the LBA to port 0x1F6: outb(0x1F6, 0xE0 | (slavebit << 4) | ((LBA >> 24) & 0x0F))
- Send a NULL byte to port 0x1F1, if you like (it is ignored and wastes lots of CPU time): outb(0x1F1, 0x00)
- Send the sectorcount to port 0x1F2: outb(0x1F2, (unsigned char) count)
- Send the low 8 bits of the LBA to port 0x1F3: outb(0x1F3, (unsigned char) LBA))
- Send the next 8 bits of the LBA to port 0x1F4: outb(0x1F4, (unsigned char)(LBA >> 8))
- Send the next 8 bits of the LBA to port 0x1F5: outb(0x1F5, (unsigned char)(LBA >> 16))
- Send the "READ SECTORS" command (0x20) to port 0x1F7: outb(0x1F7, 0x20)
  
Wait for an IRQ or poll.

Transfer 256 16-bit values, a uint16_t at a time, into your buffer from I/O port 0x1F0. (In assembler, REP INSW works well for this.)
Then loop back to waiting for the next IRQ (or poll again -- see next note) for each successive sector.
Note for polling PIO drivers: After transferring the last uint16_t of a PIO data block to the data IO port, give the drive a 400ns delay to reset its DRQ bit (and possibly set BSY again, while emptying/filling its buffer to/from the drive).

Note on the "magic bits" sent to port 0x1f6: Bit 6 (value = 0x40) is the LBA bit. This must be set for either LBA28 or LBA48 transfers. It must be clear for CHS transfers. Bits 7 and 5 are obsolete for current ATA drives, but must be set for backwards compatibility with very old (ATA1) drives.

Writing 28 bit LBA
To write sectors in 28 bit PIO mode, send command "WRITE SECTORS" (0x30) to the Command port. Do not use REP OUTSW to transfer data. There must be a tiny delay between each OUTSW output uint16_t. A jmp $+2 size of delay. Make sure to do a Cache Flush (ATA command 0xE7) after each write command completes.

### 关于

```c
struct multiboot_info {
	u32 flags;
	u32 low_mem;
	u32 high_mem;
	u32 boot_device;
	u32 cmdline;
	u32 mods_count;
	u32 mods_addr;
	struct {
		u32 num;
		u32 size;
		u32 addr;
		u32 shndx;
	} elf_sec;
	unsigned long mmap_length;
	unsigned long mmap_addr;
	unsigned long drives_length;
	unsigned long drives_addr;
	unsigned long config_table;
	unsigned long boot_loader_name;
	unsigned long apm_table;
	unsigned long vbe_control_info;
	unsigned long vbe_mode_info;
	unsigned long vbe_mode;
	unsigned long vbe_interface_seg;
	unsigned long vbe_interface_off;
	unsigned long vbe_interface_len;
};
```
