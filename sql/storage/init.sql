-- Create default database
CREATE DATABASE stormy
	OWNER quepas
	ENCODING 'UTF8';

\connect stormy

-- Creating tables --
-- Tables for raw data
CREATE TABLE station (
	uid text,
	name text,
	url text,
	refresh_time integer,
	last_update timestamp,
	PRIMARY KEY(uid)
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
	station_uid text,
	value_text text,
	value_number real,
	timestamp timestamp,
	PRIMARY KEY(id),
	FOREIGN KEY(code) REFERENCES metrics(code),
	FOREIGN KEY(station_uid) REFERENCES station(uid)
);

-- Create tables for aggregated data
CREATE TABLE aggregate_period (
	name text,
	period interval,
	PRIMARY KEY(name),
	UNIQUE(name)
);

CREATE TABLE aggregate_operation (
	name text,
	formula text,
	analysisMethod text,
	PRIMARY KEY(name),
	UNIQUE(name)
);

CREATE TABLE aggregate_task (
	id serial,
	period_name text,
	station_uid text,
	current_ts timestamp,
	PRIMARY KEY(id),
	FOREIGN KEY(period_name) REFERENCES aggregate_period(name),
	FOREIGN KEY(station_uid) REFERENCES station(uid)
);

-- Fill with default data
-- Metrics
INSERT INTO metrics VALUES('unknown', 'none', 'none', 'none', 'none');
-- Operation
INSERT INTO aggregate_operation VALUES('mean', 'mean(x)', 'mean()');
-- Period
INSERT INTO aggregate_period VALUES('hourly', '1 hour'::interval);
--INSERT INTO aggregate_period VALUES('daily', '1 day'::interval);
--INSERT INTO aggregate_period VALUES('monthly', '1 month'::interval);


-- test data --
-- station
INSERT INTO station VALUES('uid#1', 'Station#1', 'http://station1.pl', 1, to_timestamp(0));
--