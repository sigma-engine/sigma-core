import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.0
import sigma 1.0
import QtQuick.Window 2.0

ApplicationWindow {
    visible: true
    width: Screen.desktopAvailableWidth/2
    height: Screen.desktopAvailableHeight/2
    title: qsTr("seditor")

    Material.theme: Material.Light
    Material.accent: Material.color(Material.Pink)
    Material.primary: Material.color(Material.Cyan)

    GameView {
		id: gameView1
		anchors.fill: parent
        /*
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.right: parent.horizontalCenter
		anchors.left: parent.left
		*/
		//anchors.fill: parent
        activeContext: context
    }

	/*
	GameView {
		id: gameView2
        anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.left: parent.horizontalCenter
		anchors.right: parent.right
		//anchors.fill: parent
        activeContext: context
    }
	*/

	Timer {
		interval: 18; 
		running: true;
		repeat: true
		onTriggered: {
			gameView1.update()
			//gameView2.update()
		}
	}
}
