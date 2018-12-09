#ifndef MY_MD5
#define MY_MD5
#include <cstring>
#include <string>

using namespace std;

// 左循环移位表
int s[4][4] = {{7,12,17,22},{5,9,14,20},{4,11,16,23},{6,10,15,21}};

class my_md5
{
public:
	my_md5(string& str);
	my_md5(); 
	void update(const unsigned char* input_str, int str_length);
	// 将128位结果，转化为16个byte输出
	void show_result();
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


#endif


