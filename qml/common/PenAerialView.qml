import QtQuick 2.0

Rectangle {
    id:aerRecBac
    property var  source: aerialViewImage.source;
    property real srcHeight;
    property real srcWidth;
    property real viewHeight;
    property real viewWidth;
    property var  biliX;
    property var  biliY;

    signal mousePositionChanged(var aerbiliX, var aerbiliY, var mouse);
    signal mouseClicked(var mouse);
    signal mousePressed(var mouse);
    signal mouseReleased(var mouse);
    signal mouseEntered();
    signal mouseExited();
    signal setSrcMouseEnabled(var en);

    signal setROI(var x, var y);
    onSetROI: {
        aerRec.x = x;
        aerRec.y = y;
    }

    Image {
        id: aerialViewImage
        source: aerRecBac.source;
        anchors.fill: parent
        anchors.margins: 2
        clip: true
        Rectangle {
            id: aerRec
            opacity: 0.5
            color: "#409EFF"
            width: srcHeight > viewHeight || srcWidth > viewWidth ? viewWidth / srcWidth * aerialViewImage.width : 0
            height: srcHeight > viewHeight || srcWidth > viewWidth ? viewHeight / srcHeight * aerialViewImage.height : 0
            Component.onCompleted: {
                aerRec.x = aerialViewImage.x + aerialViewImage.width * biliX - aerRec.width / 2;
                aerRec.y = aerialViewImage.y + aerialViewImage.height * biliY - aerRec.height / 2;
            }
        }
        MouseArea {
            id: aerMouse;
            property double aerbiliX : 1.0;
            property double aerbiliY : 1.0;
            z:2
            anchors.fill: aerialViewImage;
            drag.target: aerRec;
            drag.minimumX: - aerRec.width * 0.98;
            drag.minimumY: - aerRec.height * 0.98;
            drag.maximumX: aerialViewImage.width - aerRec.width * 0.02;
            drag.maximumY: aerialViewImage.height - aerRec.height * 0.02;
            //悬停
            hoverEnabled: true
            onPositionChanged:{
                aerbiliX = (aerRec.x + aerRec.width / 2) / aerialViewImage.width;
                aerbiliY = (aerRec.y + aerRec.height / 2) / aerialViewImage.height;
                mousePositionChanged(aerbiliX, aerbiliY, mouse);
            }
            onClicked: {
                aerRec.x = mouseX - aerRec.width / 2;
                aerRec.y = mouseY - aerRec.height / 2;
                mouseClicked(mouse);
            }
            onEntered: {
                mouseEntered();
            }
            onExited: {
                mouseExited();
            }
            onPressed: {
                mousePressed(mouse);
            }
            onReleased: {
                mouseReleased(mouse);
            }
        }
    }
}
