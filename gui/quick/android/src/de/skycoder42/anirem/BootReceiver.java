package de.skycoder42.anirem;

import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;
import android.content.SharedPreferences;

import android.app.PendingIntent;
import android.app.AlarmManager;

public class BootReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		scheduleAutoCheck(context);
	}

	static public void scheduleAutoCheck(Context context) {
		SharedPreferences prefs = context.getSharedPreferences(Globals.AutoStartPrefs, Context.MODE_PRIVATE);
		scheduleAutoCheckImp(context, prefs.getBoolean(Globals.AutStartKey, true));
	}

	static public void scheduleAutoCheck(Context context, boolean autoCheck) {
		scheduleAutoCheckImp(context, autoCheck);

		SharedPreferences prefs = context.getSharedPreferences(Globals.AutoStartPrefs, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = prefs.edit();
		editor.putBoolean(Globals.AutStartKey, autoCheck);
		editor.apply();
	}

	static private void scheduleAutoCheckImp(Context context, boolean autoCheck) {
		Intent intent = new Intent(context, AniremService.class);
		PendingIntent pending;
		if(Globals.isOreo())
			pending = PendingIntent.getForegroundService(context, Globals.AlarmIntentId, intent, PendingIntent.FLAG_UPDATE_CURRENT);
		else
			pending = PendingIntent.getService(context, Globals.AlarmIntentId, intent, PendingIntent.FLAG_UPDATE_CURRENT);

		AlarmManager alarm = (AlarmManager)context.getSystemService(Context.ALARM_SERVICE);
		if(autoCheck) {
			alarm.setRepeating(AlarmManager.RTC_WAKEUP,
				System.currentTimeMillis() + AlarmManager.INTERVAL_DAY,
				AlarmManager.INTERVAL_DAY - 3600000, //every 23 hours, to rotate over time
				pending);
		} else {
			alarm.cancel(pending);
			pending.cancel();
		}
	}
}
