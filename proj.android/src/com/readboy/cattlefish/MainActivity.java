/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.readboy.cattlefish;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.CocosBroadcastReceiver;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.baidu.mobstat.StatService;
import com.readboy.cocos2dx.jni.ReadGameData;
import com.readboy.cocos2dx.jni.SoundResInfo;
import com.readboy.data.DataCollector;
import com.readboy.data.DataCollector.DataType;
import com.readboy.plan.service.PlanRecordDatabase;
import com.readboy.plan.service.PlanRecordDatabase.PlanRecord;
import com.readboy.sound.Sound;
import com.readboy.tutorgame.TutorGameData;
import com.readboy.tutorgame.TutorGameQst;

public class MainActivity extends Cocos2dxActivity {
	private static Sound mSound = null;
	//private static boolean mFlag=false;
	private BroadcastReceiver mRev = new BroadcastReceiver() {

		@Override
		public void onReceive(Context context, Intent intent) {
			if (intent.getAction().equals(
					"android.intent.cattlefish.savewrong.data")) {
				//Log.i("CattleFish", "android.intent.cattlefish.savewrong.data");
			}
			if (intent.getAction().equals(
					"android.intent.cattlefish.notification.data")) {
				//Log.i("CattleFish",
					//	"android.intent.cattlefish.notification.data");
			}
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		// if (Build.VERSION.SDK_INT == 22) {
		// getWindow().setFlags(0x80000000, 0x80000000);
		// } else if (Build.VERSION.SDK_INT == 21) {
		// // yan zheng
		// getWindow().setFlags(0x80000000, 0x80000000);
		// } else if (Build.VERSION.SDK_INT == 20) {
		// getWindow().setFlags(0x80000000, 0x80000000);
		// } else if (Build.VERSION.SDK_INT == 19) {
		// //yan zheng
		// getWindow().setFlags(0x80000000, 0x80000000);
		//
		// } else if (Build.VERSION.SDK_INT == 18) {
		// getWindow().setFlags(0x80000000, 0x80000000);
		// } else if (Build.VERSION.SDK_INT == 17) {
		// // yan zheng
		// getWindow().setFlags(0x02000000, 0x02000000);
		//
		// } else if (Build.VERSION.SDK_INT == 16) {
		// getWindow().setFlags(0x80000000, 0x80000000);
		// } else if (Build.VERSION.SDK_INT == 15) {
		// yan zheng
		// getWindow().setFlags(0x80000000, 0x80000000);
		// } else if (Build.VERSION.SDK_INT == 14) {
		// yan zheng
		// getWindow().setFlags(0x80000000, 0x80000000);
		// }
		//Log.i("CattleFish", "MainActivity onCreate --------------------------");

		//Log.i("", "-------- onCreate() SDK_INT " + Build.VERSION.SDK_INT);
		Log.i("CattleFish","MainActivity onCreate");
		if (Build.VERSION.SDK_INT > 19) {
			getWindow().setFlags(0x80000000, 0x80000000);
		} else if (Build.VERSION.SDK_INT >= 17) {
			getWindow().setFlags(0x02000000, 0x02000000);
		} else {
			getWindow().setFlags(0x80000000, 0x80000000);
		}
		
		Intent intent = getIntent();

		ReadGameData.FilePath = intent.getStringExtra("file");
		SharedPreferences pre=getSharedPreferences("com.readboy.cattlefish.phone",MODE_WORLD_READABLE);
		SharedPreferences.Editor edi=pre.edit();
		if(ReadGameData.FilePath!=null)edi.putString("file", ReadGameData.FilePath);
		
		
		ReadGameData.Adr = intent.getIntExtra("adr", -1);
		ReadGameData.APMODE_VALUE = intent.getIntExtra(ReadGameData.APMODE, -1);
		if (ReadGameData.APMODE_VALUE == ReadGameData.LEARN_EYE) {
			ReadGameData.FilePath = intent.getStringExtra("path");
			if(ReadGameData.FilePath!=null)edi.putString("file", ReadGameData.FilePath);
		}
		if (ReadGameData.FilePath==null) {
			ReadGameData.FilePath=pre.getString("file", null);
		}

		//Log.i("", "-------- filepath " + ReadGameData.FilePath + ", "
			//	+ ReadGameData.APMODE_VALUE);

		String machine_type = DeviceInformation
				.getDeviceInfo(DeviceInformation.KEY_MODEL);
		StatService.setAppChannel(this, machine_type, true);
		printMemory(0);
	}
	 


	@Override
	protected void onNewIntent(Intent intent) {	
		Log.i("CattleFish","MainActivity Intent");
		// TODO Auto-generated method stub
		super.onNewIntent(intent);
		if (intent != null) {
			setIntent(intent);
			String lastFilePath = ReadGameData.FilePath;
			String newFilePath = intent.getStringExtra("file");
			
			SharedPreferences pre=getSharedPreferences("com.readboy.cattlefish.phone",MODE_WORLD_READABLE);
			SharedPreferences.Editor edi=pre.edit();
			if(newFilePath!=null)edi.putString("file", newFilePath);
			
			
			Log.i("lastFilePath",lastFilePath);
			int lastAdr = ReadGameData.Adr;
			int adr = intent.getIntExtra("adr", -1);
			int apmode = intent.getIntExtra(ReadGameData.APMODE, -1);
			
			if (apmode == ReadGameData.LEARN_EYE) {
				newFilePath = intent.getStringExtra("path");
				if(newFilePath!=null)edi.putString("file", newFilePath);
			}
			
			if (newFilePath==null) {
				newFilePath=pre.getString("file", null);
			}
			
			String machine_type = DeviceInformation
					.getDeviceInfo(DeviceInformation.KEY_MODEL);
			StatService.setAppChannel(this, machine_type, true);
			if (lastFilePath != null && newFilePath != null) {
				Log.i("CattleFish","MainActivity FileExit");
				boolean isRestart = false;
				if (!lastFilePath.equals(newFilePath)) {
					isRestart = true;
				} else if (adr != lastAdr && adr != -1) {
					isRestart = true;
				} else {
					isRestart = false;
				}
				if (isRestart) {
					Log.i("CattleFish","MainActivity restart");
					ReadGameData.FilePath = newFilePath;
					ReadGameData.Adr = adr;
					ReadGameData.APMODE_VALUE = apmode;
					nativeRestart();
					
					
//					Cocos2dxHelper.stopBackgroundMusic();
//					Cocos2dxHelper.stopAllEffects();
//					Cocos2dxHelper.preloadBackgroundMusic("snd/snd_bg.ogg");
//					Cocos2dxHelper.playBackgroundMusic("snd/snd_bg.ogg", true);
					
				}
			}
		}
		

	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		StatService.onResume(this);
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		if (mSound != null) {
			mSound.stop();
		}
		StatService.onPause(this);
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		Log.i("CattleFish", "MainActivity ondestroy --------------------------");
		super.onDestroy();
		
		printMemory(1);
		System.exit(0);
	}

	private void processExtra(){
		Intent intent=getIntent();
	}
	
	public static Sound sound() {
		if (mSound == null) {
			mSound = new Sound();
		}
		return mSound;
	}

	/**
	 * 学习计划进来，保存数据
	 * 
	 * @param rightTitle
	 *            做对题目数
	 * @param doCount
	 *            做过题目数
	 * @param didTime
	 *            一次游戏的时间
	 */
	public static void savePlanData(int rightTitle, int doCount, int didTime,
			int score) {
		MainActivity ac = (MainActivity) getContext();
		Intent intent = ac.getIntent();
		if ("plan".equals(intent.getStringExtra("origin"))) {
			PlanRecord planRecord = new PlanRecord();
			planRecord.cid = intent.getIntExtra("cid", 0);
			planRecord.type = "game";
			JSONObject json = new JSONObject();
			try {
				json.put("qstNum", doCount);
				json.put("rightQstNum", rightTitle);
				json.put("score", score);
				json.put("gametime", didTime);
				planRecord.data = json.toString();
				PlanRecordDatabase.push(planRecord);
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}

	}

	/**
	 * 
	 * @param rightRate
	 * @param qstnumbers
	 * @return
	 */
	public static boolean saveWrongData(int rightRate, int qstnumbers[]) {
		MainActivity ac = (MainActivity) getContext();
		Intent intent = ac.getIntent();
		String mSectName = intent.getStringExtra("sectName");
		String mSubject = intent.getStringExtra("subject");
		int mCatalogAdr = intent.getIntExtra("catalogAdr", -1);
		int mDataId = intent.getIntExtra("dataId", -1);
		boolean save = false;
		if (ReadGameData.APMODE_VALUE == ReadGameData.LEARN_EYE) {
			String bookId = intent.getStringExtra("bookId");
			String pageId = intent.getStringExtra("pageId");
			String from = intent.getStringExtra("from");
			
			String bookSectionId = intent.getStringExtra("bookSectionId");
			String ftm2SectionId = intent.getStringExtra("ftm2SectionId");
			int module = intent.getIntExtra("module", -1);
			String unzipPath = intent.getStringExtra("upzipPath");
			String ftz2FilePath = intent.getStringExtra("ftz2FilePath");
			
			save = saveJsonWrongData(DataType.BOOK_CAPTURE_CATTLE_FISH,
					qstnumbers, mSubject, bookId, pageId, mSectName, from,
					rightRate, bookSectionId,ftm2SectionId,module,unzipPath,ftz2FilePath);
		} else if (mDataId < 0) {
			TutorGameData mGmData = new TutorGameData(ReadGameData.path(),
					ReadGameData.address(), TutorGameData.TYPE_CHOICE, 1, 1, 3,
					3);
			TutorGameQst qsts[] = new TutorGameQst[qstnumbers.length];
			for (int i = 0; i < qsts.length; i++) {
				int which = qstnumbers[i];
				if (which < ReadGameData.mQstLst.size()) {
					qsts[i] = ReadGameData.mQstLst.get(which);
				}
			}
			save = mGmData.saveErrorQsts(104, qsts, mSubject,
					ReadGameData.path(), ReadGameData.Adr, mCatalogAdr,
					mSectName, rightRate);
		}
		if (save) {
			mySendBroadcast();
		}
		ReadGameData.mQstLst.clear();
		return save;
	}

	/**
	 * 保存错误游戏数据到数据库
	 * 
	 * @param type
	 *            哪个游戏类型
	 * @param wrongTtlOrdr
	 *            错题题号集
	 * @param subject
	 *            科目："语文","数学","英语"
	 * @param sectAdr
	 *            课文地址
	 * @param sectNane
	 *            课文名称
	 * @param score
	 *            成绩
	 * @return
	 */
	public static boolean saveJsonWrongData(int type, int wrongTtlOrdr[],
			String subject, String bookId, String pageId, String sectName,
			String from, int score, String bookSectionId,String ftm2SectionId,int module,String unzipPath, String ftz2FilePath) {
		boolean save = false;
		if (wrongTtlOrdr != null && wrongTtlOrdr.length > 0) {
			JSONArray jsArray = new JSONArray();
			int qstLength = 0;
			try {
				JSONObject mJsonObj = ReadGameData.getJSONObjectFromPath(false,
						ReadGameData.FilePath);
				JSONArray mJsonArr = mJsonObj.optJSONArray("data");
				for (int i = 0; i < wrongTtlOrdr.length; i++) {
					JSONObject json = mJsonArr.getJSONObject(wrongTtlOrdr[i]);
					jsArray.put(json);
					qstLength++;
				}
			} catch (JSONException e) {
				e.printStackTrace();
			}

			if (qstLength > 0) {
							String data;
							String md5;
							if (qstLength > 0) {
								if(!from.equals("ftz2")){
									 data = DataCollector.packData(subject, ReadGameData.FilePath, bookId, pageId, sectName, from, jsArray);
									 md5 = DataCollector.getMd5(type, null, bookId, pageId);
								}else{
									 data = packTutorJsonData(subject, ReadGameData.FilePath, bookId, pageId, sectName, from, jsArray,from,unzipPath,ftz2FilePath,bookSectionId,ftm2SectionId,subject,module);
									 md5 = DataCollector.getMd5(type, null, bookId, ftm2SectionId);
								}
								save=DataCollector.update(type, md5, data, score);
								Log.e("saveJsonWrongData=====", "update--sucess versionName=1.4.9");
							}				
				}
				/*
				String data = DataCollector.packData(subject,
						ReadGameData.FilePath, bookId, pageId, sectName, from,
						jsArray);
				//Log.i("", "saveWrongJsonData:" + data);
				String md5 = DataCollector.getMd5(type, null, bookId, pageId);
				save = DataCollector.update(type, md5, data, score);*/
			}
//		}
		return save;
	}

	static private void mySendBroadcast() {

		// 创建Intent对象
		Intent intent = new Intent();
		// 设置Intent的Action属性
		intent.setAction("com.readboy.personaltailor");
		// 发送广播
		getContext().sendBroadcast(intent);
		// Log.i("", "-------- send broadcast com.readboy.personaltailor");
	}

	private static String packTutorJsonData(String subject, String fileName, String bookId, String pageId, String sectName, String origin, JSONArray jsonArray,String datatype,String unzipPath,String filePath,String bookSectionId,String ftm2SectionId,String type,int module){
		JSONObject object = new JSONObject();

		try {
			if(subject != null) {
				object.put("subject", subject);
			}

			if(fileName != null) {
				object.put("fileName", fileName);
			}

			if(bookId != null) {
				object.put("bookId", bookId);
			}

			if(pageId != null) {
				object.put("pageId", pageId);
			}

			if(sectName != null) {
				object.put("sectName", sectName);
			}

			if(origin != null) {
				object.put("from", origin);
				object.put("origin", origin);
				Log.d("data", "from/origin:  " + origin);
			}

			if(jsonArray != null) {
				object.put("data", jsonArray);
			}

			if(datatype != null) {
				object.put("datatype", datatype);
			}

			if(unzipPath != null) {
				object.put("unzipPath", unzipPath);
			}

			if(bookSectionId != null) {
				object.put("bookSectionId", bookSectionId);
			}

			if(filePath != null) {
				object.put("filePath", filePath);
			}

			if(ftm2SectionId != null) {
				object.put("ftm2SectionId", ftm2SectionId);
			}

			if(type != null) {
				object.put("type", type);
			}

			if(module != -1) {
				object.put("module", module);
			}

		} catch (JSONException var9) {
			var9.printStackTrace();
			return null;
		}

		String str = object.toString();
		str = str.replaceAll("\'", "\'\'");
		return str;
	}

	public static void release() {
		if (mSound != null) {
			if (mSound.isPlaying()) {
				mSound.stop();
			}
			mSound.release();
		}
	}
	//按退出按钮释放资源
    public static void releaseResourse(){
    	Log.i("CattleFish", "MainActivity finish");
    	
    	MainActivity ac = (MainActivity) getContext();
		ac.finish();
		
		/*System.exit(0);*/
    }
    
	public static boolean isSound(final int qstidx) {
		boolean isSnd = false;
		if (qstidx < ReadGameData.mQstLst.size()) {
			isSnd = SoundResInfo.isSound(ReadGameData.FilePath,
					ReadGameData.Adr, ReadGameData.mQstLst.get(qstidx));
		}
		return isSnd;
	}
	
	public static void voiceStart(final int qstidx) {
		if (qstidx < ReadGameData.mQstLst.size()) {
			TutorGameQst qst = ReadGameData.mQstLst.get(qstidx);
			if (qst != null) {
				TutorGameData gmData = new TutorGameData(ReadGameData.FilePath,
						ReadGameData.Adr, TutorGameData.TYPE_CHOICE, 1, 1, 3, 3);
				SoundResInfo.playSubject(gmData, ReadGameData.FilePath,
						ReadGameData.Adr, qst, sound());
			}
		}
	}

	public static void voiceStop() {
		if (mSound != null) {
			sound().stop();
		}
	}

	public static void printMemory(int step) {

		//Log.e("CattleFish", "--printMemory step =" + step + " used="
			//	+ Runtime.getRuntime().totalMemory() + "; max="
			//	+ Runtime.getRuntime().maxMemory());
	}

	public static void showToast(String toastmsg) {
		Toast.makeText(getContext(), toastmsg, Toast.LENGTH_LONG).show();
	}
	
	public static native void nativeStart();
	public static native void nativeUnseen();
	public static native void nativeRestart();
}
