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
	PRIMARY KEY(uid),
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
	station_uid text,
	value_text text,
	value_number numeric,
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
	analysis_method text,
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

CREATE TABLE aggregate (
	id serial,
	station_uid text,
	metrics_code text,
	operation_name text,
	period_name text,
	start_time timestamp,
	value numeric,
	sample_number integer,
	PRIMARY KEY(id),
	FOREIGN KEY(station_uid) REFERENCES station(uid),
	FOREIGN KEY(metrics_code) REFERENCES metrics(code),
	FOREIGN KEY(operation_name) REFERENCES aggregate_operation(name),
	FOREIGN KEY(period_name) REFERENCES aggregate_period(name)
);

-- Fill with default data
-- Operation
INSERT INTO aggregate_operation VALUES('mean', 'mean_normal');
-- Periods
INSERT INTO aggregate_period VALUES('hourly', '1 hour'::interval);
INSERT INTO aggregate_period VALUES('daily', '1 day'::interval);
INSERT INTO aggregate_period VALUES('monthly', '1 month'::interval);
