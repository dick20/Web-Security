#include "my_md5.h"
#include <iostream>

my_md5::my_md5(string& str){
	// 初始化获取IV
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

my_md5::my_md5(){}

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

my_md5::~my_md5(){
}

void my_md5::update(const unsigned char* input_str, int str_length){
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

void my_md5::show_result(){
	// 转化为16进制
	result[32] = '\0';
	for(int i=0; i<16; ++i){
		sprintf(result+i*2, "%02x", digest[i]);
	}
	cout << "加密结果：" << endl;
	cout << result << endl;
}

void my_md5::padding(){
	unsigned char padding[64] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
	cout << "MD5加密完成，以下是四个state信息" << endl; 
	for(int i = 0; i < 4; i++){
		cout << state[i] << endl;
	}
}

void my_md5::transform(unsigned char block[64]){
	// 获取IV的state数据
	unsigned int a = state[0], b = state[1], c = state[2], d = state[3];
	unsigned int x[16];
	// 将8位char转为32位int，当前处理消息分组的第 k 个 (k = 0..15) 32位字
	char_to_int(x, block, 64);
 
	H_F(a, b, c, d, x[0], s[0][0], 0xd76aa478);
	H_F(d, a, b, c, x[1], s[0][1], 0xe8c7b756);
	H_F(c, d, a, b, x[2], s[0][2], 0x242070db);
	H_F(b, c, d, a, x[3], s[0][3], 0xc1bdceee);
	H_F(a, b, c, d, x[4], s[0][0], 0xf57c0faf);
	H_F(d, a, b, c, x[5], s[0][1], 0x4787c62a);
	H_F(c, d, a, b, x[6], s[0][2], 0xa8304613);
	H_F(b, c, d, a, x[7], s[0][3], 0xfd469501); 
	H_F(a, b, c, d, x[8], s[0][0], 0x698098d8);
	H_F(d, a, b, c, x[9], s[0][1], 0x8b44f7af);
	H_F(c, d, a, b, x[10], s[0][2], 0xffff5bb1);
	H_F(b, c, d, a, x[11], s[0][3], 0x895cd7be);
	H_F(a, b, c, d, x[12], s[0][0], 0x6b901122);
	H_F(d, a, b, c, x[13], s[0][1], 0xfd987193);
	H_F(c, d, a, b, x[14], s[0][2], 0xa679438e);
	H_F(b, c, d, a, x[15], s[0][3], 0x49b40821);
	
	H_G(a, b, c, d, x[1], s[1][0], 0xf61e2562);
	H_G(d, a, b, c, x[6], s[1][1], 0xc040b340);
	H_G(c, d, a, b, x[11], s[1][2], 0x265e5a51);
	H_G(b, c, d, a, x[0], s[1][3], 0xe9b6c7aa); 
	H_G(a, b, c, d, x[5], s[1][0], 0xd62f105d);
	H_G(d, a, b, c, x[10], s[1][1],  0x2441453);
	H_G(c, d, a, b, x[15], s[1][2], 0xd8a1e681);
	H_G(b, c, d, a, x[4], s[1][3], 0xe7d3fbc8);
	H_G(a, b, c, d, x[9], s[1][0], 0x21e1cde6);
	H_G(d, a, b, c, x[14], s[1][1], 0xc33707d6);
	H_G(c, d, a, b, x[3], s[1][2], 0xf4d50d87);
	H_G(b, c, d, a, x[8], s[1][3], 0x455a14ed);
	H_G(a, b, c, d, x[13], s[1][0], 0xa9e3e905);
	H_G(d, a, b, c, x[2], s[1][1], 0xfcefa3f8);
	H_G(c, d, a, b, x[7], s[1][2], 0x676f02d9);
	H_G(b, c, d, a, x[12], s[1][3], 0x8d2a4c8a);
 
	H_H(a, b, c, d, x[5], s[2][0], 0xfffa3942);
	H_H(d, a, b, c, x[8], s[2][1], 0x8771f681);
	H_H(c, d, a, b, x[11], s[2][2], 0x6d9d6122);
	H_H(b, c, d, a, x[14], s[2][3], 0xfde5380c);
	H_H(a, b, c, d, x[1], s[2][0], 0xa4beea44);
	H_H(d, a, b, c, x[4], s[2][1], 0x4bdecfa9);
	H_H(c, d, a, b, x[7], s[2][2], 0xf6bb4b60);
	H_H(b, c, d, a, x[10], s[2][3], 0xbebfbc70);
	H_H(a, b, c, d, x[13], s[2][0], 0x289b7ec6);
	H_H(d, a, b, c, x[0], s[2][1], 0xeaa127fa); 
	H_H(c, d, a, b, x[3], s[2][2], 0xd4ef3085);
	H_H(b, c, d, a, x[6], s[2][3],  0x4881d05);
	H_H(a, b, c, d, x[9], s[2][0], 0xd9d4d039);
	H_H(d, a, b, c, x[12], s[2][1], 0xe6db99e5);
	H_H(c, d, a, b, x[15], s[2][2], 0x1fa27cf8);
	H_H(b, c, d, a, x[2], s[2][3], 0xc4ac5665);
 
	H_I(a, b, c, d, x[0], s[3][0], 0xf4292244);
	H_I(d, a, b, c, x[7], s[3][1], 0x432aff97);
	H_I(c, d, a, b, x[14], s[3][2], 0xab9423a7);
	H_I(b, c, d, a, x[5], s[3][3], 0xfc93a039);
	H_I(a, b, c, d, x[12], s[3][0], 0x655b59c3);
	H_I(d, a, b, c, x[3], s[3][1], 0x8f0ccc92);
	H_I(c, d, a, b, x[10], s[3][2], 0xffeff47d);
	H_I(b, c, d, a, x[1], s[3][3], 0x85845dd1);
	H_I(a, b, c, d, x[8], s[3][0], 0x6fa87e4f);
	H_I(d, a, b, c, x[15], s[3][1], 0xfe2ce6e0);
	H_I(c, d, a, b, x[6], s[3][2], 0xa3014314);
	H_I(b, c, d, a, x[13], s[3][3], 0x4e0811a1);
	H_I(a, b, c, d, x[4], s[3][0], 0xf7537e82);
	H_I(d, a, b, c, x[11], s[3][1], 0xbd3af235);
	H_I(c, d, a, b, x[2], s[3][2], 0x2ad7d2bb);
	H_I(b, c, d, a, x[9], s[3][3], 0xeb86d391);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}


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

