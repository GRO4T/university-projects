import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Extras 1.4
import QtCharts 2.3
import Sensors 1.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.4


GridView {
    id: sensorGridView
    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
        bottom: parent.bottom
        topMargin: 10
        leftMargin: 10
        bottomMargin: 10
    }
    cellHeight: height / 3 - 10
    cellWidth: width / 2
    model: sensorModel
    delegate: GroupBox {
        x: 10
        y: 10
        label: Text {
            text: device
        }
        width: sensorGridView.cellWidth - 10
        height: sensorGridView.cellHeight - 10
        Row {
            ListView {
                id: sensorListView
                width: 150
                height: sensorGridView.cellHeight - 60
                model: sensorList 
                delegate: Text {
                    id: sensorText
                    property var myValue: modelData.value 
                    text: modelData.name + "\t" + myValue + modelData.unit
                    Connections {
                        target: modelData 
                        onValueChanged: {
                            sensorText.myValue =  modelData.value;
                        }
                    }
                }
            }
            Column {
                width: sensorGridView.cellWidth - 10 - sensorListView.width
                height: sensorGridView.cellHeight - 60
                Row {
                    anchors.fill: parent
                    Repeater {
                        model: meterSensorList 
                        Column {
                            anchors.fill: parent
                            CircularGauge {
                                id: sensorGauge
                                property var myValue: modelData.value
                                width: parent.width - 20
                                height: parent.height - 20
                                maximumValue: modelData.maxValue
                                minimumValue: modelData.minValue
                                value: myValue
                                anchors.centerIn: parent
                                style : CircularGaugeStyle {
                                    tickmarkStepSize: modelData.stepSize
                                }
                                Connections {
                                    target: modelData 
                                    onValueChanged: {
                                        sensorGauge.myValue = modelData.value;
                                    }
                                }
                            }
                            Text {
                                text: modelData.name + "[" + modelData.unit + "]"
                                anchors.centerIn: parent
                                anchors.verticalCenterOffset: 20
                            }
                        }
                    }

                }
            }
        }
    }
}
