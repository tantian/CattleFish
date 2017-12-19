package com.readboy.cattlefish;

import java.lang.reflect.Field;
import java.util.UUID;

import android.content.Context;
import android.content.ContextWrapper;
import android.os.Build;
import android.telephony.TelephonyManager;

public class DeviceInformation {
	// 关键字
	public static final String KEY_MODEL = "MODEL";
	/**
	 * 获得机器型号
	 */
	public static String getDeviceInfo(String key) {
		String model = "";
		try {
			Class<Build> buildClass = Build.class;
			Field field = buildClass.getField(key);
			model = (String) field.get(new Build());
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return model;
	}

	/**
	 * 获取设备唯一标识码
	 * @param context
	 * @return
	 */
	public static String getUniqueId(Context context) {
		ContextWrapper cw = new ContextWrapper(context);
		final TelephonyManager tm = (TelephonyManager) cw.getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);
		final String tmDevice, tmSerial, androidId;
		tmDevice = "" + tm.getDeviceId();
		tmSerial = "" + tm.getSimSerialNumber();
		androidId = "" + android.provider.Settings.Secure.getString(cw.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
		UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
		String uniqueId = deviceUuid.toString();
		return uniqueId;
	}
}
