Acquisition, aggregation, analysis, visualization of weather data.

# Stormy Acquisition Server

Simple server for acquire meteorological data from many stations. Uses Python-based parser to getting data and NoSQL MongoDB database to temporary storage.

Requirements
------------

* Python ~3.3.3 (http://www.python.org/download/releases/3.3.3/)
* MongoDB ~2.4.8 (http://www.mongodb.org/downloads)

# Stormy Web Client

Web-based client for weather data visualization acquired by Stormy Acquisition or Storage&Aggregation Server.

Installation
------------

* Install node.js (http://nodejs.org/download/)
* Using npm (installed with node.js) install:
	+ 'bower' a package manager for web (just type in cmd 'npm install -g bower', '-g' parameter is for global installation)
	+ 'express' (in cmd go to 'client/web' directory and type 'npm install express' for local or use '-g' for global installation)
* Using bower (installed by npm) install:
	+ 'angular.js' (in 'client/web' directory type 'bower install angular')
	+ 'angular-route.js' (in 'client/web' directory type 'bower install angular-route', its external for 1.2.x angular.js)
	+ 'angular-bootstrap-datetimepicker' (in 'client/web' directory type 'bower install angular-bootstrap-datetimepicker')
* From 'bower_components' directories copy:
	+ To 'client/web/public/js':
		1. angular.min.js (from 'bower_components/angular')
		2. angular-route.min.js (from 'bower_components/angular-route')
		3. datetimepicker.js (from 'bower_components/angular-bootstrap-datetimepicker/src/js')
		4. jquery.min.js (from 'bower_components/jquery')
		5. moment.js (from 'bower_components/moment')
	+ To 'client/web/public/css':
		1. datetimepicker.css (from 'bower_components/angular-bootstrap-datetimepicker/src/css')
* Download 'bootstrap 2.3.2' (http://getbootstrap.com/2.3.2/)
	+ Copy 'bootstrap.min.js' to 'client/web/public/js'
	+ Copy 'bootstrap.min.css' to 'client/web/public/css'
	+ Copy 'glyphicons-halflings.png' to 'client/web/public/img'
	+ Copy 'glyphicons-halflings-white.png' to 'client/web/public/img'
* Download 'Chart.js' (http://www.chartjs.org/) and copy it to 'client/web/public/js'

Running
-------

Open Command Prompt (cmd). Go to 'client/web' directory and run node server from 'run.js' script (type: 'node run.js').

Using
-----

Go to your browser and open http://127.0.0.1:1337 site. Type Stormy Acquisition or Storage&Aggregation Server host address and port. Click 'Connect' and enjoy it!
