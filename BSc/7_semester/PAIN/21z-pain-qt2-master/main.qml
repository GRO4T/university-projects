import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Extras 1.4
import QtCharts 2.3
import Sensors 1.0
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.4


Window {
    minimumWidth: 600
    minimumHeight: 600
    width: 900
    height: 900
    visible: true

    SensorModel {
        id: sensorModel
    }

    Column {
        anchors.fill: parent
        TabBar {
            id: navBar
            width: parent.width
            TabButton {
                text: qsTr("Grid")
            }
            TabButton {
                text: qsTr("Graphs")
            }
        }

        StackLayout {
            width: parent.width
            height: parent.height
            currentIndex: navBar.currentIndex
            Item {
                SensorGridTab {
                    id: gridView
                }
            }
            Item {
                anchors.fill: parent
                SensorChartTab {
                    id: listView
                }
            }
        }
    }
}
