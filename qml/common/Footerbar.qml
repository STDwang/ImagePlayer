import QtQuick 2.6
import QtQuick.Controls 2.0
Rectangle{
    id: footbarBac
    anchors.bottom: parent.bottom;
    color: "#333333"

    property int xValue: 0;
    property int yValue: 0;
    property int vValue: 0;

    Row {
        anchors.fill: parent
        Item {
            height: parent.height
            width: 10
        }
        Item {
            height: xLabel.height
            width: parent.width * 0.1
            anchors.verticalCenter: parent.verticalCenter
            Label{
                id: xLabel
                text: "x: " + xValue;
                color: "#FFF";
                font.bold: true;
                font.pixelSize: 15;
            }
        }
        Item {
            height: yLabel.height
            width: parent.width * 0.1
            anchors.verticalCenter: parent.verticalCenter
            Label{
                id: yLabel
                text: "y: " + yValue;
                color: "#FFF";
                font.bold: true;
                font.pixelSize: 15;
            }
        }
        Item {
            height: vLabel.height
            width: parent.width * 0.1
            anchors.verticalCenter: parent.verticalCenter
            Label{
                id: vLabel
                text: "v: " + vValue;
                color: "#FFF";
                font.bold: true;
                font.pixelSize: 15;
            }
        }
    }
}
