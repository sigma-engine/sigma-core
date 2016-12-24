import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.0
import sigma 1.0

ApplicationWindow {
    visible: true
    width: Screen.width/2
    height: Screen.height/2
    title: qsTr("seditor")

    Material.theme: Material.Light
    Material.accent: Material.color(Material.Pink)
    Material.primary: Material.color(Material.Cyan)

    EditorGameView {
		id: gameView1
        anchors.fill: parent
    }

	Timer {
		interval: 16; 
		running: true; 
		repeat: true
		onTriggered: gameView1.update();
	}
}
