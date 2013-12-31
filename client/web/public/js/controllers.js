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
	FetchStationAndMetrics($http, $scope)

	$scope.UpdateMeteo = function() {
		var from = 0;
		var to = moment().unix() + 3600;
		if ($scope.from !== undefined) {
			from = moment($scope.from.date).unix() + 3600;
		}
		if ($scope.to !== undefined) {
			to = moment($scope.to.date).unix() + 3600;
		}


		$http.get('/meteo/' + $scope.station.uid + '?from=' + from + '&to=' + to).success(function(data) {
			$scope.measurements = data.measurements
			var context = document.getElementById("meteoChart").getContext("2d")
			var meteoChart = new Chart(context).Line(PrepareData($scope.measurements, $scope.metrics.code))
		})
	}
	$scope.UpdateChart = function() {
		var context = document.getElementById("meteoChart").getContext("2d")
		var meteoChart = new Chart(context).Line(PrepareData($scope.measurements, $scope.metrics.code))
	}
}

function ExportCtrl($scope, $http) {
	$scope.checked_metrics = []
	FetchStationAndMetrics($http, $scope)

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
			download('meteo_' + moment().unix(), data)
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
			meteoValues[i] -= 0.00001 // HACK for: https://github.com/nnnick/Chart.js/issues/242
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

function formatTimestamp(timestamp) {
	var currentDate = new Date(timestamp * 1000)
	return currentDate.getUTCHours() + ':' + currentDate.getUTCMinutes() + ' '
}

		function download(filename, text) {
    		var pom = document.createElement('a');
    		pom.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
    		pom.setAttribute('download', filename + ".csv");
    		pom.click();
		}

function FetchStationAndMetrics(http, scope) {
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
}

function CheckServerType(http, scope) {
	http.get('/info').success(function(data) {
		var info = data.server
		if(info.type == "S&A")
			scope.hide_export = false
		else
			scope.hide_export = true
	})
}