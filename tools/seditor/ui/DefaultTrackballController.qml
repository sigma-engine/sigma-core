import QtQuick 2.7
import sigma 1.0

TrackballController {
    id: c
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        function convert(x0,y0) {
            var x1 = 2*((x0/width)-.5);
            var y1 = 2*((y0/width)-.5);
            return {"x":x1,"y":y1}
        }

        onPressed: {
            if (mouse.button === Qt.LeftButton) {
                var c = convert(mouseX,mouseY);
                if(mouse.modifiers & Qt.ShiftModifier)
                    parent.beginPan(c.x,c.y);
                else
                    parent.beginRotate(c.x,c.y);

            }
        }
        onWheel: {
            parent.zoom(wheel.angleDelta.y/120)
        }

        onPositionChanged:{
            var c = convert(mouseX,mouseY);
            parent.update(c.x,c.y);
        }
        onReleased: {
            if (mouse.button === Qt.LeftButton) {
                var c = convert(mouseX,mouseY);
                if(parent.isPanning())
                    parent.endPan(c.x,c.y);
                else
                    parent.endRotate(c.x,c.y);
            }
        }
    }
}
