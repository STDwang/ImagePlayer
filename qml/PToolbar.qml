import QtQuick 2.1
import Toou2D 1.0
import QtQuick.Controls 2.0
import Qt.labs.platform 1.0 as Labs //名称冲突了要用as，注意Labs要大写开头
import "./common"
ToolBar {
    id: lefttoolbar
    signal getImageSize(string uri);
    signal openImage(string uri, var w, var h, var d, var reverse);
    signal saveImage(string uri);
    signal autoImage();
    signal reverseImage();
    signal denoiseImage();
    signal histImage();

    signal openImageConfigDia(string uri);
    onOpenImageConfigDia: {
        openimagedia.srcPath = uri;
        var temp = uri.split("/");
        openimagedia.rawName = temp[temp.length-1];
        openimagedia.open();
    }

    ImageConfigDialog {
        id: openimagedia
        onIsOK: {
            openImage(openimagedia.srcPath, w, h, d, reverse);
        }
    }

    Labs.FileDialog{
        id: openImageDialog
        title: qsTr("打开图片")
        nameFilters: ["image files (*.png *.jpg *.jpeg *.bmp *.raw)"]
        acceptLabel: qsTr("确定")
        rejectLabel: qsTr("取消")
        fileMode: Labs.FileDialog.OpenFile
        onAccepted:
        {
            if(openImageDialog.files[0].split(".")[1] == "raw"){
                getImageSize(openImageDialog.files[0]);
                var temp = rawConfig.tempPath.split("/");
                openimagedia.rawName = temp[temp.length-1];
                openimagedia.srcPath = openImageDialog.files[0];
                openimagedia.open();
            }else{
                rawConfig.tempPath = openImageDialog.files[0];
                //假的，发过去也不用它，懒得新开一个接口
                openImage(openImageDialog.files[0], 800, 600, 24, true);
            }
        }
    }

    Labs.FileDialog{
        id: saveDialog
        title: qsTr("保存图片")
        nameFilters: ["image files (*.png *.jpg *.jpeg *.bmp *.raw)"]
        acceptLabel: qsTr("确定")
        rejectLabel: qsTr("取消")
        fileMode: Labs.FileDialog.SaveFile
        onAccepted:
        {
            saveImage(saveDialog.currentFile);
        }
    }

    Column {
        anchors.fill: parent
        spacing: 10
        MouseArea {
            height: 30
            width: parent.width
            hoverEnabled: true;
            TIconButton {
                width: parent.width
                icon.source: TAwesomeType.FA_file_image_o
                icon.position: TPosition.Only;
                icon.color: "#C7C7C7"
                backgroundComponent: null;
                onClicked: {
                    openImageDialog.open();
                }
            }
            ToolTip {
                id: openImageTip
                delay: 500              //tooltip 500ms后出现
                timeout: 5000           //tooltip 5s后自动消失
                text: qsTr("打开图片")
                background: Rectangle {
                    border.color: "#373E47"
                    radius: 4
                }
            }
            onEntered: {
                openImageTip.visible = true;
            }
            onExited: {
                openImageTip.visible = false;
            }
        }
        MouseArea {
            height: 30
            width: parent.width
            hoverEnabled: true;
            TIconButton {
                width: parent.width
                icon.source: TAwesomeType.FA_save
                icon.position: TPosition.Only;
                icon.color: "#C7C7C7"
                backgroundComponent: null;
                onClicked: {
                    if(rawConfig.imgPath != ""){
                        saveDialog.open();
                    }else{
                        TToast.showInfo("操作区无图片",TTimePreset.LongTime4s, "请先打开图片并进行编辑");
                    }
                }
            }
            ToolTip {
                id: saveImageTip
                delay: 500              //tooltip 500ms后出现
                timeout: 5000           //tooltip 5s后自动消失
                text: qsTr("保存图片")
                background: Rectangle {
                    border.color: "#373E47"
                    radius: 4
                }
            }
            onEntered: {
                saveImageTip.visible = true;
            }
            onExited: {
                saveImageTip.visible = false;
            }
        }
        MouseArea {
            height: 30
            width: parent.width
            hoverEnabled: true;
            TIconButton {
                width: parent.width
                icon.source: TAwesomeType.FA_blind
                icon.position: TPosition.Only;
                icon.color: "#C7C7C7"
                backgroundComponent: null;
                onClicked: {
                    if(rawConfig.imgPath != ""){
                        autoImage();
                    }else{
                        TToast.showInfo("操作区无图片",TTimePreset.LongTime4s, "请先打开图片并进行编辑");
                    }
                }
            }
            ToolTip {
                id: autoImageTip
                delay: 500              //tooltip 500ms后出现
                timeout: 5000           //tooltip 5s后自动消失
                text: qsTr("auto")
                background: Rectangle {
                    border.color: "#373E47"
                    radius: 4
                }
            }
            onEntered: {
                autoImageTip.visible = true;
            }
            onExited: {
                autoImageTip.visible = false;
            }
        }
        MouseArea {
            height: 30
            width: parent.width
            hoverEnabled: true;
            TIconButton {
                width: parent.width
                icon.source: TAwesomeType.FA_assistive_listening_systems
                icon.position: TPosition.Only;
                icon.color: "#C7C7C7"
                backgroundComponent: null;
                onClicked: {
                    if(rawConfig.imgPath != ""){
                        reverseImage();
                    }else{
                        TToast.showInfo("操作区无图片",TTimePreset.LongTime4s, "请先打开图片并进行编辑");
                    }
                }
            }
            ToolTip {
                id: reverseImageTip
                delay: 500              //tooltip 500ms后出现
                timeout: 5000           //tooltip 5s后自动消失
                text: qsTr("reverse")
                background: Rectangle {
                    border.color: "#373E47"
                    radius: 4
                }
            }
            onEntered: {
                reverseImageTip.visible = true;
            }
            onExited: {
                reverseImageTip.visible = false;
            }
        }
        MouseArea {
            height: 30
            width: parent.width
            hoverEnabled: true;
            TIconButton {
                width: parent.width
                icon.source: TAwesomeType.FA_angle_double_up
                icon.position: TPosition.Only;
                icon.color: "#C7C7C7"
                backgroundComponent: null;
                onClicked: {
                    if(rawConfig.imgPath != ""){
                        denoiseImage();
                    }else{
                        TToast.showInfo("操作区无图片",TTimePreset.LongTime4s, "请先打开图片并进行编辑");
                    }
                }
            }
            ToolTip {
                id: denoiseTip
                delay: 500              //tooltip 500ms后出现
                timeout: 5000           //tooltip 5s后自动消失
                text: qsTr("denoise")
                background: Rectangle {
                    border.color: "#373E47"
                    radius: 4
                }
            }
            onEntered: {
                denoiseImageTip.visible = true;
            }
            onExited: {
                denoiseImageTip.visible = false;
            }
        }
        MouseArea {
            height: 30
            width: parent.width
            hoverEnabled: true;
            TIconButton {
                width: parent.width
                icon.source: TAwesomeType.FA_align_justify
                icon.position: TPosition.Only;
                icon.color: "#C7C7C7"
                backgroundComponent: null;
                onClicked: {
                    if(rawConfig.imgPath != ""){
                        histImage();
                    }else{
                        TToast.showInfo("操作区无图片",TTimePreset.LongTime4s, "请先打开图片并进行编辑");
                    }
                }
            }
            ToolTip {
                id: histTip
                delay: 500              //tooltip 500ms后出现
                timeout: 5000           //tooltip 5s后自动消失
                text: qsTr("直方图")
                background: Rectangle {
                    border.color: "#373E47"
                    radius: 4
                }
            }
            onEntered: {
                histTip.visible = true;
            }
            onExited: {
                histTip.visible = false;
            }
        }
    }
}
