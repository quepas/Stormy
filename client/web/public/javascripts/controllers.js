var mainModule = angular.module('StormyWebApp', [])

mainModule.config(function($routeProvider) {
	$routeProvider
		.when('/', {controller: 'ConnectCtrl', templateUrl: 'templates/connect.html'})
		.when('/app', {controller: 'AppCtrl', templateUrl: 'templates/visualization.html'})
		.otherwise({redirectTo: '/'})
})

function ConnectCtrl() {
	console.log("ConnectCtrl")
}

function AppCtrl($scope, $http) {
	$scope.updateMeteo = function() {
		$http.get('/acq/meteo/' + $scope.currentStation.id + '/' + $scope.currentType.id)
			.success(function(data) {
				$scope.value = data.data;
			})	
	}
	//$scope.updateMeteo()

	$http.get('/acq/station').success(function(data) {
		$scope.stations = data.stations
		$scope.currentStation = $scope.stations[0]
	})	

	$http.get('/acq/info').success(function(data) {
		$scope.availableTypes = data.availableTypes
		$scope.currentType = $scope.availableTypes[0]
	})
}

