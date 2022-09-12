import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import Toou2D 1.0

TDialog{
    id:openimagedia

    signal isOK(var h, var w, var d);
    property string rawName:               "";
    property string srcPath:               "";
    property bool   reverse:               false;

    property string bodyColor:             "#282A36";
    property string labelColor:            "#9BB3C7";//#9BB3C7
    property string headerColor:           "#0075FF";
    property string iconColor:             "white";//#9BB3C7
    property string inputBackgroundColor:  "#30323D";
    property string textAreaColor:         "#1C2128";
    property string backgroundColor:       "#347D39";
    property string diaBackgroundColor:    "#282A36";
    property string borderColor:           "#9BB3C7";//#606266
    property string fontfamily:            "微软雅黑";
    property real   labelPixSize:          14;
    property real   rowHeight:             30;
    property real   rowWidth:              450;
    property real   radiusNumber:          5;

    bodyComponent:  TRectangle{
        theme.enabled: false;
        color: bodyColor
        width: 500;
        height: 400;
        radius: radiusNumber
        clip: true;
        Column{
            spacing: 18;
            width: parent.width;
            clip: true;
            Item{
                width: parent.width;
                height: 12;
                TIconButton{
                    icon.position: TPosition.Only;
                    icon.source: TAwesomeType.FA_close;
                    icon.color: iconColor
                    backgroundComponent: null;
                    anchors.right: parent.right;
                    onClicked: openimagedia.hideAndClose();
                }
            }
            Label {
                text: qsTr("Import Raw:" + rawName)
                clip: true
                color: headerColor
                font.pixelSize: 28
                font.family: fontfamily
                anchors.left: parent.left;
                anchors.leftMargin: parent.width * 0.05
            }

            Row {
                id: imageDepthRow
                anchors.horizontalCenter: parent.horizontalCenter
                height: rowHeight
                width: rowWidth
                spacing: 20
                Label {
                    id: imageDepthLabel
                    text: qsTr("图像深度:");
                    font.pixelSize: labelPixSize
                    font.family: fontfamily
                    color: labelColor
                }
                PenComboBox {
                    id: imageDepth
                    height: rowHeight
                    width: rowWidth * 0.8
                    fontSize: 12
                    textColor: labelColor
                    backgoundColor: inputBackgroundColor
                    backgoundRadius: radiusNumber
                    anchors.verticalCenter: imageDepthLabel.verticalCenter
                    model: ["16-bit Unsigned"]
                }
            }

            Row {
                height: rowHeight
                width: rowWidth
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Label {
                    id: pixNumberLabel
                    text: qsTr("像素个数:");
                    font.pixelSize: labelPixSize
                    font.family: fontfamily
                    color: labelColor
                }
                TInputField{
                    id: imagePixNumber;
                    background.radius: 10;
                    background.color: inputBackgroundColor
                    width: rowWidth * 0.8
                    height: parent.height
                    text: imageDepthRow.visible ? (rawConfig.size / Number(imageDepth.displayText.split("-")[0]) * 8).toString():rawConfig.size.toString()
                    label.color: labelColor;
                    cursorColor: labelColor;

                    placeholderPosition: TPosition.Left;
                    placeholderLabel.text: "请输入参数";
                    placeholderLabel.color: labelColor
                    border.width: 0
                    anchors.verticalCenter: pixNumberLabel.verticalCenter
                    theme.enabled: false;
                    enabled: false;
                }
            }

            Row {
                height: rowHeight
                width: rowWidth
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Label {
                    id: imageHeightLabel
                    text: qsTr("高度(px):");
                    font.pixelSize: labelPixSize
                    font.family: fontfamily
                    color: labelColor
                }
                TInputField{
                    id: imageHeight;
                    background.radius: 10;
                    background.color: inputBackgroundColor
                    width: rowWidth * 0.8
                    height: parent.height
                    text: "1568"
                    label.color: labelColor;
                    cursorColor: labelColor;

                    placeholderPosition: TPosition.Left;
                    placeholderLabel.text: "请输入参数";
                    placeholderLabel.color: labelColor
                    border.width: 0
                    anchors.verticalCenter: imageHeightLabel.verticalCenter
                    theme.enabled: false;
//                    onTextChanged: {
//                        if(imageHeight.text.length == 4 && Number(imagePixNumber.text) !== 0){
//                            imageWidth.text = (Number(imagePixNumber.text) / Number(imageHeight.text)).toString();
//                        }
//                    }
                }
            }

            Row {
                height: rowHeight
                width: rowWidth
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Label {
                    id: imageWidthLabel
                    text: qsTr("宽度(px):");
                    font.pixelSize: labelPixSize
                    font.family: fontfamily
                    color: labelColor
                }
                TInputField{
                    id: imageWidth;
                    background.radius: 10;
                    background.color: inputBackgroundColor
                    width: rowWidth * 0.8
                    height: parent.height
                    text: "1024"
                    label.color: labelColor;
                    cursorColor: labelColor;

                    placeholderPosition: TPosition.Left;
                    placeholderLabel.text: "请输入参数";
                    placeholderLabel.color: labelColor
                    border.width: 0
                    anchors.verticalCenter: imageWidthLabel.verticalCenter
                    theme.enabled: false;
//                    onTextChanged: {
//                        if(imageWidth.text.length == 4 && Number(imagePixNumber.text) !== 0){
//                            imageHeight.text = (Number(imagePixNumber.text) / Number(imageWidth.text)).toString();
//                        }
//                    }
                }
            }
            TCheckBox {
                id: satechecked
                label.text: "反序"
                label.color: labelColor
                checked: false;
                x: parent.width / 2 - width/2
                onCheckedChanged: {
                    reverse = checked;
                }
            }
            Row {
                spacing: parent.width * 0.3
                anchors.horizontalCenter: parent.horizontalCenter
                TButton{
                    width: 80;
                    height: 32;
                    label.text: "确定"
                    label.font.family: fontfamily
                    label.color: "white"
                    backgroundComponent: Rectangle{
                        anchors.fill: parent
                        color: headerColor
                        radius: radiusNumber
                    }
                    onClicked: {
                        openimagedia.close();
                        isOK(Number(imageHeight.text), Number(imageWidth.text), imageDepth.displayText);
                    }
                }

                TButton{
                    width: 80;
                    height: 32;
                    label.text: "取消"
                    label.font.family: fontfamily
                    label.color: "white"
                    backgroundComponent: Rectangle{
                        anchors.fill: parent
                        border.color: headerColor
                        color: "#7B93B8"
                        border.width: 2
                        radius: radiusNumber
                    }

                    onClicked: {
                        openimagedia.close();
                    }
                }
            }
        }
    }
}
