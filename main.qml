import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Window 2.13
import QtQml.Models 2.13
import Element 1.0

ApplicationWindow {
    id: window

    property bool toggleOn1s: false
    property variant messageArray: []
    ListModel {
        id: messageModel
        ListElement {
            name: "name"
            message: "message"
        }
    }

    Component.onCompleted: {
       window.showFullScreen();
    }

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        Label {
            text: "QtQuickSample"
            anchors.centerIn: parent
        }

        ToolButton {
            id: toolButton
            text: "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: drawer.open()
        }

        Drawer {
            id: drawer
            width: window.width * 0.25
            height: window.height

            Column {
                anchors.fill: parent
                ItemDelegate {
                    text: "quit"
                    width: parent.width
                    onClicked: {
                        drawer.close()
                        Qt.callLater(Qt.quit)
                    }
                }
            }
        }
    }

    Item{
        Timer {
            interval: 500; running: true; repeat: true
            onTriggered: {
                var date = new Date()
                time.text = date.toLocaleTimeString()

                toggleOn1s = !toggleOn1s
            }
        }
    }

    // ============== toolsArea ==============
    Column {
        id: toolsArea;
        anchors{
            top: parent.top
            right: parent.right
            left: parent.left
            margins: 20
        }
        spacing: 10

        // Clock
        Rectangle {
            width: 300; height: 50
            radius: 10.0
            color: "orange"
            Text {
                id: time
                anchors.centerIn: parent
                font.pointSize: 32
            }
        }

        // Buttons
        Row {
            spacing: 20
            Button {
                height: 40
                padding: 5
                font.pointSize: 18
                text: "random add";
                onClicked: {
                    elementList.addElement(('000' + Math.floor(Math.random()*200) ).slice( -3 ))
                }
            }
            Button {
                height: 40
                padding: 5
                font.pointSize: 16
                text: "random remove";
                onClicked: {
                    elementList.randRemoveRow()
                }
            }
            Button {
                height: 40
                padding: 5
                font.pointSize: 18
                text: "all remove";
                onClicked: {
                    while(true){
                        if(elementList.rowCount()===0)
                            break;
                        elementList.randRemoveRow()
                    }
                }
            }
            Button {
                height: 40
                padding: 5
                font.pointSize: 18
                text: "quit";
                onClicked: {
                    Qt.callLater(Qt.quit)
                }
            }
        }
    }

    // ============== messageArea ==============
    Rectangle {
        id: messageArea
        width: parent.width/5
        anchors{
            top: toolsArea.bottom
            left: parent.left
            bottom: parent.bottom
            margins: 20
        }
        color: "lightgreen"

        ListView {
            id: messageList
            model: messageModel
            anchors.fill: parent
            clip: true
            verticalLayoutDirection: ListView .BottomToTop
            delegate: Item {
                width: parent.width
                height: 50
                Text {
                    id: nameField
                    text: name + ": " + message
                    font.pointSize: 20
                }
            }
            onCountChanged: {
                messageList.positionViewAtEnd()
            }
        }
    }

    // ============== listArea ==============
    Rectangle {
        id: listArea
        anchors{
            top: toolsArea.bottom
            right: parent.right
            left: messageArea.right
            bottom: parent.bottom
            margins: 20
        }
        color: "lightblue"

        GridView {
            id: grid
            anchors.fill: parent
            cellWidth: parent.width/4
            cellHeight: 100
            model:
            DelegateModel {
                model: elementList
                delegate: Item {
                    width: grid.cellWidth
                    height: grid.cellHeight
                    Rectangle {
                        id: elementRectangle
                        anchors {
                            fill: parent
                            margins: 5
                        }
                        color: "orange"
                        border.color: "black"
                        border.width: 2
                        Text {
                            text: edit.detail
                            font.pointSize: 20
                            anchors.centerIn: parent
                        }

                    }
                    Connections {
                        target: edit
                        onMessageChanged: {
                            messageModel.append({"message": edit.message, "name":edit.name})
                        }
                    }
                    GridView.onAdd: {
                        messageModel.append({"message": edit.message, "name":edit.name})
                    }
                }
            }
            displaced: Transition {
                SequentialAnimation {
                    PauseAnimation { duration: 500 }
                    NumberAnimation { properties: "x,y"; duration: 400 }
                }
            }
            add: Transition {
                ParallelAnimation {
                    NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
                    NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
                }
            }
            remove: Transition {
                ParallelAnimation {
                    NumberAnimation { property: "opacity"; from: 1.0; to: 0; duration: 400 }
                    NumberAnimation { property: "scale"; from: 1.0; to: 0; duration: 400 }
                }
            }
        }
    }

}
