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

import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.bindings.QtService;

import android.util.Log;

public class SeasonProxerService extends QtService {
	private static final int PROGRESS_NOT_KEY = 13;
	private static final int STATUS_NOT_KEY = 42;
	private static final int OPEN_INTENT_ID = 0;
	public static final String GROUP_KEY = "de.skycoder42.seasonproxer.NotificationGroup";

	private NotificationCompat.Builder progressBuilder = null;

	public static native void quitApp();

	static public void startService(Context context) {
		context.startService(new Intent(context, SeasonProxerService.class));
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		super.onStartCommand(intent, flags, startId);
		return Service.START_NOT_STICKY;
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		quitApp();
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	public void showProgressNotification() {
		progressBuilder = new NotificationCompat.Builder(this)
			.setContentTitle(getResources().getString(R.string.update_progress_title))
			.setContentText(getResources().getString(R.string.update_progress_text))
			.setContentInfo(getResources().getString(R.string.app_name))
			.setCategory(NotificationCompat.CATEGORY_PROGRESS)
			.setLargeIcon(BitmapFactory.decodeResource(getResources(), R.drawable.ic_launcher))
			.setSmallIcon(R.drawable.ic_launcher)
			.setOngoing(true)
			.setProgress(0, 0, true)
			.setGroup(GROUP_KEY);

		startForeground(PROGRESS_NOT_KEY, progressBuilder.build());
	}

	public void updateProgress(int current, int max)
	{
		NotificationManager manager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
		progressBuilder.setProgress(max, current, false)
			.setContentText(current + " / " + max);
		manager.notify(PROGRESS_NOT_KEY, progressBuilder.build());
	}

	public void showUpdateNotification(boolean success, String title, String message) {
		stopForeground(true);

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
			.setContentInfo(getResources().getString(R.string.app_name))
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
