import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
Item {
    id: beishuTip
    property var text: beishuTipLabel.text
    signal timerStart();
    onTimerStart: {
        beishuTimer.start();
    }

    Glow{
        source: tipRec
        anchors.fill: tipRec
        radius: 14
        samples: 16
        color: "#F2F6FC"
    }
    Timer{
        id: beishuTimer;
        interval: 1500;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            beishuTip.visible = false;
        }
    }
    Rectangle {
        id: tipRec
        color: "black"
        anchors.fill: parent
        radius: 15
        Label {
            id: beishuTipLabel
            anchors.centerIn: parent
            text: beishuTip.text
            font.pixelSize: 16
            font.bold: true
            color: "white"
        }
    }
}
