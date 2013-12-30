var mainModule = angular.module('StormyWebApp', [])

mainModule.config(function($routeProvider) {
	$routeProvider
		.when('/', {controller: 'ConnectCtrl', templateUrl: 'templates/connect.html'})
		.when('/app', {controller: 'AppCtrl', templateUrl: 'templates/visualization.html'})
		.otherwise({redirectTo: '/'})
})

function ConnectCtrl() {
}

function AppCtrl($scope, $http) {
	$scope.updateMeteo = function() {
		console.log($scope.metrics.code)
		$http.get('/acq/meteo/' + $scope.station.uid + '/' + $scope.metrics.code)
			.success(function(data) {
				$scope.currentMeteo = data.measurements
				var context = document.getElementById("meteoChart").getContext("2d")
				var meteoChart = new Chart(context).Line(prepareData($scope.currentMeteo))
			})
	}

	// Fetch station data
	$http.get('/station').success(function(data) {
		var station_uids = data.stations
		var stations_info = []
		for (var i = 0; i < station_uids.length; ++i) {
			$http.get('/station/' + station_uids[i]).success(function(data) {
				stations_info.push(data.station)
			})
		}
		$scope.all_stations = stations_info
		$scope.station = $scope.all_stations[0]
	})
	// Fetch metrics data
	$http.get('/metrics').success(function(data) {
		var metrics_codes = data.metrics
		var metrics_info = []
		for (var i = 0; i < metrics_codes.length; ++i) {
			$http.get('/metrics/' + metrics_codes[i]).success(function(data) {
				metrics_info.push(data.metrics)
			})
		}
		$scope.all_metrics = metrics_info
        $scope.metrics = $scope.all_metrics[0]
	})
}

///////////////////////////////// help functions /////////////////////////////////
function prepareData(meteoData) {
	var meteoLabels = []
	var meteoValues = []
	var startIndex = 0;
	var fixedLength = 30
	var length = meteoData.length

	if(length > fixedLength) {
		startIndex = length - fixedLength;
	}

	for(var i = 0; i < fixedLength; i++) {
		meteoLabels[i] = formatTimestamp(meteoData[startIndex + i].timestamp)
		meteoValues[i] = meteoData[startIndex + i].data
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