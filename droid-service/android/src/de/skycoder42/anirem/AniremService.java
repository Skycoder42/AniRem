package de.skycoder42.anirem;

import android.content.Intent;

import android.graphics.BitmapFactory;

import android.support.v4.app.NotificationCompat;

import org.qtproject.qt5.android.bindings.QtService;

public class AniremService extends QtService {
	@Override
	public void onDestroy() {
		super.onDestroy();
		// explicitly exit to prevent the process from beeing cached
		System.exit(0);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		int result = super.onStartCommand(intent, flags, startId);

		NotificationCompat.Builder builder = new NotificationCompat.Builder(this, Globals.ForegroundChannelId)
			.setContentTitle(getString(R.string.update_progress_title))
			.setContentText(getString(R.string.update_progress_text))
			.setContentInfo(getString(R.string.app_name))
			.setLargeIcon(BitmapFactory.decodeResource(getResources(), R.mipmap.ic_launcher))
			.setSmallIcon(R.drawable.ic_notification)
			.setColor(Globals.NormalColor)
			.setOngoing(true)
			.setProgress(0, 0, true)
			.setCategory(NotificationCompat.CATEGORY_PROGRESS);

		if (!Globals.isOreo())
			builder.setPriority(NotificationCompat.PRIORITY_MIN);

		startForeground(Globals.ForegroundId, builder.build());
		return result;
	}

	public void completeAction() {
		stopForeground(true);
		stopService(new Intent(this, AniremService.class));//Stop myself
	}
}
