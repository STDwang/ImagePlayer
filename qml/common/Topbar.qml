import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import Toou2D 1.0
Rectangle{
    signal showMenu();
    color: "#252526"//"#2D333B"
    property string title: "ImagePlayer";
    Row {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10
        spacing: 10
        TImage {
            width: 20
            height: 20
            source: "qrc:/res/png/sate(1).png"
            anchors.verticalCenter: parent.verticalCenter
        }
        TLabel {
            text: title
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: TPixelSizePreset.PH4
            color: "#CDD9D5"
            font.bold: true
        }
    }

    TLabel {
        text: "当前：" + rawConfig.imgPath
        anchors.centerIn: parent
        font.pixelSize: TPixelSizePreset.PH4
        color: "#CDD9D5"
        font.bold: true
        visible: rawConfig.imgPath != ""
    }

    //记录鼠标移动的位置，此处变量过多会导致移动界面变卡
    property point clickPos: "0,0"

    //处理鼠标移动后窗口坐标逻辑
    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton  //只处理鼠标左键
        onDoubleClicked: {
            if(root.visibility === Window.Maximized){
                root.showNormal();          //窗口平时态
                maxWinButton.icon.source = "qrc:/res/svg/max.svg";
            }else {
                root.showMaximized()        //窗口最大化
                maxWinButton.icon.source = "qrc:/res/svg/min.svg";
            }
        }

        onPressed: {
            clickPos = Qt.point(mouse.x, mouse.y)
        }
        onPositionChanged: {
            //计算鼠标移动的差值
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
            //设置窗口坐标
            root.setX(root.x + delta.x)
            root.setY(root.y + delta.y)
        }
    }

    Row {
        spacing: 10
        x: parent.width - width - 10;
        Behavior on x {
            NumberAnimation {
                duration: 100
            }
        }
        MouseArea {
            height: parent.height
            width: minWinButton.width
            hoverEnabled: true;
            TIconButton {
                width: parent.width
                icon.source: TAwesomeType.FA_th
                icon.position: TPosition.Only;
                icon.color: "#C7C7C7"
                backgroundComponent: null;
                onClicked: {
                    tablesizedia.open();
                }
            }
            ToolTip {
                id: viewTip
                delay: 500              //tooltip 500ms后出现
                timeout: 5000           //tooltip 5s后自动消失
                text: qsTr("视图布局")
                background: Rectangle {
                    border.color: "#373E47"
                    radius: 4
                }
            }
            onEntered: {
                viewTip.visible = true;
            }
            onExited: {
                viewTip.visible = false;
            }
        }
        //最小化窗口按钮
        TIconButton{
            id:minWinButton
            icon.position: TPosition.Only;
            icon.source: "qrc:/res/svg/hide.svg";
            icon.color: "#9BB3C7"
            backgroundComponent: null;
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                root.showMinimized()        //窗口最小化
            }
        }
        //最大化窗口按钮
        TIconButton{
            id:maxWinButton
            icon.position: TPosition.Only;
            icon.source: "qrc:/res/svg/max.svg";
            icon.color: "#9BB3C7"
            backgroundComponent: null;
            anchors.verticalCenter: minWinButton.verticalCenter
            onClicked: {
                if(root.visibility === Window.Maximized){
                    root.showNormal();          //窗口平时态
                    maxWinButton.icon.source = "qrc:/res/svg/max.svg";
                }else {
                    root.showMaximized()        //窗口最大化
                    maxWinButton.icon.source = "qrc:/res/svg/min.svg";
                }
            }
        }
        //关闭窗口按钮
        TIconButton{
            id:closeWinButton
            icon.position: TPosition.Only;
            icon.source: "qrc:/res/svg/close.svg";
            icon.color: "#9BB3C7"
            backgroundComponent: null;
            anchors.verticalCenter: minWinButton.verticalCenter
            onClicked: {
                Qt.quit();               //退出程序
            }
        }
    }
}
