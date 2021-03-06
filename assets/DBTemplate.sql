DROP TABLE IF EXISTS "Channel";
CREATE TABLE Channel (IDChannel INTEGER PRIMARY KEY ASC ON CONFLICT ABORT UNIQUE ON CONFLICT ABORT NOT NULL ON CONFLICT ABORT, Name STRING NOT NULL ON CONFLICT ABORT, Size INTEGER NOT NULL DEFAULT (1), Formula STRING NOT NULL DEFAULT x, Description TEXT, DataType REFERENCES DataType (IDDataType) ON DELETE CASCADE ON UPDATE CASCADE, CHECK (Size BETWEEN 1 AND 8));
DROP TABLE IF EXISTS "City";
CREATE TABLE City (IDCity INTEGER PRIMARY KEY AUTOINCREMENT, Name STRING, Lat REAL, Lon REAL, Country INTEGER REFERENCES Country (IDCountry));
DROP TABLE IF EXISTS "Coord";
CREATE TABLE Coord (IDCoord INTEGER PRIMARY KEY AUTOINCREMENT, Lat REAL NOT NULL, Lon REAL NOT NULL, Role INTEGER DEFAULT (0), Radius REAL DEFAULT (0), Time REAL DEFAULT (0), Space REAL DEFAULT (0), Use BOOLEAN DEFAULT (1), Track INTEGER REFERENCES Track (IDTrack));
DROP TABLE IF EXISTS "Country";
CREATE TABLE Country (IDCountry INTEGER PRIMARY KEY AUTOINCREMENT, Name STRING);
INSERT INTO "Country" VALUES(1,'Afghanistan');
INSERT INTO "Country" VALUES(2,'Albania');
INSERT INTO "Country" VALUES(3,'Algeria');
INSERT INTO "Country" VALUES(4,'American Samoa');
INSERT INTO "Country" VALUES(5,'Andorra');
INSERT INTO "Country" VALUES(6,'Angola');
INSERT INTO "Country" VALUES(7,'Anguilla');
INSERT INTO "Country" VALUES(8,'Antigua & Barbuda');
INSERT INTO "Country" VALUES(9,'Argentina');
INSERT INTO "Country" VALUES(10,'Armenia');
INSERT INTO "Country" VALUES(11,'Aruba');
INSERT INTO "Country" VALUES(12,'Australia');
INSERT INTO "Country" VALUES(13,'Austria');
INSERT INTO "Country" VALUES(14,'Azerbaijan');
INSERT INTO "Country" VALUES(15,'Bahamas, The');
INSERT INTO "Country" VALUES(16,'Bahrain');
INSERT INTO "Country" VALUES(17,'Bangladesh');
INSERT INTO "Country" VALUES(18,'Barbados');
INSERT INTO "Country" VALUES(19,'Belarus');
INSERT INTO "Country" VALUES(20,'Belgium');
INSERT INTO "Country" VALUES(21,'Belize');
INSERT INTO "Country" VALUES(22,'Benin');
INSERT INTO "Country" VALUES(23,'Bermuda');
INSERT INTO "Country" VALUES(24,'Bhutan');
INSERT INTO "Country" VALUES(25,'Bolivia');
INSERT INTO "Country" VALUES(26,'Bosnia & Herzegovina');
INSERT INTO "Country" VALUES(27,'Botswana');
INSERT INTO "Country" VALUES(28,'Brazil');
INSERT INTO "Country" VALUES(29,'British Virgin Is.');
INSERT INTO "Country" VALUES(30,'Brunei');
INSERT INTO "Country" VALUES(31,'Bulgaria');
INSERT INTO "Country" VALUES(32,'Burkina Faso');
INSERT INTO "Country" VALUES(33,'Burma');
INSERT INTO "Country" VALUES(34,'Burundi');
INSERT INTO "Country" VALUES(35,'Cambodia');
INSERT INTO "Country" VALUES(36,'Cameroon');
INSERT INTO "Country" VALUES(37,'Canada');
INSERT INTO "Country" VALUES(38,'Cape Verde');
INSERT INTO "Country" VALUES(39,'Cayman Islands');
INSERT INTO "Country" VALUES(40,'Central African Rep.');
INSERT INTO "Country" VALUES(41,'Chad');
INSERT INTO "Country" VALUES(42,'Chile');
INSERT INTO "Country" VALUES(43,'China');
INSERT INTO "Country" VALUES(44,'Colombia');
INSERT INTO "Country" VALUES(45,'Comoros');
INSERT INTO "Country" VALUES(46,'Congo, Dem. Rep.');
INSERT INTO "Country" VALUES(47,'Congo, Repub. of the');
INSERT INTO "Country" VALUES(48,'Cook Islands');
INSERT INTO "Country" VALUES(49,'Costa Rica');
INSERT INTO "Country" VALUES(50,'Cote d''Ivoire');
INSERT INTO "Country" VALUES(51,'Croatia');
INSERT INTO "Country" VALUES(52,'Cuba');
INSERT INTO "Country" VALUES(53,'Cyprus');
INSERT INTO "Country" VALUES(54,'Czech Republic');
INSERT INTO "Country" VALUES(55,'Denmark');
INSERT INTO "Country" VALUES(56,'Djibouti');
INSERT INTO "Country" VALUES(57,'Dominica');
INSERT INTO "Country" VALUES(58,'Dominican Republic');
INSERT INTO "Country" VALUES(59,'East Timor');
INSERT INTO "Country" VALUES(60,'Ecuador');
INSERT INTO "Country" VALUES(61,'Egypt');
INSERT INTO "Country" VALUES(62,'El Salvador');
INSERT INTO "Country" VALUES(63,'Equatorial Guinea');
INSERT INTO "Country" VALUES(64,'Eritrea');
INSERT INTO "Country" VALUES(65,'Estonia');
INSERT INTO "Country" VALUES(66,'Ethiopia');
INSERT INTO "Country" VALUES(67,'Faroe Islands');
INSERT INTO "Country" VALUES(68,'Fiji');
INSERT INTO "Country" VALUES(69,'Finland');
INSERT INTO "Country" VALUES(70,'France');
INSERT INTO "Country" VALUES(71,'French Guiana');
INSERT INTO "Country" VALUES(72,'French Polynesia');
INSERT INTO "Country" VALUES(73,'Gabon');
INSERT INTO "Country" VALUES(74,'Gambia, The');
INSERT INTO "Country" VALUES(75,'Gaza Strip');
INSERT INTO "Country" VALUES(76,'Georgia');
INSERT INTO "Country" VALUES(77,'Germany');
INSERT INTO "Country" VALUES(78,'Ghana');
INSERT INTO "Country" VALUES(79,'Gibraltar');
INSERT INTO "Country" VALUES(80,'Greece');
INSERT INTO "Country" VALUES(81,'Greenland');
INSERT INTO "Country" VALUES(82,'Grenada');
INSERT INTO "Country" VALUES(83,'Guadeloupe');
INSERT INTO "Country" VALUES(84,'Guam');
INSERT INTO "Country" VALUES(85,'Guatemala');
INSERT INTO "Country" VALUES(86,'Guernsey');
INSERT INTO "Country" VALUES(87,'Guinea');
INSERT INTO "Country" VALUES(88,'Guinea-Bissau');
INSERT INTO "Country" VALUES(89,'Guyana');
INSERT INTO "Country" VALUES(90,'Haiti');
INSERT INTO "Country" VALUES(91,'Honduras');
INSERT INTO "Country" VALUES(92,'Hong Kong');
INSERT INTO "Country" VALUES(93,'Hungary');
INSERT INTO "Country" VALUES(94,'Iceland');
INSERT INTO "Country" VALUES(95,'India');
INSERT INTO "Country" VALUES(96,'Indonesia');
INSERT INTO "Country" VALUES(97,'Iran');
INSERT INTO "Country" VALUES(98,'Iraq');
INSERT INTO "Country" VALUES(99,'Ireland');
INSERT INTO "Country" VALUES(100,'Isle of Man');
INSERT INTO "Country" VALUES(101,'Israel');
INSERT INTO "Country" VALUES(102,'Italy');
INSERT INTO "Country" VALUES(103,'Jamaica');
INSERT INTO "Country" VALUES(104,'Japan');
INSERT INTO "Country" VALUES(105,'Jersey');
INSERT INTO "Country" VALUES(106,'Jordan');
INSERT INTO "Country" VALUES(107,'Kazakhstan');
INSERT INTO "Country" VALUES(108,'Kenya');
INSERT INTO "Country" VALUES(109,'Kiribati');
INSERT INTO "Country" VALUES(110,'Korea, North');
INSERT INTO "Country" VALUES(111,'Korea, South');
INSERT INTO "Country" VALUES(112,'Kuwait');
INSERT INTO "Country" VALUES(113,'Kyrgyzstan');
INSERT INTO "Country" VALUES(114,'Laos');
INSERT INTO "Country" VALUES(115,'Latvia');
INSERT INTO "Country" VALUES(116,'Lebanon');
INSERT INTO "Country" VALUES(117,'Lesotho');
INSERT INTO "Country" VALUES(118,'Liberia');
INSERT INTO "Country" VALUES(119,'Libya');
INSERT INTO "Country" VALUES(120,'Liechtenstein');
INSERT INTO "Country" VALUES(121,'Lithuania');
INSERT INTO "Country" VALUES(122,'Luxembourg');
INSERT INTO "Country" VALUES(123,'Macau');
INSERT INTO "Country" VALUES(124,'Macedonia');
INSERT INTO "Country" VALUES(125,'Madagascar');
INSERT INTO "Country" VALUES(126,'Malawi');
INSERT INTO "Country" VALUES(127,'Malaysia');
INSERT INTO "Country" VALUES(128,'Maldives');
INSERT INTO "Country" VALUES(129,'Mali');
INSERT INTO "Country" VALUES(130,'Malta');
INSERT INTO "Country" VALUES(131,'Marshall Islands');
INSERT INTO "Country" VALUES(132,'Martinique');
INSERT INTO "Country" VALUES(133,'Mauritania');
INSERT INTO "Country" VALUES(134,'Mauritius');
INSERT INTO "Country" VALUES(135,'Mayotte');
INSERT INTO "Country" VALUES(136,'Mexico');
INSERT INTO "Country" VALUES(137,'Micronesia, Fed. St.');
INSERT INTO "Country" VALUES(138,'Moldova');
INSERT INTO "Country" VALUES(139,'Monaco');
INSERT INTO "Country" VALUES(140,'Mongolia');
INSERT INTO "Country" VALUES(141,'Montserrat');
INSERT INTO "Country" VALUES(142,'Morocco');
INSERT INTO "Country" VALUES(143,'Mozambique');
INSERT INTO "Country" VALUES(144,'Namibia');
INSERT INTO "Country" VALUES(145,'Nauru');
INSERT INTO "Country" VALUES(146,'Nepal');
INSERT INTO "Country" VALUES(147,'Netherlands');
INSERT INTO "Country" VALUES(148,'Netherlands Antilles');
INSERT INTO "Country" VALUES(149,'New Caledonia');
INSERT INTO "Country" VALUES(150,'New Zealand');
INSERT INTO "Country" VALUES(151,'Nicaragua');
INSERT INTO "Country" VALUES(152,'Niger');
INSERT INTO "Country" VALUES(153,'Nigeria');
INSERT INTO "Country" VALUES(154,'N. Mariana Islands');
INSERT INTO "Country" VALUES(155,'Norway');
INSERT INTO "Country" VALUES(156,'Oman');
INSERT INTO "Country" VALUES(157,'Pakistan');
INSERT INTO "Country" VALUES(158,'Palau');
INSERT INTO "Country" VALUES(159,'Panama');
INSERT INTO "Country" VALUES(160,'Papua New Guinea');
INSERT INTO "Country" VALUES(161,'Paraguay');
INSERT INTO "Country" VALUES(162,'Peru');
INSERT INTO "Country" VALUES(163,'Philippines');
INSERT INTO "Country" VALUES(164,'Poland');
INSERT INTO "Country" VALUES(165,'Portugal');
INSERT INTO "Country" VALUES(166,'Puerto Rico');
INSERT INTO "Country" VALUES(167,'Qatar');
INSERT INTO "Country" VALUES(168,'Reunion');
INSERT INTO "Country" VALUES(169,'Romania');
INSERT INTO "Country" VALUES(170,'Russia');
INSERT INTO "Country" VALUES(171,'Rwanda');
INSERT INTO "Country" VALUES(172,'Saint Helena');
INSERT INTO "Country" VALUES(173,'Saint Kitts & Nevis');
INSERT INTO "Country" VALUES(174,'Saint Lucia');
INSERT INTO "Country" VALUES(175,'St Pierre & Miquelon');
INSERT INTO "Country" VALUES(176,'Saint Vincent and the Grenadines');
INSERT INTO "Country" VALUES(177,'Samoa');
INSERT INTO "Country" VALUES(178,'San Marino');
INSERT INTO "Country" VALUES(179,'Sao Tome & Principe');
INSERT INTO "Country" VALUES(180,'Saudi Arabia');
INSERT INTO "Country" VALUES(181,'Senegal');
INSERT INTO "Country" VALUES(182,'Serbia');
INSERT INTO "Country" VALUES(183,'Seychelles');
INSERT INTO "Country" VALUES(184,'Sierra Leone');
INSERT INTO "Country" VALUES(185,'Singapore');
INSERT INTO "Country" VALUES(186,'Slovakia');
INSERT INTO "Country" VALUES(187,'Slovenia');
INSERT INTO "Country" VALUES(188,'Solomon Islands');
INSERT INTO "Country" VALUES(189,'Somalia');
INSERT INTO "Country" VALUES(190,'South Africa');
INSERT INTO "Country" VALUES(191,'Spain');
INSERT INTO "Country" VALUES(192,'Sri Lanka');
INSERT INTO "Country" VALUES(193,'Sudan');
INSERT INTO "Country" VALUES(194,'Suriname');
INSERT INTO "Country" VALUES(195,'Swaziland');
INSERT INTO "Country" VALUES(196,'Sweden');
INSERT INTO "Country" VALUES(197,'Switzerland');
INSERT INTO "Country" VALUES(198,'Syria');
INSERT INTO "Country" VALUES(199,'Taiwan');
INSERT INTO "Country" VALUES(200,'Tajikistan');
INSERT INTO "Country" VALUES(201,'Tanzania');
INSERT INTO "Country" VALUES(202,'Thailand');
INSERT INTO "Country" VALUES(203,'Togo');
INSERT INTO "Country" VALUES(204,'Tonga');
INSERT INTO "Country" VALUES(205,'Trinidad & Tobago');
INSERT INTO "Country" VALUES(206,'Tunisia');
INSERT INTO "Country" VALUES(207,'Turkey');
INSERT INTO "Country" VALUES(208,'Turkmenistan');
INSERT INTO "Country" VALUES(209,'Turks & Caicos Is');
INSERT INTO "Country" VALUES(210,'Tuvalu');
INSERT INTO "Country" VALUES(211,'Uganda');
INSERT INTO "Country" VALUES(212,'Ukraine');
INSERT INTO "Country" VALUES(213,'United Arab Emirates');
INSERT INTO "Country" VALUES(214,'United Kingdom');
INSERT INTO "Country" VALUES(215,'United States');
INSERT INTO "Country" VALUES(216,'Uruguay');
INSERT INTO "Country" VALUES(217,'Uzbekistan');
INSERT INTO "Country" VALUES(218,'Vanuatu');
INSERT INTO "Country" VALUES(219,'Venezuela');
INSERT INTO "Country" VALUES(220,'Vietnam');
INSERT INTO "Country" VALUES(221,'Virgin Islands');
INSERT INTO "Country" VALUES(222,'Wallis and Futuna');
INSERT INTO "Country" VALUES(223,'West Bank');
INSERT INTO "Country" VALUES(224,'Western Sahara');
INSERT INTO "Country" VALUES(225,'Yemen');
INSERT INTO "Country" VALUES(226,'Zambia');
INSERT INTO "Country" VALUES(227,'Zimbabwe');
DROP TABLE IF EXISTS "Data";
CREATE TABLE Data ("Index" INTEGER, Race INTEGER REFERENCES Vehicle, Channel INTEGER REFERENCES Channel (IDChannel), Time TIME, Value STRING, PRIMARY KEY ("Index", Race, Channel));
DROP TABLE IF EXISTS "DataType";
CREATE TABLE DataType (IDDataType INTEGER PRIMARY KEY AUTOINCREMENT, Type STRING NOT NULL);
INSERT INTO "DataType" VALUES(1,'BitFlag');
INSERT INTO "DataType" VALUES(2,'Unsigned Number');
INSERT INTO "DataType" VALUES(3,'Signed Number');
INSERT INTO "DataType" VALUES(4,'String');
DROP TABLE IF EXISTS "Race";
CREATE TABLE Race (IDRace INTEGER PRIMARY KEY AUTOINCREMENT, Name STRING, Date DATE, StartTime TIME, EndTime TIME, Description TEXT, Vehicle INTEGER REFERENCES Vehicle (IDVehicle), Track INTEGER REFERENCES Track (IDTrack));
DROP TABLE IF EXISTS "SpecialChannel";
CREATE TABLE SpecialChannel (IDSpecialChannel INTEGER PRIMARY KEY AUTOINCREMENT, Function STRING NOT NULL, Channel INTEGER REFERENCES Channel (IDChannel) ON DELETE SET NULL DEFAULT NULL);
INSERT INTO "SpecialChannel" VALUES(1,'Space',NULL);
INSERT INTO "SpecialChannel" VALUES(2,'Time',NULL);
INSERT INTO "SpecialChannel" VALUES(3,'Latitude',NULL);
INSERT INTO "SpecialChannel" VALUES(4,'Longitude',NULL);
INSERT INTO "SpecialChannel" VALUES(5,'Lap',NULL);
INSERT INTO "SpecialChannel" VALUES(6,'Waypoint',NULL);
INSERT INTO "SpecialChannel" VALUES(7,'Push-to-talk request',NULL);
INSERT INTO "SpecialChannel" VALUES(8,'Push-to-talk status',NULL);
DROP TABLE IF EXISTS "Track";
CREATE TABLE Track (IDTrack INTEGER PRIMARY KEY AUTOINCREMENT, Name STRING, Description TEXT, Lat REAL, Lon REAL, City INTEGER REFERENCES City (IDCity));
DROP TABLE IF EXISTS "Vehicle";
CREATE TABLE Vehicle (IDVehicle INTEGER PRIMARY KEY AUTOINCREMENT, Name STRING NOT NULL, Photo STRING, Description TEXT);
