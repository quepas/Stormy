var express = require('express')
var http = require('http')
var app = express()

var host = '127.0.0.1'
var port = 1337;

var acqHost = ''
var acqPort = ''

app.use(express.bodyParser());
app.use(express.static(__dirname + '/public'))

app.post('/app', function(req, res) {
	acqHost = req.body.address
	acqPort = req.body.port

	console.log('[INFO]: Loggin into app on server ' + acqHost + ':' + acqPort)
	return res.redirect('#' + req.url)
})

// AcqREST mappings
app.get('/acq/station', function(req, res) {
	console.log('[INFO]: Get stations data')
	
	http.get(prepareConnectOptions('/station'), 
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})

app.get('/acq/meteo/:stationId', function(req, res) {
	console.log("[INFO]: Get meteo for stationId")
	
	http.get(prepareConnectOptions('/meteo/' + req.params.stationId), 
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})

app.get('/acq/meteo/:stationId/:typeId', function(req, res) {
	console.log("[INFO]: Get current type meteo for stationId")

	http.get(prepareConnectOptions('/meteo/' + req.params.stationId + '/' + req.params.typeId),				
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})

app.get('/acq/info', function(req, res) {
	console.log("[INFO]: Get info from server")

	http.get(prepareConnectOptions('/info'), 
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})

process.on('uncaughtException', function (err) {
  console.error((new Date).toUTCString() + ' UncaughtException: ', err.message)
  console.error(err.stack) 
})

app.listen(port, host)
console.log('Stormy Web Client is running at http://' + host + ':' + port)

///////////////////////////////// help functions /////////////////////////////////
function prepareConnectOptions(path) {
	var result = {
		host: acqHost,
		port: acqPort,
		path: path
	}
	return result
}

function collectAndSendRawData(res, rawData) {
	var data = '';
	rawData.on('data', function(chunk) {				
		data += chunk;
	})
	rawData.on('end', function() {
		res.send(data)
	})
}