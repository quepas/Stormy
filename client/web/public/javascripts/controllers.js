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
	$http.get('/acq/station').success(function(data) {
		$scope.stations = data.stations
		$scope.currentStation = $scope.stations[0]
	})	
	console.log("AppCtrl")
}

function AcquireStations() {

}