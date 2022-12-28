import QtQuick 2.2
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import Toou2D 1.0
import "./common"
Item {
    id: viewRoot
    property int    rowCount:   1;
    property int    columnCount:1;

    property int    rowImageIndex: 0;
    property int    columnImageIndex: 0;

    property double beishu :    1.0;
    property int    old_x:      0.0;
    property int    old_y:      0.0;
    property double biliX :     1.0;
    property double biliY :     1.0;
    property bool   isPress:    false;

    property real   clickNum :  0;
    property real   startX :    0;
    property real   startY :    0;
    property real   stopX :     0;
    property real   stopY :     0;
    property double startXBili :1.0;
    property double startYBili :1.0;
    property double stopXBili : 1.0;
    property double stopYBili : 1.0;

    property real   tempStartX :0;
    property real   tempStartY :0;
    property real   tempStopX : 0;
    property real   tempStopY : 0; 

    property real   imageX:     0;
    property real   imageY:     0;
    property real   imageHeight:0;
    property real   imageWidth: 0;

    PToolbar {
        id: lefttoolbar
        z: 3
        height: parent.height
        width: parent.width * 0.02
        background: Rectangle{
            color: "#2D2D2D"
            anchors.fill: parent
        }
        onGetImageSize: {
            global.tempPath = [];
            global.tempPath.push(uri);
            imagesever.getImageSize(uri);
        }
        onOpenImage: {
            rawConfig.isOpening = true;
            rawConfig.isOpened = false;
            imagesever.openImage(rowImageIndex, columnImageIndex, uri, w, h, d, reverse);
        }
        onSaveImage: {
            imagesever.saveImage(rowImageIndex, columnImageIndex, uri, "png");
            rawConfig.isOpening = true;
        }
    }
    TBusyIndicator{
        id: openingBusy
        z:3
        visible: rawConfig.isOpening
        anchors.centerIn: parent;
    }
    Column {
        id: viewColumn
        spacing: 5
        anchors.margins: 10
        anchors {
            left: lefttoolbar.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        Repeater{
            model: rowCount
            delegate: Row {
                property real cIndex: index
                spacing: 5
                Repeater{
                    model: columnCount
                    delegate: Rectangle{
                        id: stackImageRec
                        color: "#252526"
                        width: (viewColumn.width - (columnCount - 1) * 5) / columnCount
                        height: (viewColumn.height - (rowCount - 1) * 5) / rowCount
                        radius: 5
                        clip: true
                        QtObject {
                            id: privateImageData
                            property real   row: cIndex;
                            property real   col: index;
                            property string path: ""
                            property real   width: 0;
                            property real   height: 0;
                        }
                        Connections {
                            target: imagesever
                            onSendImageToUI: {
                                if(cIndex == row && index == col){
                                    privateImageData.height = h;
                                    privateImageData.width = w;
                                    stackImage.source = "";
                                    stackImage.source = "image://CodeImg/" + String(privateImageData.row) + "_" + String(privateImageData.col) + "###" + Date.now();
                                    privateImageData.path = global.tempPath[0];
                                    rawConfig.imgPath = privateImageData.path;
                                }
                            }
                        }
                        PenZoomTip {
                            id: beishuTip
                            height: 30
                            width: 100
                            z: 3
                            visible: false;
                            opacity: 0.5
                            anchors.centerIn: parent
                        }
                        PenAerialView {
                            id:aerialView
                            z: 2
                            radius: 5
                            color: "#FFFFFF"
                            width: parent.width * 0.16
                            height: imageHeight / imageWidth * width
                            visible: stackImage.source != "" ? true : false;
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                margins: 5
                            }
                            source:     stackImage.source
                            srcHeight:  stackImage.height
                            srcWidth:   stackImage.width
                            viewHeight: stackImageRec.height
                            viewWidth:  stackImageRec.width
                            biliX:      viewRoot.biliX
                            biliY:      viewRoot.biliY
                            onMousePositionChanged: {
                                if(isPress){
                                    imageX = - stackImage.width * aerbiliX + stackImageRec.width / 2;
                                    imageY = - stackImage.height * aerbiliY + stackImageRec.height / 2;
                                }
                            }
                            onMouseClicked: {
                                imageX = - stackImage.width * (mouse.x / aerialView.width) + stackImageRec.width / 2;
                                imageY = - stackImage.height * (mouse.y / aerialView.height) + stackImageRec.height / 2;
                            }
                            onMouseEntered: {
                                rawImageMouseArea.enabled = false;
                            }
                            onMouseExited: {
                                rawImageMouseArea.enabled = true;
                            }
                            onMousePressed: {
                                isPress = true;
                            }
                            onMouseReleased: {
                                isPress = false;
                            }
                        }
                        Image {
                            id: stackImage;
                            x: imageX;
                            y: imageY;
                            width: source != ""?imageWidth:0;
                            height: source != ""?imageHeight:0;
                            smooth: false;
                            cache:false;
                            onXChanged: {
                                //控制鸟瞰图RoI区域相对坐标
                                aerialView.setROI(-imageX/imageWidth * aerialView.width, -imageY/imageHeight * aerialView.height);
                            }
                            onYChanged: {
                                //控制鸟瞰图RoI区域相对坐标
                                aerialView.setROI(-imageX/imageWidth * aerialView.width, -imageY/imageHeight * aerialView.height);
                            }
                        }
                        MouseArea {
                            id: rawImageMouseArea
                            anchors.fill: stackImage
                            drag.target: stackImage
                            drag.minimumX: - imageWidth * 0.98
                            drag.minimumY: - imageHeight * 0.98
                            drag.maximumX: parent.width - imageWidth * 0.02
                            drag.maximumY: parent.height - imageHeight * 0.02
                            hoverEnabled: true
                            propagateComposedEvents: true
                            onClicked: {
                                rawConfig.imgPath = privateImageData.path;
                                rowImageIndex = privateImageData.row;
                                columnImageIndex = privateImageData.col;
                            }
                            onPositionChanged: {
                                //拖拽时锁定
                                imageX = stackImage.x;
                                imageY = stackImage.y;

                                biliX = mouseX / imageWidth;
                                biliY = mouseY / imageHeight; 
                                var rawx = mouseX / beishu;
                                var rawy = mouseY / beishu;
                                //数据边界检查
                                if (rawx <= 0) rawx = 0;
                                if (rawy <= 0) rawy = 0;
                                if (rawx >= privateImageData.width - 1) rawx = privateImageData - 1;
                                if (rawy >= privateImageData.height - 1) rawy = privateImageData.height - 1;
                                //得到raw图像中对应的灰度值
                                imagesever.getPixValue(privateImageData.row, privateImageData.col, rawx, rawy);
                            }
                            onWheel: {
                                var datla = wheel.angleDelta.y / 120;
                                old_x = imageX + imageWidth * biliX;
                                old_y = imageY + imageHeight * biliY;
                                if(datla > 0)
                                {
                                    beishu = beishu / 0.9;
                                    if(beishu > 30) {
                                        beishu = beishu * 0.9;
                                        beishuTip.text = "已达到最大";
                                        beishuTip.visible = true;
                                        beishuTip.timerStart();
                                        return;
                                    }
                                }
                                else
                                {
                                    beishu = beishu * 0.9;
                                    if(beishu < 0.2) {
                                        beishu = beishu / 0.9;
                                        beishuTip.text = "已达到最小";
                                        beishuTip.visible = true;
                                        beishuTip.timerStart();
                                        return;
                                    }
                                }
                                //请求cpp将缩放倍数设置为beishu
                                imagesever.setImageScaled(beishu);

                                imageHeight = privateImageData.height * beishu;
                                imageWidth = privateImageData.width * beishu;
                                imageX = old_x - imageWidth * biliX;
                                imageY = old_y - imageHeight * biliY;
                                startX = Math.floor(imageWidth * startXBili);
                                startY = Math.floor(imageHeight * startYBili);
                                stopX = Math.floor(imageWidth * stopXBili);
                                stopY = Math.floor(imageHeight * stopYBili);
                                beishuTip.text = Math.round(beishu * 100).toString() + "%";
                                beishuTip.visible = true;
                                beishuTip.timerStart();

                                //强制刷新图像，重新申请缩放后的图像源（之前的图像源尺寸是不变的，缩放仅在qml端）
                                stackImage.source = "";
                                stackImage.source = "image://CodeImg/" + String(privateImageData.row) + "_" + String(privateImageData.col) + "###" + Date.now();
                            }
                        }
                        DropArea {
                            id: dropArea;
                            anchors.fill: parent
                            onEntered: {
                                drag.accept (Qt.LinkAction);
                            }
                            onDropped: {
                                rowImageIndex = privateImageData.row;
                                columnImageIndex = privateImageData.col;
                                global.tempPath = [];
                                for(var i=0;i<drop.urls.length;i++){
                                    global.tempPath.push(drop.urls[i]);
                                }
                                imagesever.getImageSize(drop.urls[0]);
                                lefttoolbar.openImageConfigDia(drop.urls[0]);
                            }
                        }
                        TIconButton{
                            id: tabNameClose
                            height: 30;
                            width: 30;
                            anchors.right: parent.right;
                            visible: stackImage.source == "" ? false : true
                            icon.source: TAwesomeType.FA_close;
                            icon.position: TPosition.Only;
                            icon.color: "#FAFAFA";
                            backgroundComponent: Rectangle{
                                color: "#1E1E1E"
                                radius: 5
                                anchors.fill: parent
                            }
                            onClicked: {
                                stackImage.source = "";
                                rawConfig.imgPath = "";
                                privateImageData.width = 0;
                                privateImageData.height = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    Connections {
        target: imagesever
        onSendImageToUI: {
            if(tip){
                TToast.showSuccess("打开成功",TTimePreset.LongTime4s, global.tempPath[0]);
                global.srcPath.push({
                                        "path":       global.tempPath[0],
                                        "size":       rawConfig.size,
                                        "height":     h,
                                        "width":      w
                                    });
                imageWidth = w * beishu;
                imageHeight = h * beishu;
            }else{
                TToast.showError("打开失败",TTimePreset.LongTime4s, global.tempPath[0]);
            }
            rawConfig.isOpening = false;
            rawConfig.isOpened = true;
        }
        onSendImageSizeToUI: {
            rawConfig.size = size;
        }
        onSendPixValueToUI: {
            rawConfig.x = x;
            rawConfig.y = y;
            rawConfig.v = v;
        }
        onSendSaveTipToUI: {
            if(tip){
                TToast.showSuccess("保存成功",TTimePreset.LongTime4s, global.srcPath[global.srcPath.length - 1].path);
            }else{
                TToast.showError("保存失败",TTimePreset.LongTime4s, "......");
            }
            rawConfig.isOpening = false;
            rawConfig.isOpened = true;
        }
    }
}
