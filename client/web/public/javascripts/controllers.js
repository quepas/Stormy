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
		$http.get('/acq/meteo/' + $scope.currentStation.id + '/' + $scope.currentType.id)
			.success(function(data) {
				$scope.currentMeteo = data.measurements
				var context = document.getElementById("meteoChart").getContext("2d");
				var meteoChart = new Chart(context).Line(prepareData($scope.currentMeteo));
			})
	}

	$http.get('/acq/station').success(function(data) {
		$scope.stations = data.stations
		$scope.currentStation = $scope.stations[0]
	})

	$http.get('/acq/info').success(function(data) {
		$scope.availableTypes = data.availableTypes
	})
}

///////////////////////////////// help functions /////////////////////////////////
function prepareData(meteoData) {
	var meteoLabels = []
	var meteoValues = []
	var fixedLength = 15
	var length = (meteoData.length > fixedLength) ? fixedLength : meteoData.length

	for(var i = 0; i < length; i++) {
		meteoLabels[i] = formatTimestamp(meteoData[i].timestamp)
		meteoValues[i] = meteoData[i].data
	}

	var data = {
	labels : meteoLabels,
	datasets : [
		{
			fillColor : "rgba(220,220,220,0.5)",
			strokeColor : "rgba(220,220,220,1)",
			pointColor : "rgba(220,220,220,1)",
			pointStrokeColor : "#fff",
			data : meteoValues
		}
	]}
	return data
}

function formatTimestamp(timestamp) {
	var currentDate = new Date(timestamp * 1000)
	return currentDate.getHours() + ':' + currentDate.getMinutes()
}