var mainModule = angular.module('StormyWebApp', ['ngRoute', 'ui.bootstrap.datetimepicker'])

mainModule.config(function($routeProvider) {
  $routeProvider
    .when('/', {controller: 'ConnectCtrl', templateUrl: 'templates/connect.html'})
    .when('/app', {controller: 'AppCtrl', templateUrl: 'templates/visualization.html'})
    .when('/export', {controller: 'ExportCtrl', templateUrl: 'templates/export.html'})
    .otherwise({redirectTo: '/'})
})

function ConnectCtrl() {
}

function AppCtrl($scope, $http) {
  CheckServerType($http, $scope)
  InsertVisualizationModes($scope)
  FetchInitialData($http, $scope)

  $scope.UpdateMeteo = function() {
    var from = 0;
    var to = moment().unix() + 3600;
    if ($scope.from !== undefined) {
      from = moment($scope.from.date).unix() + 3600;
    }
    if ($scope.to !== undefined) {
      to = moment($scope.to.date).unix() + 3600;
    }

    if ($scope.mode.name == 'Meteo') {
      $http.get('/meteo/' + $scope.station.uid + '?from=' + from + '&to=' + to).success(function(data) {
        $scope.measurements = data.measurements
        var context = document.getElementById("meteoChart").getContext("2d")
        var meteoChart = new Chart(context).Line(PrepareData($scope.measurements, $scope.metrics.code))
      })
    } else if ($scope.mode.name == 'Aggregate') {
      $http.get('/aggregate/' + $scope.station.uid + '/' + $scope.period + '?from=' + from + '&to=' + to).success(function(data) {
        $scope.aggregates = data.aggregates
        var context = document.getElementById("meteoChart").getContext("2d")
        var meteoChart = new Chart(context).Line(PrepareAggregate($scope.aggregates, $scope.metrics.code, $scope.operation))
      })
    }
  }
  $scope.UpdateChart = function() {
    var context = document.getElementById("meteoChart").getContext("2d")
    if ($scope.mode.name == 'Meteo') {
      var meteoChart = new Chart(context).Line(PrepareData($scope.measurements, $scope.metrics.code))
    } else {
      var meteoChart = new Chart(context).Line(PrepareAggregate($scope.aggregates, $scope.metrics.code, $scope.operation))
    }
  }
  $scope.UpdateMode = function() {
    $scope.hide_operation = !$scope.hide_operation;
    $scope.UpdateMeteo()
  }
}

function ExportCtrl($scope, $http) {
  $scope.checked_metrics = []
  FetchInitialData($http, $scope)

  $scope.Export = function() {
    var checked = $scope.checked_metrics
    var from = 0
    var to = moment().unix() + 3600
    var metrics = ','
    if ($scope.from !== undefined) {
      from = moment($scope.from.date).unix() + 3600
    }
    if ($scope.to !== undefined) {
      to = moment($scope.to.date).unix() + 3600
    }

    Object.keys(checked).forEach(function (key) {
      if (metrics == ',')
        metrics = ''
        if (checked[key] != '')
          metrics += checked[key] + ','

    });

    $http.get('/export/' + $scope.station.uid + '?from=' + from + '&to=' + to + '&metrics=' + metrics).success(function(data) {
      SaveAsCSV('meteo_' + $scope.station.uid + '_' + from + '_' + to, data)
    })
  }
}

///////////////////////////////// help functions /////////////////////////////////
function PrepareData(measurements, metrics_code) {
  var meteoLabels = []
  var meteoValues = []
  var startIndex = 0;
  var fixedLength = 30
  var length = measurements.length
  for(var i = 0; i < measurements.length; i++) {
    meteoLabels[i] = ''//formatTimestamp(measurements[startIndex + i].timestamp)
    meteoValues[i] = measurements[i].data[metrics_code]

    if(i == 0) {
      meteoValues[i] -= 0.0001 // HACK for: https://github.com/nnnick/Chart.js/issues/242
    }
  }

  var data = {
  labels : meteoLabels,
  datasets : [
    {
      fillColor : "rgba(107,149,145,0.5)",
      strokeColor : "rgba(36,61,60,1)",
      pointColor : "rgba(36,61,60,1)",
      pointStrokeColor : "#fff",
      data : meteoValues
    }
  ]}
  return data
}

function PrepareAggregate(aggregates, metrics_code, operation_code) {
  var meteoLabels = []
  var aggregateValues = []
  var startIndex = 0;
  var fixedLength = 30
  var length = aggregates.length
  for(var i = 0; i < aggregates.length; i++) {
    meteoLabels[i] = ''//formatTimestamp(aggregates[startIndex + i].timestamp)
    aggregateValues[i] = aggregates[i].data[metrics_code][operation_code]

    if(i == 0) {
      aggregateValues[i] -= 0.0001 // HACK for: https://github.com/nnnick/Chart.js/issues/242
    }
  }

  var data = {
  labels : meteoLabels,
  datasets : [
    {
      fillColor : "rgba(107,149,145,0.5)",
      strokeColor : "rgba(36,61,60,1)",
      pointColor : "rgba(36,61,60,1)",
      pointStrokeColor : "#fff",
      data : aggregateValues
    }
  ]}
  return data
}

function formatTimestamp(timestamp) {
  var currentDate = new Date(timestamp * 1000)
  return currentDate.getUTCHours() + ':' + currentDate.getUTCMinutes() + ' '
}

function SaveAsCSV(filename, text) {
  var pom = document.createElement('a');
  pom.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
  pom.setAttribute('download', filename + ".csv");
    pom.click();
}

function FetchInitialData(http, scope) {
  // Fetch station data
  http.get('/station').success(function(data) {
    var station_uids = data.stations
    var stations_info = []
    for (var i = 0; i < station_uids.length; ++i) {
      http.get('/station/' + station_uids[i]).success(function(data) {
        stations_info.push(data.station)
      })
    }
    scope.all_stations = stations_info
    scope.station = scope.all_stations[0]
  })
  // Fetch metrics data
  http.get('/metrics').success(function(data) {
    var metrics_codes = data.metrics
    var metrics_info = []
    for (var i = 0; i < metrics_codes.length; ++i) {
      http.get('/metrics/' + metrics_codes[i]).success(function(data) {
        metrics_info.push(data.metrics)
      })
    }
    scope.all_metrics = metrics_info
    scope.metrics = scope.all_metrics[0]
  })
  if (!scope.hide_export) {
    // Fetch periods data
    http.get('/period').success(function(data) {
      var period_names = data.periods
      scope.all_periods = period_names
      scope.period = period_names[0]
    })
    // Fetch operations data
    http.get('/operation').success(function(data) {
      var operation_names = data.operations
      scope.all_operations = operation_names
      scope.operation = operation_names[0]
    })
  }
}

function CheckServerType(http, scope) {
  http.get('/info').success(function(data) {
    var info = data.server
    if(info.type == "S&A") {
      scope.hide_mode = false
    }
    else {
      scope.hide_mode = true
    }
    scope.hide_export = true
    scope.hide_operation = true
  })
}

function InsertVisualizationModes(scope) {
  scope.all_modes = [{name:'Meteo'},{name:'Aggregate'}]
  scope.mode = scope.all_modes[0]
}