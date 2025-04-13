DROP TABLE IF EXISTS PUBLIC.PROVIDERS;

CREATE TABLE PUBLIC.PROVIDERS (id int NOT NULL AUTO_INCREMENT PRIMARY KEY,first_name varchar(100) DEFAULT NULL,last_name varchar(100) DEFAULT NULL,phone varchar(20),street_address varchar(255) DEFAULT NULL,city varchar(100) DEFAULT NULL,state varchar(100) DEFAULT NULL,zip_code varchar(5) DEFAULT NULL,specialty varchar(255) DEFAULT NULL, public_listing boolean DEFAULT NULL,suspended boolean DEFAULT NULL, taking_new_patients boolean DEFAULT NULL);
