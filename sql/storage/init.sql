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
