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
