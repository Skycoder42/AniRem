package de.skycoder42.anirem;

import android.os.Bundle;
import android.os.IBinder;

import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.ComponentName;

import android.app.NotificationManager;
import android.app.NotificationChannel;

import org.qtproject.qt5.android.bindings.QtActivity;

public class AniremActivity extends QtActivity {
	private boolean _isBound;
	private ServiceConnection _connection = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {
			_isBound = true;
		}

		@Override
		public void onServiceDisconnected(ComponentName className) {
			_isBound = false;
		}
	};

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		createChannels();

		Intent intent = new Intent(this, AniremService.class);
		bindService(intent, _connection, Context.BIND_AUTO_CREATE);
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		if (_isBound) {
			unbindService(_connection);
			_isBound = false;
		}
	}

	private void createChannels() {
		if(!Globals.isOreo())
			return;

		NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

		//create normal channel
		NotificationChannel normal = new NotificationChannel(Globals.NormalChannelId,
				getString(R.string.channel_nrm_name),
				NotificationManager.IMPORTANCE_DEFAULT);
		normal.setDescription(getString(R.string.channel_nrm_desc));
		normal.enableLights(true);
		normal.setLightColor(Globals.NormalColor);
		normal.enableVibration(true);
		normal.setShowBadge(true);
		manager.createNotificationChannel(normal);

		//create foreground channel
		NotificationChannel foreground = new NotificationChannel(Globals.ForegroundChannelId,
				getString(R.string.channel_svc_name),
				NotificationManager.IMPORTANCE_MIN);
		foreground.setDescription(getString(R.string.channel_svc_desc));
		foreground.enableLights(false);
		foreground.enableVibration(false);
		foreground.setShowBadge(false);
		manager.createNotificationChannel(foreground);
	}
}
