import QtQuick 2.8
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0 //DEBUG failed to detect by android deploy Qt
import QtQuick.Controls.Material 2.1
import de.skycoder42.qtmvvm 1.0

App {
	Material.theme: Material.Dark

	Material.primary: "#777777"
	Material.accent: "#8A0E0E"
	Material.background: "#4E4E4E"

	//Material.primary: Material.color(Material.Grey, Material.Shade600)
	//Material.accent: Material.color(Material.Red, Material.Shade900)
	//Material.background: Material.color(Material.Grey, Material.Shade800)
}
