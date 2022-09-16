import QtQuick 2.6
import QtQuick.Controls 2.0
import Toou2D 1.0
Rectangle{
    id: footbarBac
    anchors.bottom: parent.bottom;
    color: "#333333"

    property int xValue: 0;
    property int yValue: 0;
    property int vValue: 0;

    Row {
        height: parent.height
        width: parent.width / 2
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
    Row {
        height: parent.height
        anchors.centerIn: parent
        spacing: 5
        TIconButton {
            id: gitButton
            width: 30
            icon.source: TAwesomeType.FA_github
            icon.position: TPosition.Only;
            icon.color: "#C7C7C7"
            backgroundComponent: null;
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                Qt.openUrlExternally("https://github.com/STDwang/ImagePlayer");
            }
            onEntered: {
                gitTip.visible = true;
            }
            onExited: {
                gitTip.visible = false;
            }
            ToolTip {
                id: gitTip
                delay: 500              //tooltip 500ms后出现
                timeout: 5000           //tooltip 5s后自动消失
                text: qsTr("Github")
                background: Rectangle {
                    border.color: "#373E47"
                    radius: 4
                }
            }
        }
        Label {
            text: "author: stdWang"
            color: "#FFF";
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 15
        }
    }
}
