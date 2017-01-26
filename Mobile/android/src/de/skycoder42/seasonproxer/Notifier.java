package de.skycoder42.seasonproxer;

import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.app.NotificationManager;
import android.app.Notification;
import android.app.PendingIntent;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationCompat.BigTextStyle;

import org.qtproject.qt5.android.bindings.QtActivity;

import org.qtproject.qt5.android.bindings.QtService;
import android.util.Log;
import android.content.ComponentName;

public class Notifier {	
	private static final int STATUS_NOT_KEY = 42;
	private static final int OPEN_INTENT_ID = 0;
	public static final String GROUP_KEY = "com.skycoder42.seasonproxer.NotificationGroup";

	private Context context;

	static public void startService(Context context) {
		Log.wtf("SeasonProxer.Notifier", "Starting service...");
		ComponentName c = context.startService(new Intent(context, SeasonProxerService.class));
		if(c != null)
			Log.wtf("SeasonProxer.Notifier", "Created: " + c.flattenToString());
		else
			Log.wtf("SeasonProxer.Notifier", "IT IS NULL!!!");
	}

	public Notifier(Context context) {
		this.context = context;
	}

	public void showUpdateNotification(boolean success, String title, String message) {
		NotificationManager manager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

		Intent openIntent = new Intent(context, QtActivity.class);
		PendingIntent pending = PendingIntent.getActivity(context,
			OPEN_INTENT_ID,
			openIntent,
			PendingIntent.FLAG_UPDATE_CURRENT);

		NotificationCompat.Builder builder = new NotificationCompat.Builder(context)
			.setContentTitle(title)
			.setContentText(context.getResources().getString(R.string.expand_notifcation))
			.setStyle(new NotificationCompat.BigTextStyle()
				.bigText(message))
			.setContentInfo("Proxer.me Season Reminder")
			.setContentIntent(pending)
			.setLargeIcon(BitmapFactory.decodeResource(context.getResources(), R.drawable.ic_launcher))
			.setSmallIcon(R.drawable.ic_launcher)
			.setAutoCancel(true)
			.setCategory(success ? NotificationCompat.CATEGORY_EVENT : NotificationCompat.CATEGORY_ERROR)
			.setDefaults(Notification.DEFAULT_ALL)
			.setGroup(GROUP_KEY);

		manager.notify(STATUS_NOT_KEY, builder.build());
	}
}
