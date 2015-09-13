import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import sigmafive.editor.widgets 1.0
import QtQuick.Layouts 1.0
import Assimp 1.0

ApplicationWindow {

    visible: true
    title: "sigmafive editor"
    minimumWidth: 800
    minimumHeight: 600

    menuBar: MenuBar {
        Menu {
            title: "File"

            Item {
                AssimpImporter {
                    id:importer
                }
                FileDialog {
                    id: importDialog
                    folder: shortcuts.desktop
                    nameFilters: [ "3D Models (*.blend *.dae)" ]
                    onAccepted: {
                        importer.import_file(_entityManager,fileUrl)
                        _entityManager.layoutChanged()
                    }
                }
                MenuItem {
                    text: "Import"
                    onTriggered: importDialog.open()
                }
            }
        }
    }

    /*statusBar: ProgressBar {
        id: progressBar
        visible: false
        value: 0
    }*/


    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        ScrollView {
            width: 200
            Layout.minimumWidth: 200
            ListView {
                id:listView
                anchors.fill: parent
                model: _entityManager
                delegate: Text {
                    width: parent.width;
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listView.currentIndex = index
                        }
                    }

                    text: "Entity: " + ID
                }

                highlight:  Rectangle { width: parent.width; color: "lightsteelblue"}
            }
        }

        GameView {
            id: mainView
            //anchors.fill: parent
            color: "lightGrey"
            entityManager: _entityManager

            MouseArea {
                anchors.fill: mainView
                acceptedButtons: Qt.AllButtons;
                hoverEnabled: true

                onPressed: {
                    if(mouse.button == Qt.MiddleButton) {
                        mainView.begin_rotate(Qt.point(mouse.x,mouse.y));
                        mainView.update();
                    }
                }

                onReleased: {
                    if(mouse.button == Qt.MiddleButton) {
                        mainView.end_rotate(Qt.point(mouse.x,mouse.y));
                        mainView.update();
                    }
                }

                onPositionChanged: {
                    mainView.focus = true
                    mainView.mouse_moved(Qt.point(mouse.x,mouse.y));
                    mainView.update();
                }

                onWheel: {
                    mainView.wheel_scroll(wheel.angleDelta.y);
                }
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Shift) {
                    mainView.begin_pan();
                    mainView.update();
                }
            }
            Keys.onReleased: {
                if (event.key === Qt.Key_Shift) {
                    mainView.end_pan();
                    mainView.update();
                }
            }
        }
    }
}
