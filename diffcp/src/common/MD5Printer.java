package common;

public class MD5Printer {
	static char hexDigits[] = { // 用来将字节转换成 16 进制表示的字符
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
			'f' };

	public static String printMd5(byte dig[]) {
		char str[] = new char[dig.length * 2]; 
		int k = 0; 
		for (int i = 0; i < dig.length; i++) { 
			byte byte0 = dig[i]; 
			
			str[k++] = hexDigits[byte0 >>> 4 & 0xf]; 
			str[k++] = hexDigits[byte0 & 0xf]; 
		}
		 return new String(str);
	}
}
