import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0 as Labs
import Toou2D 1.0

import "./qml/common"
import "./qml"

ApplicationWindow {
    id: root
    visible: true
    minimumWidth: 600
    minimumHeight: 400
    width: 1600
    height: 820
    title: qsTr("ImagePlayer")
    color: "#00000000"
    flags: Qt.Window | Qt.FramelessWindowHint   //去标题栏

    Rectangle{
        anchors.fill:parent
        radius:10
        color: "#1E1E1E"
        Topbar{
            id:topbar;
            width: parent.width;
            height: 35;
            color: "#323233"
            anchors.top: parent.top
            radius: 10
            Rectangle{
                z:-1
                y:parent.y + parent.height/2
                height: parent.height/2
                width: parent.width
                color: parent.color
            }
        }

        Footerbar{
            id:footerbar
            width: parent.width;
            height: 30;
            color: "#323233"
            anchors.bottom: parent.bottom;
            xValue: rawConfig.x
            yValue: rawConfig.y
            vValue: rawConfig.v
            radius: 10
            Rectangle{
                z:-1
                height: parent.height/2
                width: parent.width
                color: parent.color
            }
        }

        Rectangle {
            color: "#1E1E1E"
            anchors {
                top: topbar.bottom
                bottom: footerbar.top
                left: parent.left
                right: parent.right
            }
            Index {
                id: view
                rowCount: tablesizedia.rowCount;
                columnCount: tablesizedia.columnCount;
                anchors.fill: parent
            }
            Component.onCompleted: {
                tablesizedia.open();
            }
        }
    }

    T2DWorld {
        mouseAreaCursorShape: Qt.PointingHandCursor
        appStartupTheme: "Dark"
    }

    //存储全局变量。
    property QtObject rawConfig: QtObject {
        property bool   isOpened:       false;
        property bool   isOpening:      false;
        property string tempPath:       "";
        property string imgPath:        "";
        property real   size:           0;

        property real   x:              0;
        property real   y:              0;
        property real   v:              0;

        property real   min:            0;
        property real   max:            0;
        property real   autoMin:        0;
        property real   autoMax:        0;
        property var    hist:           [];
    }

    PenTableSizeDialog{
        property bool isSelected : false;
        id: tablesizedia
        onIsOk: {
            isSelected = true;
            tablesizedia.close();
            imagesever.setViewSize(rowCount, columnCount);
        }
    }
}
