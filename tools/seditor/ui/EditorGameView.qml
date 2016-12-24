import QtQuick 2.7
import sigma 1.0

GameView {
    id: gameView
    activeContext: context

    DefaultTrackballController {
        id: defaultController
        anchors.fill: parent
    }
    Connections {
        target: defaultController
        onViewChanged: gameView.update()
    }
    controller: defaultController
}
