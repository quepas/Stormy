-- Create default database
CREATE DATABASE stormy
	OWNER quepas
	ENCODING 'UTF8';

\connect stormy

-- Creating tables --
-- Tables for raw data
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
	code text,
	station integer,
	value_text text,
	value_number real,
	timestamp timestamp,
	PRIMARY KEY(id),
	FOREIGN KEY(code) REFERENCES metrics(code),
	FOREIGN KEY(station) REFERENCES station(id)
);

-- Create tables for aggregated data
CREATE TABLE aggregate_period (	
	name text,
	period interval,
	PRIMARY KEY(name),
	UNIQUE(name)
);

CREATE TABLE aggregate_task (
	id serial,
	period text,
	operation text,
	station integer,
	refresh integer,
	PRIMARY KEY(id),
	FOREIGN KEY(period) REFERENCES aggregate_period(name)
	FOREIGN KEY(operation) REFERENCES aggregate_operation(name)
	FOREIGN KEY(station) REFERENCES station(id)
);

CREATE TABLE aggregate_operation (	
	name text,
	formula text,
	analysisMethod text
	PRIMARY KEY(name),
	UNIQUE(name)
);

-- Fill with default data
-- Metrics
INSERT INTO metrics VALUES('unknown', 'none', 'none', 'none', 'none');