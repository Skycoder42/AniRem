package de.skycoder42.seasonproxer;

import android.app.NotificationManager;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.os.IBinder;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationCompat.BigTextStyle;
import android.widget.Toast;

import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.bindings.QtService;

public class SeasonProxerService extends QtService {
	private static final int STATUS_NOT_KEY = 42;
	private static final int OPEN_INTENT_ID = 0;
	public static final String GROUP_KEY = "com.skycoder42.seasonproxer.NotificationGroup";

	static public void startService(Context context) {
		context.startService(new Intent(context, SeasonProxerService.class));
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
//		NotificationCompat.Builder builder = new NotificationCompat.Builder(this)
//						.setContentTitle("ServiceTest")
//						.setSmallIcon(R.drawable.ic_launcher);

//		startForeground(123, builder.build());
		super.onStartCommand(intent, flags, startId);
		return Service.START_NOT_STICKY;
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	public void showUpdateNotification(boolean success, String title, String message) {
		NotificationManager manager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);

		Intent openIntent = new Intent(this, QtActivity.class);
		PendingIntent pending = PendingIntent.getActivity(this,
			OPEN_INTENT_ID,
			openIntent,
			PendingIntent.FLAG_UPDATE_CURRENT);

		NotificationCompat.Builder builder = new NotificationCompat.Builder(this)
			.setContentTitle(title)
			.setContentText(getResources().getString(R.string.expand_notifcation))
			.setStyle(new NotificationCompat.BigTextStyle()
				.bigText(message))
			.setContentInfo("Proxer.me Season Reminder")
			.setContentIntent(pending)
			.setLargeIcon(BitmapFactory.decodeResource(getResources(), R.drawable.ic_launcher))
			.setSmallIcon(R.drawable.ic_launcher)
			.setAutoCancel(true)
			.setCategory(success ? NotificationCompat.CATEGORY_EVENT : NotificationCompat.CATEGORY_ERROR)
			.setDefaults(Notification.DEFAULT_ALL)
			.setGroup(GROUP_KEY);

		manager.notify(STATUS_NOT_KEY, builder.build());
	}
}
