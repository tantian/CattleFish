package com.readboy.cocos2dx.jni;

import java.io.File;

import com.readboy.sound.Sound;
import com.readboy.tutorgame.TutorGameData;
import com.readboy.tutorgame.TutorGameQst;

public class SoundResInfo {
	
	public static boolean isSound(String fileName, int adr, TutorGameQst qst) {
		boolean back = false;
		if (fileName != null && qst != null) {
			if (qst != null && qst.mSubjectResInfo!=null && qst.mSubjectResInfo.addr != 0 &&  qst.mSubjectResInfo.size != 0) {		
				back = true;
			}
		}
		
		return back;
	}

	/**
	 * 播放题干
	 * @param fileName
	 * @param adr
	 * @param qst
	 * @param sound
	 */
	public static void playSubject(TutorGameData gameData, String fileName, int adr, TutorGameQst qst, Sound sound) {
		if (fileName != null && qst != null && gameData!=null) {
			File file = new File(fileName);
//			InputStream is= null;
//			try {
//				is = new FileInputStream(file);
				
				if (qst != null && qst.mSubjectResInfo!=null && qst.mSubjectResInfo.addr != 0 &&  qst.mSubjectResInfo.size != 0) {		
//					byte[] bytes = new byte[(int) qst.mSubjectResInfo.size]; 
					byte[] bytes = gameData.getSoundData(qst.mSubjectResInfo.addr, (int) qst.mSubjectResInfo.size);
					if (bytes != null){
//						is.skip(qst.mSubjectResInfo.addr+adr);
//						is.read(bytes);
						sound.setDataSource(bytes);
						sound.start();
					}
				}
//			} catch (FileNotFoundException e) {
//				e.printStackTrace();
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
		}
	}
	
	
	/**
	 * 播放选项
	 * @param fileName
	 * @param adr
	 * @param qst
	 * @param option
	 * @param sound
	 */
	public static void playOptions(TutorGameData gameData, String fileName, int adr, TutorGameQst qst, int option, Sound sound) {
		if (fileName != null && qst != null && gameData!= null) {
			File file = new File(fileName);
//			InputStream is= null;
//			try {
//				is = new FileInputStream(file);
				
				if (qst != null && qst.mOptionResInfo!=null && qst.mOptionResInfo[option].addr != 0 &&  qst.mOptionResInfo[option].size != 0) {		
//					byte[] bytes = new byte[(int) qst.mOptionResInfo[option].size]; 
					byte[] bytes = gameData.getSoundData(qst.mOptionResInfo[option].addr, (int) qst.mOptionResInfo[option].size);
					if (bytes != null){
//						is.skip(qst.mOptionResInfo[option].addr+adr);
//						is.read(bytes);
						sound.setDataSource(bytes);
						sound.start();
					}
				}
//			} catch (FileNotFoundException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			} catch (IOException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
		}
	}
}