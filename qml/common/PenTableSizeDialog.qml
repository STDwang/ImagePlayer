import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import Toou2D 1.0

TDialog{
    id:openimagedia
    property real rowCount :    1;
    property real columnCount : 1;

    signal isOk();

    bodyComponent:  Item {
        width: 400;
        height: 250;
        function sureTable(mouseX, mouseY){
            var itemWidth = (tableClumn.width - 10) / 3
            var itemHeight = (tableClumn.height - 10) / 3
            if(mouseX > 0 && mouseX < itemWidth){
                columnCount = 1;
            }else if(mouseX >= itemWidth && mouseX < itemWidth * 2 + tableClumn.spacing){
                columnCount = 2;
            }else if(mouseX >= itemWidth * 2 + tableClumn.spacing && mouseX < itemWidth * 3 + tableClumn.spacing * 2){
                columnCount = 3;
            }else {
                columnCount = 1;
            }
            if(mouseY >= 0 && mouseY < itemHeight){
                rowCount = 1;
            }else if(mouseY >= itemHeight && mouseY < itemHeight * 2 + tableClumn.spacing){
                rowCount = 2;
            }else if(mouseY >= itemHeight * 2 + tableClumn.spacing && mouseY < itemHeight * 3 + tableClumn.spacing * 2){
                rowCount = 3;
            }else {
                rowCount = 1;
            }
        }
        Column {
            id: tableClumn
            anchors.fill: parent
            spacing: 5
            Repeater{
                model: 3
                delegate: Row {
                    property real rIndex: index
                    spacing: 5
                    Repeater {
                        model: 3
                        delegate: Rectangle{
                            color: columnCount > index && rowCount > rIndex ? "#409EFF":"#1E1E1E"
                            width: (tableClumn.width - 10) / 3
                            height: (tableClumn.height - 10) / 3
                            radius: 5
                        }
                    }
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true;
            onExited: {
                rowCount = 1;
                columnCount = 1;
            }
            onPositionChanged: {
                sureTable(mouseX, mouseY);
            }
            onClicked: {
                sureTable(mouseX, mouseY);
                isOk();
            }
        }
    }
}
