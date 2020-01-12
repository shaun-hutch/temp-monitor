// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// Write your JavaScript code.

function setupChart(model) {
    var fixedData = [];
    var labels = [];

    for (var i = 0; i < model.length; i++) {
        labels.push(model[i].TempDate);
        fixedData.push({
            y: model[i].TempInside,
            t: model[i].TempDate
        });
    }

    var chart = new Chart($("#chartContainer"), {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: "Indoor Temperature",
                data: fixedData,
                backgroundColor: 'rgba(0, 119, 204, 0.3)'
            }]
        }
    });
}
