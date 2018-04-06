package de.skycoder42.anirem;

import android.os.Build;

public class Globals {
	// alarms
	public static final int AlarmIntentId = 11;
	public static final int OpenIntentId = 12;

	// preferences
	public static final String AutoStartPrefs = "de.skycoder42.anirem.preferences";
	public static final String AutStartKey = "de.skycoder42.anirem.autostart";

	// notifications channels
	public static final String NormalChannelId = "de.skycoder42.anirem.channel.normal";
	public static final String ForegroundChannelId = "de.skycoder42.anirem.channel.foreground";

	// notifications IDs
	public static final int ForegroundId = 21;
	public static final int NotifyId = 22;

	// colors
	public static final int NormalColor = 0xFF8A0E0E; // proxer red

	public static boolean isOreo() {
		return Build.VERSION.SDK_INT >= Build.VERSION_CODES.O;
	}
}
