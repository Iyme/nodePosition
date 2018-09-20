
import QtQuick 2.0

Rectangle {
    width: 20
    height: 20
    color: "red"
    property string value

    Text{
        anchors.centerIn: parent
        text: value
    }
}
