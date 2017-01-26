package de.skycoder42.seasonproxer;

import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.support.v4.app.NotificationCompat;
import android.widget.Toast;
import org.qtproject.qt5.android.bindings.QtService;

public class SeasonProxerService extends QtService {
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		NotificationCompat.Builder builder = new NotificationCompat.Builder(this)
						.setContentTitle("ServiceTest")
						.setSmallIcon(R.drawable.ic_launcher);

		startForeground(123, builder.build());
		super.onStartCommand(intent, flags, startId);
		return Service.START_NOT_STICKY;
	}

	public void showTheToast() {
		Toast.makeText(this, "THE TOAST", Toast.LENGTH_SHORT).show();
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
}
