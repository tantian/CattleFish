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
import org.json.JSONException;
import org.json.JSONObject;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.baidu.mobstat.StatService;
import com.iflytek.tts.TtsService.Tts;
import com.iflytek.tts.TtsService.Tts.onReadListeren;
import com.readboy.cocos2dx.jni.ReadGameData;
import com.readboy.cocos2dx.jni.SoundResInfo;
import com.readboy.plan.service.PlanRecordDatabase;
import com.readboy.plan.service.PlanRecordDatabase.PlanRecord;
import com.readboy.sound.Sound;
import com.readboy.tutorgame.TutorGameData;
import com.readboy.tutorgame.TutorGameQst;

public class MainActivity extends Cocos2dxActivity {
	
	private static Sound mSound = null;
	
	private BroadcastReceiver mRev = new BroadcastReceiver() {
		
		@Override
		public void onReceive(Context context, Intent intent) {
			if (intent.getAction().equals("android.intent.cattlefish.savewrong.data")) {
				Log.i("CattleFish", "android.intent.cattlefish.savewrong.data");
			}
			if (intent.getAction().equals("android.intent.cattlefish.notification.data")) {
				Log.i("CattleFish", "android.intent.cattlefish.notification.data");
			}
		}
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		
//		if (Build.VERSION.SDK_INT == 22) {
//			getWindow().setFlags(0x80000000, 0x80000000);
//		} else if (Build.VERSION.SDK_INT == 21) {
//			// yan zheng 
//			getWindow().setFlags(0x80000000, 0x80000000);
//		} else if (Build.VERSION.SDK_INT == 20) {
//			getWindow().setFlags(0x80000000, 0x80000000);
//		} else if (Build.VERSION.SDK_INT == 19) {
//			//yan zheng 
//			getWindow().setFlags(0x80000000, 0x80000000);
//			
//		} else if (Build.VERSION.SDK_INT == 18) {
//			getWindow().setFlags(0x80000000, 0x80000000);
//		} else if (Build.VERSION.SDK_INT == 17) {
//			// yan zheng 
//			getWindow().setFlags(0x02000000, 0x02000000);
//			
//		} else if (Build.VERSION.SDK_INT == 16) {
//			getWindow().setFlags(0x80000000, 0x80000000);
//		} else if (Build.VERSION.SDK_INT == 15) {
			//yan zheng
//			getWindow().setFlags(0x80000000, 0x80000000);
//		} else if (Build.VERSION.SDK_INT == 14) {
			// yan zheng 
//			getWindow().setFlags(0x80000000, 0x80000000);
//		}
		
		Log.i("", "-------- onCreate() SDK_INT "+Build.VERSION.SDK_INT);
		if (Build.VERSION.SDK_INT > 19) {
			getWindow().setFlags(0x80000000, 0x80000000);
		} else if(Build.VERSION.SDK_INT >= 17) {
			getWindow().setFlags(0x02000000, 0x02000000);
		} else {
			getWindow().setFlags(0x80000000, 0x80000000);
		}
		super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		
		ReadGameData.FilePath = intent.getStringExtra("file");
		ReadGameData.Adr = intent.getIntExtra("adr", -1);
		ReadGameData.APMODE_VALUE = intent.getIntExtra(ReadGameData.APMODE, -1);
		if (ReadGameData.APMODE_VALUE == ReadGameData.LEARN_EYE) {
			ReadGameData.FilePath = intent.getStringExtra("path");
		}
		
		Log.i("", "-------- filepath "+ReadGameData.FilePath+", "+ReadGameData.APMODE_VALUE);
		
		String machine_type = DeviceInformation.getDeviceInfo(DeviceInformation.KEY_MODEL);
		StatService.setAppChannel(this, machine_type, true);
		printMemory(0);
	}
	
	
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		StatService.onResume(this);
		Log.i("CattleFish", "MainActivity onResume() StatService.onResume() 百度统计接口");
//		IntentFilter flter = new IntentFilter("android.intent.cattlefish.notification.data");
//		flter.addAction("android.intent.cattlefish.savewrong.data");
//		registerReceiver(mRev, flter);
	}
	
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		if (mSound != null) {
			mSound.stop();
		}
		StatService.onPause(this);
		Log.i("CattleFish", "MainActivity onPause() StatService.onPause() 百度统计接口");
//		unregisterReceiver(mRev);
	}
	
	

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		printMemory(1);
		System.exit(0);
	}
	
	public static Sound sound() {
		if (mSound == null) {
			mSound = new Sound();
		}
		return mSound;
	}
	
	/**
	 * 学习计划进来，保存数据
	 * @param rightTitle 做对题目数
	 * @param doCount 做过题目数
	 * @param didTime 一次游戏的时间
	 */
	public static void savePlanData(int rightTitle, int doCount, int didTime, int score) {
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
		if(ReadGameData.APMODE_VALUE == ReadGameData.LEARN_EYE) {
			return true;
		}
		MainActivity ac = (MainActivity) getContext();
	    Intent intent = ac.getIntent();
    	String mSectName = intent.getStringExtra("sectName");
    	String mSubject = intent.getStringExtra("subject");
    	int mCatalogAdr = intent.getIntExtra("catalogAdr", -1);
    	int mDataId = intent.getIntExtra("dataId", -1);
		boolean save = false;
		if (mDataId < 0) {
			TutorGameData mGmData = new TutorGameData(ReadGameData.path(), ReadGameData.address(), TutorGameData.TYPE_CHOICE, 1, 1, 3, 3);
			TutorGameQst qsts[] = new TutorGameQst[qstnumbers.length];
			for (int i=0; i<qsts.length; i++) {
				int which = qstnumbers[i];
				Log.i("", "-MainActivity which errNumber is "+which);
				if (which < ReadGameData.mQstLst.size()) {
					qsts[i] = ReadGameData.mQstLst.get(which);
				}
			}
			save = mGmData.saveErrorQsts(104, qsts, mSubject, ReadGameData.path(), ReadGameData.Adr, mCatalogAdr, mSectName, rightRate);
			if (save) {
				mySendBroadcast(); 
			} 
			
		}
		ReadGameData.mQstLst.clear();
		return save;
	}
	
	
	static private void mySendBroadcast() {
		
		//创建Intent对象  
	    Intent intent=new Intent();  
	    //设置Intent的Action属性  
	    intent.setAction("com.readboy.personaltailor"); 
	    //发送广播  
	    getContext().sendBroadcast(intent);
//	    Log.i("", "-------- send broadcast com.readboy.personaltailor");
	}
	
	public static void release() {
		if (mSound != null) {
			if (mSound.isPlaying()) {
				mSound.stop();
			}
			mSound.release();
		}
	}
	
	
	public static boolean isSound(final int qstidx){
		boolean isSnd = false;
		if (qstidx < ReadGameData.mQstLst.size()) {
			isSnd = SoundResInfo.isSound(ReadGameData.FilePath, ReadGameData.Adr, ReadGameData.mQstLst.get(qstidx));
		}
		return isSnd;
	}
	
	public static void voiceStart(final int qstidx) {
		TutorGameQst qst = ReadGameData.mQstLst.get(qstidx);
		TutorGameData gmData = new TutorGameData(ReadGameData.FilePath, ReadGameData.Adr, TutorGameData.TYPE_CHOICE, 1, 1, 3, 3);
		SoundResInfo.playSubject(gmData, ReadGameData.FilePath, ReadGameData.Adr, qst, sound());
	}
	
	public static void voiceStop() {
		sound().stop();
	}
	
	
	public static void printMemory(int step){
		
		Log.e("CattleFish", "--printMemory step ="+ step +" used=" + Runtime.getRuntime().totalMemory()+"; max="+Runtime.getRuntime().maxMemory());
	}
	
	
	public static void showToast(String toastmsg) {
		Toast.makeText(getContext(), toastmsg, Toast.LENGTH_LONG).show();
	}
}
