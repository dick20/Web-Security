# MD5算法

软件工程（计算机应用方向）	16340132	梁颖霖

---

## 一. 算法原理概述

> The **MD5 message-digest algorithm** is a widely used [hash function](https://en.wikipedia.org/wiki/Hash_function) producing a 128-[bit](https://en.wikipedia.org/wiki/Bit) hash value. Although MD5 was initially designed to be used as a [cryptographic hash function](https://en.wikipedia.org/wiki/Cryptographic_hash_function), it has been found to suffer from extensive vulnerabilities. 	——Wikipedia

+ MD5 使用 little-endian (小端模式)，输入任意不定长度信息，以512-bit 进行分组，生成四个32-bit 数据，最后联合输出固定128-bit 的信息摘要。
+ MD5 算法的基本过程为：**填充、分块、缓冲区初始化、循环压缩、得出结果。**



**MD5的基本流程图如下**

![1](https://github.com/dick20/Web-Security/blob/master/MD5/image/1.png)

## 二. 总体结构

### 1.填充

在长度为 K bits 的原始消息数据尾部填充长度为 P bits 的标识100…0，1 $\leq$ P $\leq$ 512 (即至少要填充1个bit)，使得填充后的消息位数为：K + P $\equiv$ 448 (mod 512).

+ 注意到当 K $\equiv$ 448 (mod 512) 时，需要 P= 512.

再向上述填充好的消息尾部附加 K 值的低64位 (即 K mod 264)，最后得到一个长度位数为 K + P + 6 $\equiv$ 0 (mod 512) 的消息。

### 2.分块

+ 把填充后的消息结果分割为 L 个 512-bit 分组：$Y_0$, $Y_1$, …, $Y_{L-1}$。
+ 分组结果也可表示成 N 个32-bit 字 $M_0$, $M_1$, …, $M_{N-1}$，N = L*16。

### 3.缓冲区初始化

初始化一个128-bit 的 MD 缓冲区，记为 $CV_q$，表示成4个32-bit寄存器 (A, B, C, D)；$CV_0$ = IV。迭代在 MD 缓冲区进行，最后一步的128-bit 输出即为算法结果。

+ 寄存器 (A, B, C, D) 置16进制初值作为初始向量 IV，并采用小端存储 (little-endian) 的存储结构：
  + A = 0x67452301
  + B = 0xEFCDAB89
  + C = 0x98BADCFE
  + D = 0x10325476

### 4.循环压缩

以512-bit 消息分组为单位，每一分组 Yq (q = 0, 1, …, L-1) 经过4个循环的压缩算法，表示为：

+ $CV_0$ = IV
+ $CV_i$ = $H_{MD5}$($CV_{i-1}$ , ${Y_i}$)
+ 输出结果：MD = $CV_L$ .

## 三. 模块分解

#### a. MD5 压缩函数 HMD5——轮函数

HMD5 从 CV 输入128位，从消息分组输入512位，完成4轮循环后，输出128位，用于下一轮输入的 CV 值。

每轮循环分别固定不同的生成函数 F, G, H, I，结合指定的 T 表元素 T[] 和消息分组的不同部分 X[] 做16次迭代运算，生成下一轮循环的输入。

4轮循环总共有64次迭代运算。

4轮循环中使用的生成函数(轮函数) g 是一个32位非线性逻辑函数，在相应各轮
的定义如下：

| 轮次 | Function g | g(b,c,d)                                       |
| ---- | ---------- | ---------------------------------------------- |
| 1    | F(b, c, d) | (b $\bigwedge$ c) $\bigvee$ (~b $\bigwedge$ d) |
| 2    | G(b, c, d) | ($b \bigwedge d$) $\bigvee$ ($c \bigwedge$ ~d) |
| 3    | H(b, c, d) | b $\bigoplus$ c $\bigoplus$ d                  |
| 4    | I(b, c, d) | c $\bigoplus$ (b $\bigvee$ ~d)                 |

#### b. MD5 压缩函数 HMD5——迭代运算

+ 每轮循环中的一次迭代运算逻辑
  (1) 对 A 迭代：a $\leftarrow$ b + ((a + g(b, c, d) + X[k] + T[i]) <<<s)
  (2) 缓冲区 (A, B, C, D) 作循环轮换：(B, C, D, A) $\leftarrow$ (A, B, C, D) 
+ 说明：
  + a, b, c, d : MD 缓冲区 (A, B, C, D) 的当前值。
  + g : 轮函数 (F, G, H, I 中的一个)。
  + <<<s : 将32位输入循环左移 (CLS) s 位。
  + X[k] : 当前处理消息分组的第 k 个 (k = 0..15) 32位字，即
    $M_{q*16+k}$。
  + T[i] : T 表的第 i 个元素，32位字；T 表总共有64个元素，也
    称为加法常数。
  + +: 模 232 加法

#### c. MD5 压缩函数 HMD5——各轮循环的 X[k]

各轮循环中第 i 次迭代 (i = 1..16) 使用的 X[k] 的确定：
设 j = i -1：

+ 第1轮迭代：k = j.
  + 顺序使用 X[0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15]
+ 第2轮迭代：k = (1 + 5j) mod 16
  + 顺序使用 X[1, 6,11, 0, 5,10,15, 4, 9,14, 3, 8,13, 2, 7,12]
+ 第3轮迭代：k = (5 + 3j) mod 16.
  + 顺序使用 X[5, 8,11,14, 1, 4, 7,10,13, 0, 3, 6, 9,12,15, 2]
+ 第4轮迭代：k = 7j mod 16.
  + 顺序使用 X[0, 7,14, 5,12, 3,10, 1, 8,15, 6,13 , 4,11, 2, 9]

#### d. MD5 压缩函数 HMD5——T表的生成

T[i] = int($ 2^{32} *|sin(i)|$)

+ int 取整函数，sin 正弦函数，以 i 作为弧度输入

各次迭代运算采用的 T 值：

+ T[ 1.. 4] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee }
+ T[ 5.. 8] = { 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 } 
+ T[ 9..12] = { 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be } 
+ T[13..16] = { 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 } 
+ T[17..20] = { 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa } 
+ T[21..24] = { 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 } 
+ T[25..28] = { 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed } 
+ T[29..32] = { 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a } 
+ T[33..36] = { 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c } 
+ T[37..40] = { 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 } 
+ T[41..44] = { 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 } 
+ T[45..48] = { 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 } 
+ T[49..52] = { 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 } 
+ T[53..56] = { 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 } 
+ T[57..60] = { 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 } 
+ T[61..64] = { 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 

#### e. MD5 压缩函数 HMD5——左循环移位的 s 值

各次迭代运算采用的左循环移位的 s 值：

+ s[ 1..16] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22 } 
+ s[17..32] = { 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20 } 
+ s[33..48] = { 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23 } 
+ s[49..64] = { 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 }

## 四. 数据结构

我的MD5算法使用了C++类的设计来实现，主要使用数组来表示循环移位表，unsigned int与unsigned char来表示不同的位数，char为8位，int为32位，在不同的表示函数中各有作用，故需要对其进行转换。

```c++
	unsigned int state[4];		// 四个寄存器，MD缓冲区，共128位
	unsigned int count[2];		// 统计长度，仅保留前64位
	unsigned char buffer[512];	// 输入
	unsigned char digest[128];	// 输出
```

表示state状态时，我使用的是16进制表达64位，这样较简便

```c++
	// 初始化获取IV
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
```

关于S表，则直接用二维数组

```c++
// 左循环移位表
int s[4][4] = {{7,12,17,22},{5,9,14,20},{4,11,16,23},{6,10,15,21}};
```

对于填充的内容，是1000···000，最多512位，最少要有一位，这是根据填充长度来决定的，使用我直接要预设填充长度为512位。

```c++
unsigned char padding[64] = 
{0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
```

表示轮函数，则直接根据T表来进行书写。例如下面

```c++
	H_F(a, b, c, d, x[0], s[0][0], 0xd76aa478);
	H_F(d, a, b, c, x[1], s[0][1], 0xe8c7b756);
	H_F(c, d, a, b, x[2], s[0][2], 0x242070db);
	···
```

关于int与char的转换，是这次算法实现比较关键的部分，因为关于使用byte还是bit取决于函数的需要。以下第一个函数是int转为char，将1个int转为4个char，这里使用位运算+移位的方法，不断取8位转化为char。第二个函数是char转int，也像之前类似，不过是要将4个char转为1个int，这里用到的是移位+或运算，这样就可以得到1个32位int类型。

```c++
void my_md5::int_to_char(unsigned char *output,  const unsigned int  *input, int length){
	for (int i = 0, j = 0; i < length; ++i)
	{
		output[j] = input[i] & 0xff;
		output[j+1] = (input[i] >> 8) & 0xff;
		output[j+2] = (input[i] >> 16) & 0xff;
		output[j+3] = (input[i] >> 24) & 0xff;
		j += 4;
	}
}

void my_md5::char_to_int(unsigned int  *output, const unsigned char *input, int length){
	for (int i = 0, j = 0; i < length; i += 4)
	{
		output[j] = input[i] | (input[i+1] << 8) | (input[i+2] << 16) | (input[i+3] << 24);
		j++;
	}
}
```



## 五. 源代码

### 1.MD5类设计

```c++
class my_md5
{
public:
	my_md5(string& str);
	my_md5(); 
	void update(const unsigned char* input_str, int str_length);
	// 将128位结果，转化为16个byte输出
	void show_result();
    // 对输入的字符串进行MD5加密
	void decode_string(string& str);
	~my_md5();
	
private:
	unsigned int state[4];		// 四个寄存器，MD缓冲区，共128位
	unsigned int count[2];		// 统计长度，仅保留前64位
	unsigned char buffer[512];	// 输入
	unsigned char digest[128];	// 输出
	bool is_padding;
	char result[33];			// 32位的输出结果

	// 私有辅助函数，用以变换
	// 对于其中一个区块做变换
	void transform(unsigned char block[64]);
	// 填充函数，添加长度
	void padding();
	// 以下两个函数用以将32位的unsigned int与8位的unsigned char互相转换输出
	void int_to_char(unsigned char *output,  const unsigned int  *input, int length);
	void char_to_int(unsigned int  *output, const unsigned char *input, int length);
	// 左移函数
	unsigned int shift_left(unsigned int num, int pos);
	// 轮函数
	unsigned int F(unsigned int b, unsigned int c, unsigned int d);
	unsigned int G(unsigned int b, unsigned int c, unsigned int d);
	unsigned int H(unsigned int b, unsigned int c, unsigned int d);
	unsigned int I(unsigned int b, unsigned int c, unsigned int d);
	// 压缩函数
	void H_F(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int Ti);
	void H_G(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int Ti);
	void H_H(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int Ti);
	void H_I(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int Ti);
};
```

该类的**公有函数**包括两个构造函数，带参的构造函数是直接加密一个字符串，而缺省构造函数仅构造对象，需要调用decode_string(string& str);来进行多次加密。

show_result()函数是将128位digest结果转化为32位16进制来进行输出显示。

update()就是更新函数，会在里面调用私有函数transform来进行轮函数变换。

**私有函数**包括transform，里面包含64次迭代运算，分别利用下面的轮函数F, G, H, I与压缩函数H_F, H_G, H_H, H_I .

左移函数shift_left，根据位数来进行循环左移

F, G, H, I都是轮函数，根据MD5算法提供的函数表来进行构造。压缩函数亦然。

### 2.Transform函数变换

由于Transform函数是需要进行对于一个block来进行处理，压缩变换，64次迭代运算。里面所用到的压缩函数，MD缓冲区，轮函数，左移运算，T表。

以下是一次迭代运算的示意图：

![2](https://github.com/dick20/Web-Security/blob/master/MD5/image/2.png)

```c++
unsigned int my_md5::shift_left(unsigned int num, int pos){
	return (num << pos) | (num >> (32 - pos));
}

unsigned int my_md5::F(unsigned int b, unsigned int c, unsigned int d){
	return (b & c) | ((~b) & d);
}  

unsigned int my_md5::G(unsigned int b, unsigned int c, unsigned int d){
	return (b & d) | (c & (~d)); 
}

unsigned int my_md5::H(unsigned int b, unsigned int c, unsigned int d){
	return (b ^ c ^ d);
}

unsigned int my_md5::I(unsigned int b, unsigned int c, unsigned int d){
	return (c ^ (b | ~d));
}

void my_md5::H_F(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int Ti){
	a = shift_left(a + F(b, c, d) + x + Ti, s) + b;
}

void my_md5::H_G(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int Ti){
	a = shift_left(a + G(b, c, d) + x + Ti, s) + b;
}

void my_md5::H_H(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int Ti){
	a = shift_left(a + H(b, c, d) + x + Ti, s) + b;
}

void my_md5::H_I(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int Ti){
	a = shift_left(a + I(b, c, d) + x + Ti, s) + b;
}
```

Transform函数要经过64次迭代运算，然后将计算好的state保存，下面由于篇幅就不一一展示，只写出一次迭代的。

```c++
void my_md5::transform(unsigned char block[64]){
	// 获取IV的state数据
	unsigned int a = state[0], b = state[1], c = state[2], d = state[3];
	unsigned int x[16];
	// 将8位char转为32位int，当前处理消息分组的第 k 个 (k = 0..15) 32位字
	char_to_int(x, block, 64);
 	
	H_F(a, b, c, d, x[0], s[0][0], 0xd76aa478);
    ···
    H_G(a, b, c, d, x[1], s[1][0], 0xf61e2562);
    ···
    H_H(a, b, c, d, x[5], s[2][0], 0xfffa3942);
    ···
    H_I(a, b, c, d, x[0], s[3][0], 0xf4292244);
    ···
    state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}
```



### 3.输入字符串与输出加密后结果

输入函数必须还要进行初始化，因为上次的加密结果仍保存在state，不进行初始化会导致加密失败。

初始化是赋值给state初始向量IV，并采用小端存储的存储结构。将count记录长度，以及digest记录结果的数组赋值回0.

然后就可以执行update与padding操作了，执行完上面两个函数，结果存放在4个state中，故需要将4个state的结果连接起来放在digest中 ，便于后面转化回16进制输出。

```c++
void my_md5::decode_string(string& str){
	// 每次执行前，都要初始化一次 
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
	is_padding = false;
	memset(count, 0, sizeof(count));
	memset(digest, '\0', sizeof(digest));
	update((unsigned char*)(str.c_str()),str.size());
	padding();
	// 将4个state的结果连接起来放在digest中 
	int_to_char(digest, state, 16);
}
```

输出函数只是简单的将128位2进制转化为32位16进制，并输出结果。

```c++
void my_md5::show_result(){
	// 转化为16进制
	result[32] = '\0';
	for(int i=0; i<16; ++i){
		sprintf(result+i*2, "%02x", digest[i]);
	}
	cout << "解密结果：" << endl;
	cout << result << endl;
}
```



### 4.update函数与padding函数填充

update函数的处理是判断输入字符串的长度，并记录其长度到count数组中，接着进行分组进行transform函数，这里每一个分组要512bits。

```c++
	int index = (count[0] >> 3) & 0x3f;
	// 使用count时，将2个32位转成64bit来处理
	count[0] += str_length << 3;
	if(count[0] < (str_length << 3)){
		count[1]++;
	}
	// 需要填补的长度
	int padding_length = 64 - index;
	if (str_length >= padding_length)
	{
		// 将数据放入buffer中进行处理
		memcpy(buffer+index, input_str, padding_length);
		transform(buffer);
		// 分组处理 
		for (int i = padding_length; i+64 < str_length; i+=64)
		{
			transform((unsigned char*)input_str+i);
		}
		index = 0;
	}
	memcpy(buffer+index,input_str,str_length);
}
```

padding函数则是需要填充1000···000，以及补充64位的count。在填充完毕后，再进行update操作，这样就可以实现对字符串的填充处理。

```c++
nsigned char padding[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	if(!is_padding){
		unsigned char bits[8];
		// 将2个int转为64位bit 
		int_to_char(bits, count, 8);
		int index = (count[0] >> 3) & 0x3f;
		// 预留8位是给length，所以计算填充要减去8 
		int padding_length = index < 56 ? (56 - index) : (120 - index);
 		// 填补1000....0，即padding数组，长度为pad_len 
		update(padding, padding_length);
		// 对于长度8位进行处理 
		update(bits, 8);
		is_padding = true;
	}
}
```



### 5. Main函数

main函数主要是实现简单的命令行交互，构建MD5对象来执行期中的函数。这里唯一需要注意的只是**使用getline函数来处理带空格的字符串。**

```c++
#include "my_md5.h"
#include "my_md5.cpp"
 
int main()
{
	string str;
	cout << "MD5加密程序已启动"  << endl;
	my_md5 m;
	while(true){
		cout << "---------------------------------------" << endl;
		cout << "请输入需要加密的字符串，exit表示退出程序" << endl;
		getline(cin,str);
		if(str == "exit") break;
		m.decode_string(str);
		m.show_result();
	}
	return 0;
}
```



## 六. 编译运行结果

### 测试一

根据Wiki中提供的第一个例子来进行测试

```
"The quick brown fox jumps over the lazy dog"
```

**解密结果一致**



![3](https://github.com/dick20/Web-Security/blob/master/MD5/image/3.png)



### 测试二

根据Wiki中提供的第二个例子来进行测试，这次只多加了一个句号，但是加密结果与测试一完全不一样。

```
"The quick brown fox jumps over the lazy dog."
```

**解密结果一致**

![4](https://github.com/dick20/Web-Security/blob/master/MD5/image/4.png)



### 测试三

我自己在网上MD5在线加密进行测试

```
"Hello,MD5"
```

**解密结果一致**

![5](https://github.com/dick20/Web-Security/blob/master/MD5/image/5.png)



所以，根据上述的测试可以得出结论**算法实现正确，程序执行无误。**
