#include <iostream>
#include <string>
#include "DES.cpp"

using namespace std;

void show_hint(){
	cout << "************ Hint ***************\n";
	cout << "Code:    encrypt——DES加密\n\t decrypt——DES解密\n\t exit——退出程序\n";
	cout << "*********************************\n";
	cout << "请输出指令：\n";
}

int main(){
	DES myDES;
	string m, k;
	string choose;
	bitset<64> c;
	while(true){
		show_hint();
		cin >> choose;
		if(choose == "encrypt"){
			cout << "请输入明文字符串（最多8位char）以及密钥（8位char）： " << endl; 
			cin >> m >> k;
			myDES.show_encrypt(m,k);
		}
		else if(choose == "decrypt"){
			cout << "请输入密文（64位bit）以及密钥（8位char）： " << endl; 
			cin >> c >> k;
			myDES.show_decrypt(c,k);
		}
		else if(choose == "exit"){
			break;
		}
	}
	return 0;
}
