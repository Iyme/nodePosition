import QtQuick 2.9
import QtQuick.Window 2.2
import NodePosition.MyObject 1.0

import "script.js" as Script

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    MouseArea{
        anchors.fill: parent;
        onClicked: myObject.start()
    }

    Rectangle{
        id: viewArea
        Canvas {
            id: canvas
            anchors.fill: parent
            onPaint: {
                var ctx = getContext('2d')
                ctx.beginPath()
                ctx.strokeStyle = "black"
                Script.drawLine(ctx)
            }
        }

        anchors.fill: parent
        anchors.margins: 10
        border.color: "#ffff66"
        border.width: 1
    }

    MyObject{
        id:myObject
        onCreateNode:{
            var conponent = Qt.createComponent("singleNode.qml")
            if(conponent.status == Component.Ready)
            {
                var node = conponent.createObject(canvas)
                node.x = x
                node.y = y
                node.value = name
            }
        }
        onDrowLine:{
            Script.appendLine(x_pre, y_pre, x_new, y_new)
            canvas.requestPaint()
            console.log(x_pre, y_pre, x_new, y_new)
        }
    }



}
