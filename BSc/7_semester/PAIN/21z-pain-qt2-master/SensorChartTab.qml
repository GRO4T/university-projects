import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Extras 1.4
import QtCharts 2.3
import Sensors 1.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.4


ListView {
    id: sensorChartView
    anchors.fill: parent
    model: sensorModel

    delegate : ChartView {
        id: chart
        title: device 
        antialiasing: true
        width: sensorChartView.width - 10
        height: myHeight

        property int minY: 0
        property int maxY: 100
        property int stepY: 10
        property var myIndex: index
        property int myHeight: 250;

        ValueAxis{ id:xAxis; min:    0; max: 60; tickInterval:  10 }
        ValueAxis{ id:yAxis; min: minY; max:  maxY; tickInterval: stepY }

        function drawChart() {
            let colors = ["cyan", "magenta", "red", "darkRed", "darkCyan", "darkMagenta", "green", "darkGreen", "yellow", "blue"];
            let currentMinY = Number.POSITIVE_INFINITY;
            let currentMaxY = Number.NEGATIVE_INFINITY; 
            let currentStepY = Number.NEGATIVE_INFINITY;
            var series = sensorModel.devices[chart.myIndex].graphSensors;
            if (series.length === 0) {
                chart.myHeight = 0;
                return;
            }
            chart.removeAllSeries();
            for (var i = 0; i < series.length; i++) {
                var samples = series[i].history;
                var lineTypeSeries = chart.createSeries(ChartView.SeriesTypeLine, series[i].name + "[" + series[i].unit + "]", xAxis, yAxis);
                lineTypeSeries.color = colors[i % colors.length];
                lineTypeSeries.width = 2;
                lineTypeSeries.borderWidth = 0;
                for (var j = 0; j < samples.length - 1; j++) {
                    lineTypeSeries.append(j, samples[j]);
                    lineTypeSeries.append(j+1, samples[j+1]);
                }
                if (series[i].stepSize > currentStepY)
                    currentStepY = series[i].stepSize;
                if (series[i].minValue < currentMinY)
                    currentMinY = series[i].minValue;
                if (series[i].maxValue > currentMaxY)
                    currentMaxY = series[i].maxValue;
            }
            chart.minY = currentMinY;
            chart.maxY = currentMaxY;
            chart.stepY = currentStepY;
        }

        Connections {
            target: sensorModel
            onDataChanged: {
                chart.drawChart();
            }
        }

        Component.onCompleted: {
            chart.drawChart();
        }
    }
}
