-- Create default database
CREATE DATABASE stormy
	OWNER quepas
	ENCODING 'UTF8';

\connect stormy

-- Creating tables
CREATE TABLE station (
	id serial,
	uid text,
	name text,
	url text,
	refresh integer,
	PRIMARY KEY(id),
	UNIQUE(uid)
);

CREATE TABLE metrics (
	code text,
	equivalents text,
	type text,
	unit text,
	format text,
	PRIMARY KEY(code),
	UNIQUE(code)
);

CREATE TABLE measurement (
	id serial,
	id_metrics text,
	id_station integer,
	value_text text,
	value_number real,
	timestamp timestamp,
	PRIMARY KEY(id),
	FOREIGN KEY(id_metrics) REFERENCES metrics(code),
	FOREIGN KEY(id_station) REFERENCES station(id)
)