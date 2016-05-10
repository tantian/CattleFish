package com.readboy.cocos2dx.jni;

public class USToChinese {
	public USToChinese() {}
	
	public String usChange(String mExplain) {
		boolean isChange = false;
		String reString = mExplain;
		if (mExplain != null) {
			char[] chars = mExplain.toCharArray();
			for (int i = 0; i < chars.length; i++) {
				if (chars[i] == 0x0061) {
					chars[i] = 0x058D;// a
					isChange = true;
				} else if (chars[i] == 0x0101) {
					chars[i] = 0x0589;// a一声
					isChange = true;
				} else if (chars[i] == 0x00E1) {
					chars[i] = 0x058A;// a二声
					isChange = true;
				} else if (chars[i] == 0x01CE) {
					chars[i] = 0x058B;// a三声
					isChange = true;
				} else if (chars[i] == 0x00E0) {
					chars[i] = 0x058C;// a四声
					isChange = true;
				}
				if (isChange)
					chars[i] = oldCodeToNewCode(chars[i]);
			}
			if (isChange)
				reString = new String(chars);
		}
//		Log.i("", "-------- reString is "+reString);
		return reString; 
	}
		
	public char oldCodeToNewCode(char code) {
		char newCode = code;

		if (code >= 0x0550 && code < 0x05bb) {
			newCode = (char) (code - 0x0550 + 0xE250);
		}

		return newCode;
	}
	
	public String delSpace(String mExplain) {
		boolean isDel = false;
		boolean isChange = false;
		String reString = mExplain;
		if (mExplain != null) {
			reString = mExplain.replaceAll("[（\\(]\\s*[）\\)]", "（）");
//			char[] chars = mExplain.toCharArray();
//			char[] reback = new char[chars.length];
//			int j=0;
//			for (int i = 0; i < chars.length; i++) {
//				if (chars[i] == '）' || chars[i] == ')') {
//					isDel = false;
//				} 
//				if (!isDel) {
//					reback[j] = chars[i];
//					j++;
//				} 
//				if (chars[i] == '（' || chars[i] == '(') {
//					isDel = true;
//					isChange = true;
//				} 
//				
//			}
//			if (isChange)
//				reString = new String(reback);
		}
//		Log.i("", "-------- reString is "+reString);
		return reString; 
	}
	
}
