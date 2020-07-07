import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12
import ToDo 1.0

ColumnLayout{
    Frame {
        ListView{
            implicitWidth: 250
            implicitHeight: 250
            spacing:8
            clip: true

            model: ToDoModel{
                list:toDoList
            }

            delegate: RowLayout{
                width: parent.width

                CheckBox{
                    checked:model.done
                    onClicked:model.done = checked
                }

                TextField{
                    Layout.fillWidth: true
                    text: model.description
                    onEditingFinished: model.description
                }
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("Add new item")
            onClicked: toDoList.appendItem()
        }

        Button{
            text: qsTr("Remove completed")
            onClicked: toDoList.removeItem()
        }
    }
}
