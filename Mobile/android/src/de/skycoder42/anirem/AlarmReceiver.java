package de.skycoder42.anirem;

import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.app.AlarmManager;
import android.support.v4.content.WakefulBroadcastReceiver;

public class AlarmReceiver extends WakefulBroadcastReceiver {
	private static final int ALARM_INTENT_ID = 20;

	@Override
	public void onReceive(Context context, Intent intent) {
		Intent serviceIntent = new Intent(context, SeasonProxerService.class);
		startWakefulService(context, serviceIntent);
	}

	static public void scheduleAutoCheck(Context context, boolean autoCheck) {
		Intent intent = new Intent(context, AlarmReceiver.class);

		//skip existing alarm
		if(autoCheck) {
			PendingIntent pending = PendingIntent.getBroadcast(context,
				ALARM_INTENT_ID,
				intent,
				PendingIntent.FLAG_NO_CREATE);
			if(pending != null)
				return;
		}

		PendingIntent pending = PendingIntent.getBroadcast(context,
			ALARM_INTENT_ID,
			intent,
			PendingIntent.FLAG_UPDATE_CURRENT);

		AlarmManager alarm = (AlarmManager)context.getSystemService(Context.ALARM_SERVICE);
		if(autoCheck) {
			alarm.setRepeating(AlarmManager.RTC_WAKEUP,
				System.currentTimeMillis() + AlarmManager.INTERVAL_DAY,
				AlarmManager.INTERVAL_DAY,
				pending);
		} else {
			alarm.cancel(pending);
			pending.cancel();
		}
	}
}
