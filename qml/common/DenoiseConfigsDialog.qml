import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import Toou2D 1.0

TDialog{
    id:denoiseConfigsDia

    signal isOK(var denoiseType, var denoiseRadius);
    property string rawName:               "";
    property string srcPath:               "";
    property real   denoisRadius:          1;

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
        height: 300;
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
                    onClicked: denoiseConfigsDia.hideAndClose();
                }
            }
            Label {
                text: qsTr("去噪设置:")
                clip: true
                color: headerColor
                font.pixelSize: 24
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
                    id: denoisTypeLabel
                    text: qsTr("去噪算法选择:");
                    font.pixelSize: labelPixSize
                    font.family: fontfamily
                    color: labelColor
                }
                PenComboBox {
                    id: denoisType
                    height: rowHeight
                    width: rowWidth * 0.6
                    fontSize: 12
                    textColor: labelColor
                    backgoundColor: inputBackgroundColor
                    backgoundRadius: radiusNumber
                    anchors.verticalCenter: denoisTypeLabel.verticalCenter
                    model: ["均值滤波", "中值滤波", "lee滤波", "frost滤波", "高斯滤波", "gamma-map滤波", "双边滤波"]
                }
            }

            Row {
                height: rowHeight
                width: rowWidth
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Label {
                    id: denoisRadiusLabel
                    text: qsTr("半径:");
                    width: denoisTypeLabel.width
                    font.pixelSize: labelPixSize
                    font.family: fontfamily
                    color: labelColor
                }
                TInputField{
                    id: denoisRadiusInput;
                    background.radius: 10;
                    background.color: inputBackgroundColor
                    width: rowWidth * 0.6
                    height: parent.height
                    text: "1"
                    label.color: labelColor;
                    cursorColor: labelColor;

                    placeholderPosition: TPosition.Left;
                    placeholderLabel.text: "请输入参数";
                    placeholderLabel.color: labelColor
                    border.width: 0
                    anchors.verticalCenter: denoisRadiusLabel.verticalCenter
                    theme.enabled: false;
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
                        isOK(denoisType.displayText, Number(denoisRadiusInput.text));
                        denoiseConfigsDia.close();
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
                        denoiseConfigsDia.close();
                    }
                }
            }
        }
    }
}
