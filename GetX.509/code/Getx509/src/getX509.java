 	import java.security.*;
    import java.io.*;
    import java.security.cert.*;
    import java.security.cert.Certificate;
public class getX509 {
	
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
		    String s=c.toString();
		    System.out.println("输出证书信息:\n"+s);
		    System.out.println();
		    System.out.println();
		    System.out.println("版本号:"+t.getVersion());
		    System.out.println("序列号:"+t.getSerialNumber().toString(16));
		    System.out.println("签发者："+t.getIssuerDN());
		    System.out.println("有效起始日期："+t.getNotBefore());
		    System.out.println("有效终止日期："+t.getNotAfter());
		    System.out.println("主体名："+t.getSubjectDN());
		    System.out.println("签名算法："+t.getSigAlgName());
		    System.out.println("签名："+t.getSignature().toString());
		    PublicKey pk=t.getPublicKey();
		    byte [] pkenc=pk.getEncoded();  
		    System.out.println("公钥:");
		    for(int i=0;i<pkenc.length;i++)System.out.print(pkenc[i]+",");
		    
		    System.out.println();
		} catch (CertificateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
