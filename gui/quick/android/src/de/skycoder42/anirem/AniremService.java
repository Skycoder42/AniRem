package de.skycoder42.anirem;

import android.content.Context;
import android.content.Intent;

import android.app.PendingIntent;
import android.app.Notification;
import android.app.NotificationManager;

import android.graphics.BitmapFactory;

import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationCompat.BigTextStyle;

import org.qtproject.qt5.android.bindings.QtService;

public class AniremService extends QtService {
	private NotificationCompat.Builder progressBuilder = null;

	@Override
	public void onDestroy() {
		super.onDestroy();
		// explicitly exit to prevent the process from beeing cached
		System.exit(0);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		int result = super.onStartCommand(intent, flags, startId);

		if(progressBuilder == null) {
			progressBuilder = new NotificationCompat.Builder(this, Globals.ForegroundChannelId)
				.setContentTitle(getString(R.string.update_progress_title))
				.setContentText(getString(R.string.update_progress_text))
				.setContentInfo(getString(R.string.app_name))
				.setLargeIcon(BitmapFactory.decodeResource(getResources(), R.mipmap.ic_launcher))
				.setSmallIcon(R.drawable.ic_notification)
				.setColor(Globals.NormalColor)
				.setOngoing(true)
				.setProgress(0, 0, true)
				.setCategory(NotificationCompat.CATEGORY_PROGRESS);

			if(!Globals.isOreo())
				progressBuilder.setPriority(NotificationCompat.PRIORITY_MIN);
		}

		startForeground(Globals.ForegroundId, progressBuilder.build());
		handleStart();
		return result;
	}

	public void completeAction() {
		stopForeground(true);
		stopService(new Intent(this, AniremService.class));//Stop myself
	}

	public void updateProgress(int current, int max)
	{
		NotificationManager manager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
		progressBuilder.setProgress(max, current, false)
			.setContentText(getString(R.string.update_progress_active_text) + current + " / " + max);
		manager.notify(Globals.ForegroundId, progressBuilder.build());
	}

	public void showUpdateNotification(boolean success, String title, String message) {
		NotificationManager manager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);

		Intent openIntent = new Intent(this, AniremActivity.class);
		PendingIntent pending = PendingIntent.getActivity(this,
			Globals.OpenIntentId,
			openIntent,
			PendingIntent.FLAG_UPDATE_CURRENT);

		NotificationCompat.Builder builder = new NotificationCompat.Builder(this, Globals.NormalChannelId)
			.setContentTitle(title)
			.setContentText(message)
			.setContentInfo(getString(R.string.app_name))
			.setStyle(new NotificationCompat.BigTextStyle()
				.bigText(message))
			.setContentIntent(pending)
			.setLargeIcon(BitmapFactory.decodeResource(getResources(), R.mipmap.ic_launcher))
			.setSmallIcon(R.drawable.ic_notification)
			.setColor(Globals.NormalColor)
			.setAutoCancel(true)
			.setCategory(success ? NotificationCompat.CATEGORY_EVENT : NotificationCompat.CATEGORY_ERROR);

		if(!Globals.isOreo())
			builder.setDefaults(Notification.DEFAULT_ALL);

		manager.notify(Globals.NotifyId, builder.build());
	}

	private static native void handleStart();
}
