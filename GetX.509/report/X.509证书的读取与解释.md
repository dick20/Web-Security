# X.509证书的读取与解释

软件工程（计算机应用）	16340132	梁颖霖

---

## 一.X.509 证书结构描述

**证书的结构如下所示：**

Certificate

+ Version
+ Serial Number
+ Algorithm ID
+ Issuer (CA’s name)
+ Validity
  + Not Before
  + Not After
+ Subject
+ Subject Public Key Info
  + Public Key Algorithm
  + Subject Public Key
+ Issuer Unique Identifier (Optional)
+ Subject Unique Identifier (Optional)
+ Extensions (Optional)
+ Certificate Signature Algorithm
+ Certificate Signature



**解释：**

1. Version 版本号：标识证书的版本（版本1、版本2或是版本3）。

2. Serial Number 序列号：标识证书的唯一整数，由证书颁发者分配的本证书的唯一标识符。

3. Algorithm ID：算法ID:  标识证书算法

4. Issuer (CA’s name) 颁发者：证书颁发者的可识别名（DN）。

5. Validity 有效期：证书有效期的时间段。本字段由”Not Before”和”Not After”两项组成，它们分别由UTC时间或一般的时间表示。

6. Subject 主体：证书拥有者的可识别名，这个字段必须是非空的，除非你在证书扩展中有别名。

7. Subject Public Key Info   主体公钥信息： 主体的公钥（以及算法标识符）。

8. Issuer Unique Identifier (Optional)  颁发者唯一标识符：标识符—证书颁发者的唯一标识符，仅在版本2和版本3中有要求，属于可选项。

9. Subject Unique Identifier (Optional) 主体唯一标识符：证书拥有者的唯一标识符，仅在版本2和版本3中有要求，属于可选项。

10. Extensions (Optional)  X.509证书扩展部分

11. Certificate Signature Algorithm  签名认证算法

12. Certificate Signature  认证签名


## 二.数据结构

这次我读取证书使用的是JAVA语言，使用java中一些证书的包，其中包括

```java
 	import java.security.*;
    import java.io.*;
    import java.security.cert.*;
    import java.security.cert.Certificate;
```

利用java中的CertificateFactory来获取X.509证书的工厂的实例

还使用了String数组与Byte数组来进行对数据处理，输出。

利用文件流读入证书来进行处理，FileInputStream的使用。

## 三.源代码

### 1.获取证书的过程

**step1：点击网站的地址左边的感叹号，找到证书的入口。**

![2](https://github.com/dick20/Web-Security/blob/master/GetX.509/report/image/2.png)



**step2：点击证书后，找到详细信息，找到复制到文件入口**

![3](https://github.com/dick20/Web-Security/blob/master/GetX.509/report/image/3.png)



**step3：选择格式，点击下一步，然后选择路径即可**

![4](https://github.com/dick20/Web-Security/blob/master/GetX.509/report/image/4.png)



### 2.处理证书

首先将获取到的证书放到合适的相对路径下，便于读入。CertificateFactory来获取X.509证书的工厂的实例，然后利用FileInputStream读入我从csdn网站拿下来的证书。

最后利用cf生成证书，接着用X509Certificate来将证书转变类型。

这样，我们就得到了X.509的证书实例了。

```java
	public static void main(String[] args) throws IOException {
		CertificateFactory cf;
		try {
            // 获取工厂实例
			cf = CertificateFactory.getInstance("X.509");
            // 用文件流读入证书
		    FileInputStream in=new FileInputStream("out.cer");
            // 生成证书
		    Certificate c=cf.generateCertificate(in);
		    X509Certificate t=(X509Certificate)c;
		    in.close();
         ···
}

```



### 3.输出证书中的内容

利用X509Certificate类的一些成员函数来进行输出关键信息，其中包括版本号，序列号，签发者，有效期限，主体名，签名算法，签名，公钥等等。并加上适当的解释语句来输出。

```java
			// 输出完整的证书信息
		    String s=c.toString();
		    System.out.println("输出证书信息:\n"+s);
            // 输出关键信息，截取部分并进行标记
		    System.out.println("版本号:"+t.getVersion());
		    System.out.println("序列号:"+
                               t.getSerialNumber().toString(16));
		    System.out.println("签发者："+t.getIssuerDN());
		    System.out.println("有效起始日期："+t.getNotBefore());
		    System.out.println("有效终止日期："+t.getNotAfter());
		    System.out.println("主体名："+t.getSubjectDN());
		    System.out.println("签名算法："+t.getSigAlgName());
		    System.out.println("签名："+t.getSignature().toString());
		    PublicKey pk=t.getPublicKey();
		    byte [] pkenc=pk.getEncoded();  
		    System.out.println("公钥:");
		    for(int i=0;i<pkenc.length;i++)
                System.out.print(pkenc[i]+",");
		} catch (CertificateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
```



## 四.编译运行输出结果

X.509证书的一些关键内容输出如下：

![1](https://github.com/dick20/Web-Security/blob/master/GetX.509/report/image/1.png)

该证书的版本号为3，序列号如图所示

**证书的签发者和证书主体用X.509 DN表示，DN是由RDN构成的序列。**

| 属性类型名称             | 含义         | 简写 |
| ------------------------ | ------------ | ---- |
| Common Name              | 通用名称     | CN   |
| Organizational Unit name | 机构单元名称 | OU   |
| Organization name        | 机构名       | O    |
| Locality                 | 地理位置     | L    |
| State or province name   | 州/省名      | S    |
| Country                  | 国名         | C    |

通过上表可以得出**签发者**的通用名称围为GeoTrust RSA CA 2018; 机构单元名称为 www.digicert.com; 机构名为Dogicert Inc; 国名为 US。

**主体名**的通用名称为*.csdn.net; 机构单元名称为IT; 机构名为北京创新乐知信息技术有限公司；地理位置为北京市，国家为CN

有效日期为2018.11.7到2020.11.06

签名算法为SHA256withRSA



**X.509证书的全部内容也可以进行输出**：

![5](https://github.com/dick20/Web-Security/blob/master/GetX.509/report/image/5.png)
