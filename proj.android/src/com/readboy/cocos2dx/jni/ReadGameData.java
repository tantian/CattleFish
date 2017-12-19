package com.readboy.cocos2dx.jni;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.Environment;
import android.util.Log;

import com.readboy.cattlefish.MainActivity;
import com.readboy.tutorgame.TutorGameData;
import com.readboy.tutorgame.TutorGameQst;

public class ReadGameData {
	
	private final static String FILEPATH ="bshdabnyuwnxaxue3shng_CE0F.ptd";
	public static final String APMODE = "apMode";
	public static int LEARN_EYE = 16;
	
	private final static int ADR = 5564160;
	
	public static String FilePath = null;
	public static int Adr = -1;
	public static int APMODE_VALUE = -1;
	
	private static JSONArray mJsonArr = null;
	
	private static boolean mTtsSound = false;

	
	public static ArrayList<TutorGameQst> mQstLst = new ArrayList<TutorGameQst>();
	
//	private static ReadGameData mReadGameData = null;
	
	
//	public ReadGameData() {
//		File mSdc = Environment.getExternalStorageDirectory();
//		String path = mSdc.getAbsolutePath()+File.separator+FILEPATH;
//		File f = new File(path);
//		Log.i("ReadGameData", "-------- f path is "+path);
//		if (f.exists()) {
//			Log.i("ReadGameData", "-------- file exists true ");
//			mGmData = new TutorGameData(path, ADR, TutorGameData.TYPE_CHOICE, 1, 1, 3, 3);
//			int qstNum = mGmData.getQstNum();
//		}
//	}
//	
//	public ReadGameData(int which) {
//		File mSdc = Environment.getExternalStorageDirectory();
//		String path = mSdc.getAbsolutePath()+File.separator+FILEPATH;
//		File f = new File(path);
//		Log.i("ReadGameData", "-------- f path is "+path);
//		if (f.exists()) {
//			Log.i("ReadGameData", "-------- file exists true ");
//			mGmData = new TutorGameData(path, ADR, TutorGameData.TYPE_CHOICE, 1, 1, 3, 3);
//			mQst = mGmData.getQst(which);
//		}
//	}
	
//	public static ReadGameData instance() {
//		if (mReadGameData == null) {
//			mReadGameData = new ReadGameData();
//		}
//		return mReadGameData;
//	}
	
	public static boolean isTtsSound() {
		return mTtsSound;
	}
	
	public static String path() {
		File mSdc = Environment.getExternalStorageDirectory();
		String path = mSdc.getAbsolutePath()+File.separator+FILEPATH;
		if (FilePath != null) {
			path = FilePath;
		} 
		return path;
	}
	
	public static int address() {
		int adr = ADR;
		if (FilePath != null) {
			adr = Adr;
		} 
		return adr;
	}
	
	public static int qstNumber() {
		File mSdc = Environment.getExternalStorageDirectory();
		String path = mSdc.getAbsolutePath()+File.separator+FILEPATH;
		int adr = ADR;
		
		if (FilePath != null) {
			path = FilePath;
			adr = Adr;
		} 
		File f = new File(path);
		int msgs = 10;
		
		Log.i("ReadGameData", " path is "+path);
		
		if (f.exists()) {
			Log.i("ReadGameData", "-------- file exist true ");
			if (APMODE_VALUE != LEARN_EYE) {
				TutorGameData gmData = new TutorGameData(path, adr, TutorGameData.TYPE_CHOICE, 1, 1, 3, 3);
				msgs = gmData.getQstNum();
			} else {
				JSONObject mJson = getJSONObjectFromPath(false, path);
				if (mJson != null && mJson.has("data")) {
					try {
						mJsonArr = mJson.getJSONArray("data");
						msgs = mJsonArr.length();
					} catch (JSONException e) {
						e.printStackTrace();
					}
				} else {
					msgs = 0;
				}
//				msgs = -1;
			}
		}
		Log.i("ReadGameData", "-------- title number is "+msgs);
		return msgs;
	}
	
	
	
	public static String[] qst(int which) {
		File mSdc = Environment.getExternalStorageDirectory();
		String path = mSdc.getAbsolutePath()+File.separator+FILEPATH;
		int adr = ADR;
		if (FilePath != null) {
			path = FilePath;
			adr = Adr;
		} 
		mTtsSound = false;
		File f = new File(path);
//		String[] msgs = {"下面哪个选项是一", "one", "two", "three"};
		String[] msgs = {"", " ", " ", " "};
		if (f.exists()) {
			if (APMODE_VALUE != LEARN_EYE) {
				
				TutorGameData gmData = new TutorGameData(path, adr, TutorGameData.TYPE_CHOICE, 1, 1, 3, 3);
				if (gmData.getQstNum() > 0) {
					TutorGameQst qst = gmData.getQst(which);
					USToChinese us = new USToChinese();					
					String str = qst.mSubject; //us.usChange(qst.mSubject);
					msgs[0] = us.delSpace(str);
					for (int i=0; i<qst.mOption.length; i++) {						
						msgs[i+1] = qst.mOption[i];// us.usChange(qst.mOption[i]);
					}
					mQstLst.add(qst);
					mTtsSound = SoundResInfo.isSound(path, adr, qst);
				} 
			
			} else {
				msgs = getJSONQst(which);
			}
			
		}
		
		MainActivity.voiceStop();
		return msgs;
	}
	
	
	
	public static String[] getJSONQst(int which) {
		File mSdc = Environment.getExternalStorageDirectory();
		String path = mSdc.getAbsolutePath()+File.separator+FILEPATH;
		if (FilePath != null) {
			path = FilePath;
		} 
		File f = new File(path);
		String[] msgs = {"", " ", " ", " "};
		if (f.exists()) {
			// 获得path文件 json数据
			JSONObject mJson = getJSONObjectFromPath(false, path);
			JSONArray mJsonArr = mJson.optJSONArray("data");
			
			if (mJsonArr != null && mJsonArr.length() > which) {
				
				try {
					JSONObject json = mJsonArr.getJSONObject(which);
					if (json.has("content")) {
						// 这里获得题干
						msgs[0] = json.getString("content");
//						Log.i("", " content "+json.getString("content"));
						
						// 这里获得选项
						JSONArray array = json.optJSONArray("options");
						if(array != null){
							for(int i = 0; i < array.length(); i++){
//								Log.d("TAG", String.format("%d,%s" ,i,array.optString(i)));
								msgs[i+1] = array.optString(i);
							}
						}
						
						// 这里获得正确答案 
						if (json.has("correctAnswers")) {
							JSONArray rightAnser = json.getJSONArray("correctAnswers");
							int rightId = rightAnser.getString(0).charAt(0) - 'A';
							Log.i("", " rightAnser "+rightAnser.getString(0).charAt(0)+", "+rightId);
							// 这个正确答案不在第一个时，把正确答案放在第一个
							if (rightId > 0) {
								String strTemp = msgs[1];
								msgs[1] = msgs[1+rightId];
								msgs[1+rightId] = strTemp;
							}
						}
							
					}
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}

		}
		
		return msgs;
	}
	

//	public void data(int which) {
//		mQst = mGmData.getQst(which);
//		String[] msgs = new String[4];
//		msgs[0] = mQst.mSubject;
//		Log.i("", "-------- subject is subject is "+mQst.mSubject);
//		for (int i=0; i<mQst.mOption.length; i++) {
//			msgs[i+1] = mQst.mOption[i];
//			Log.i("", "-------- subject is option is "+mQst.mOption[i]);
//		}
//	}
//	
//	public String subject() {
//		return mQst.mSubject;
//	}
//	
//	public String option(int which) {
//		return mQst.mOption[which];
//	}
//	
//	public void release() {
////		mGmData.
//	}
//	
//	public String[] title(int which) {
//		mQst = mGmData.getQst(which);
//		int optionLength = mQst.mOptionNum;
//		String[] strs = new String[optionLength+1];
//		strs[0] = mQst.mSubject;
//		for (int i=0; i<optionLength; i++) {
//			strs[i+1] = mQst.mOption[i];
//			Log.i("", "-------- subject is option is "+mQst.mOption[i]);
//		}
//		return strs;
//	}
	
	
	/**
	 * 获取本地缓存数据
	 * @param bEnCode true 加密
	 * @param path 路径
	 * @return
	 */
	public static JSONObject getJSONObjectFromPath(boolean bEnCode, String path) {
		String rawJson = null;
		byte[] buffer = new byte[1024];
		ArrayList<Byte> buffers = new ArrayList<Byte>();
		int length = 0;
		JSONObject json;
		try {
			FileInputStream is = new FileInputStream(path);
			while ((length = is.read(buffer)) != -1) {
				for (int i = 0; i < length; i++) {
					buffers.add(buffer[i]);
				}
			}
			is.close();
			// 解密数据
			byte[] encrypt = new byte[buffers.size()];
			for (int i = 0; i < buffers.size(); i++) {
				encrypt[i] = buffers.get(i);
			}
			if(bEnCode){
//				rawJson = new String(RC4.crypt(encrypt, 0, encrypt.length), "utf-8");
			}else{
				rawJson = new String(encrypt, "utf-8");
			}
		} catch (IOException e) {
			Log.e("ReadGameData", "file open fail");
			return null;
		}
		try {
			json = new JSONObject(rawJson);
		} catch (JSONException e) {
			Log.e("ReadGameData", "json parse fail");
			return null;
		}
		return json;
	}

	
}
