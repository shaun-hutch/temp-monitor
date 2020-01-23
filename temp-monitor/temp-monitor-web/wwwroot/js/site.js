// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// Write your JavaScript code.

function setupChart(model) {
    var insideData = [];
    var outsideData = [];
    var humidityData = [];
    var labels = [];

    for (var i = 0; i < model.length; i++) {
        labels.push(model[i].TempDate);
        insideData.push({
            y: model[i].TempInside,
            t: model[i].TempDate
        });

        outsideData.push({
            y: model[i].TempOutside,
            t: model[i].TempDate
        });

        humidityData.push({
            y: model[i].Humidity,
            t: model[i].TempDate
        });
    }

    var chart = new Chart($("#chartContainer"), {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: "Indoor Temperature",
                data: insideData,
                backgroundColor: 'rgba(0, 119, 204, 0.0)',
                borderColor: 'rgba(0, 119, 204, 0.5)'
            },
            {
                label: "Outdoor Temperature",
                data: outsideData,
                backgroundColor: 'rgba(56, 142, 60, 0.0)',
                borderColor: 'rgba(56, 142, 60, 0.5)'
            },
            {
                label: "Humidity",
                data: humidityData,
                backgroundColor: 'rgba(228, 30, 30, 0.0)',
                borderColor: 'rgba(228, 30, 30, 0.5)'
            }]
        }
    });
}
