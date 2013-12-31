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

	console.log('[INFO] Loggin into app on server ' + acqHost + ':' + acqPort)
	return res.redirect('#' + req.url)
})

// Stormy REST Api
app.get('/station', function(req, res) {
	console.log('[INFO] Get stations UID list.')
	http.get(prepareConnectOptions('/station'),
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})
app.get('/station/:station_uid', function(req, res) {
	var station_uid = req.params.station_uid
	console.log('[INFO] Get station details: ' + station_uid )
	http.get(prepareConnectOptions('/station/' + station_uid),
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})
app.get('/metrics', function(req, res) {
	console.log('[INFO] Get metrics codes list.')
	http.get(prepareConnectOptions('/metrics'),
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})
app.get('/metrics/:metrics_code', function(req, res) {
	var metrics_code = req.params.metrics_code
	console.log('[INFO] Get metrics details: ' + metrics_code )
	http.get(prepareConnectOptions('/metrics/' + metrics_code),
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})
app.get('/meteo/:station_uid', function(req, res) {
	var station_uid = req.params.station_uid
	var from_ts = req.query.from;
	var to_ts = req.query.to;
	console.log('[INFO] Fetched station meteo: ' + station_uid + '. From: ' + from_ts + ', to: ' + to_ts)
	http.get(prepareConnectOptions('/meteo/' + station_uid + '?from=' + from_ts + '&to=' + to_ts),
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})
app.get('/export/:station_uid', function(req, res) {
	var station_uid = req.params.station_uid
	var from_ts = req.query.from;
	var to_ts = req.query.to;
	var metrics = req.query.metrics;
	console.log('[INFO] Export meteo to csv for: ' + station_uid + '. From: ' + from_ts + ', to: ' + to_ts + ', metrics: ' + metrics)
	http.get(prepareConnectOptions('/export/' + station_uid + '?from=' + from_ts + '&to=' + to_ts + '&metrics=' + metrics),
		function(rawData) {
			collectAndSendRawData(res, rawData)
		}
	)
})
app.get('/info', function(req, res) {
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