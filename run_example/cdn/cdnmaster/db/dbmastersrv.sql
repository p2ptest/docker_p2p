-- MySQL dump 10.11
--
-- Host: localhost    Database: dbmastersrvex
-- ------------------------------------------------------
-- Server version	5.0.95

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `local_back_db`
--

DROP TABLE IF EXISTS `local_back_db`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `local_back_db` (
  `seqnum` bigint(25) NOT NULL,
  `sqlquery` varchar(1024) NOT NULL,
  PRIMARY KEY  (`seqnum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_0`
--

DROP TABLE IF EXISTS `source_info_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_0` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_1`
--

DROP TABLE IF EXISTS `source_info_1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_1` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_10`
--

DROP TABLE IF EXISTS `source_info_10`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_10` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_100`
--

DROP TABLE IF EXISTS `source_info_100`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_100` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_11`
--

DROP TABLE IF EXISTS `source_info_11`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_11` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_12`
--

DROP TABLE IF EXISTS `source_info_12`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_12` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_13`
--

DROP TABLE IF EXISTS `source_info_13`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_13` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_14`
--

DROP TABLE IF EXISTS `source_info_14`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_14` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_15`
--

DROP TABLE IF EXISTS `source_info_15`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_15` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_16`
--

DROP TABLE IF EXISTS `source_info_16`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_16` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_17`
--

DROP TABLE IF EXISTS `source_info_17`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_17` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_18`
--

DROP TABLE IF EXISTS `source_info_18`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_18` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_19`
--

DROP TABLE IF EXISTS `source_info_19`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_19` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_2`
--

DROP TABLE IF EXISTS `source_info_2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_2` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_20`
--

DROP TABLE IF EXISTS `source_info_20`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_20` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_21`
--

DROP TABLE IF EXISTS `source_info_21`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_21` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_22`
--

DROP TABLE IF EXISTS `source_info_22`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_22` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_23`
--

DROP TABLE IF EXISTS `source_info_23`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_23` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_24`
--

DROP TABLE IF EXISTS `source_info_24`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_24` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_25`
--

DROP TABLE IF EXISTS `source_info_25`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_25` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_26`
--

DROP TABLE IF EXISTS `source_info_26`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_26` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_27`
--

DROP TABLE IF EXISTS `source_info_27`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_27` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_28`
--

DROP TABLE IF EXISTS `source_info_28`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_28` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_29`
--

DROP TABLE IF EXISTS `source_info_29`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_29` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_3`
--

DROP TABLE IF EXISTS `source_info_3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_3` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_30`
--

DROP TABLE IF EXISTS `source_info_30`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_30` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_31`
--

DROP TABLE IF EXISTS `source_info_31`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_31` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_32`
--

DROP TABLE IF EXISTS `source_info_32`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_32` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_33`
--

DROP TABLE IF EXISTS `source_info_33`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_33` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_34`
--

DROP TABLE IF EXISTS `source_info_34`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_34` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_35`
--

DROP TABLE IF EXISTS `source_info_35`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_35` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_36`
--

DROP TABLE IF EXISTS `source_info_36`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_36` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_37`
--

DROP TABLE IF EXISTS `source_info_37`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_37` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_38`
--

DROP TABLE IF EXISTS `source_info_38`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_38` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_39`
--

DROP TABLE IF EXISTS `source_info_39`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_39` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_4`
--

DROP TABLE IF EXISTS `source_info_4`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_4` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_40`
--

DROP TABLE IF EXISTS `source_info_40`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_40` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_41`
--

DROP TABLE IF EXISTS `source_info_41`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_41` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_42`
--

DROP TABLE IF EXISTS `source_info_42`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_42` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_43`
--

DROP TABLE IF EXISTS `source_info_43`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_43` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_44`
--

DROP TABLE IF EXISTS `source_info_44`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_44` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_45`
--

DROP TABLE IF EXISTS `source_info_45`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_45` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_46`
--

DROP TABLE IF EXISTS `source_info_46`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_46` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_47`
--

DROP TABLE IF EXISTS `source_info_47`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_47` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_48`
--

DROP TABLE IF EXISTS `source_info_48`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_48` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_49`
--

DROP TABLE IF EXISTS `source_info_49`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_49` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_5`
--

DROP TABLE IF EXISTS `source_info_5`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_5` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_50`
--

DROP TABLE IF EXISTS `source_info_50`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_50` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_51`
--

DROP TABLE IF EXISTS `source_info_51`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_51` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_52`
--

DROP TABLE IF EXISTS `source_info_52`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_52` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_53`
--

DROP TABLE IF EXISTS `source_info_53`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_53` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_54`
--

DROP TABLE IF EXISTS `source_info_54`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_54` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_55`
--

DROP TABLE IF EXISTS `source_info_55`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_55` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_56`
--

DROP TABLE IF EXISTS `source_info_56`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_56` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_57`
--

DROP TABLE IF EXISTS `source_info_57`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_57` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_58`
--

DROP TABLE IF EXISTS `source_info_58`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_58` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_59`
--

DROP TABLE IF EXISTS `source_info_59`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_59` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_6`
--

DROP TABLE IF EXISTS `source_info_6`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_6` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_60`
--

DROP TABLE IF EXISTS `source_info_60`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_60` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_61`
--

DROP TABLE IF EXISTS `source_info_61`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_61` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_62`
--

DROP TABLE IF EXISTS `source_info_62`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_62` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_63`
--

DROP TABLE IF EXISTS `source_info_63`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_63` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_64`
--

DROP TABLE IF EXISTS `source_info_64`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_64` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_65`
--

DROP TABLE IF EXISTS `source_info_65`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_65` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_66`
--

DROP TABLE IF EXISTS `source_info_66`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_66` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_67`
--

DROP TABLE IF EXISTS `source_info_67`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_67` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_68`
--

DROP TABLE IF EXISTS `source_info_68`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_68` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_69`
--

DROP TABLE IF EXISTS `source_info_69`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_69` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_7`
--

DROP TABLE IF EXISTS `source_info_7`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_7` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_70`
--

DROP TABLE IF EXISTS `source_info_70`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_70` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_71`
--

DROP TABLE IF EXISTS `source_info_71`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_71` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_72`
--

DROP TABLE IF EXISTS `source_info_72`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_72` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_73`
--

DROP TABLE IF EXISTS `source_info_73`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_73` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_74`
--

DROP TABLE IF EXISTS `source_info_74`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_74` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_75`
--

DROP TABLE IF EXISTS `source_info_75`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_75` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_76`
--

DROP TABLE IF EXISTS `source_info_76`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_76` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_77`
--

DROP TABLE IF EXISTS `source_info_77`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_77` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_78`
--

DROP TABLE IF EXISTS `source_info_78`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_78` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_79`
--

DROP TABLE IF EXISTS `source_info_79`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_79` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_8`
--

DROP TABLE IF EXISTS `source_info_8`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_8` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_80`
--

DROP TABLE IF EXISTS `source_info_80`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_80` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_81`
--

DROP TABLE IF EXISTS `source_info_81`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_81` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_82`
--

DROP TABLE IF EXISTS `source_info_82`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_82` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_83`
--

DROP TABLE IF EXISTS `source_info_83`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_83` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_84`
--

DROP TABLE IF EXISTS `source_info_84`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_84` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_85`
--

DROP TABLE IF EXISTS `source_info_85`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_85` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_86`
--

DROP TABLE IF EXISTS `source_info_86`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_86` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_87`
--

DROP TABLE IF EXISTS `source_info_87`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_87` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_88`
--

DROP TABLE IF EXISTS `source_info_88`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_88` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_89`
--

DROP TABLE IF EXISTS `source_info_89`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_89` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_9`
--

DROP TABLE IF EXISTS `source_info_9`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_9` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_90`
--

DROP TABLE IF EXISTS `source_info_90`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_90` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_91`
--

DROP TABLE IF EXISTS `source_info_91`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_91` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_92`
--

DROP TABLE IF EXISTS `source_info_92`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_92` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_93`
--

DROP TABLE IF EXISTS `source_info_93`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_93` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_94`
--

DROP TABLE IF EXISTS `source_info_94`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_94` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_95`
--

DROP TABLE IF EXISTS `source_info_95`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_95` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_96`
--

DROP TABLE IF EXISTS `source_info_96`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_96` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_97`
--

DROP TABLE IF EXISTS `source_info_97`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_97` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_98`
--

DROP TABLE IF EXISTS `source_info_98`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_98` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_info_99`
--

DROP TABLE IF EXISTS `source_info_99`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_info_99` (
  `gcid` varchar(40) NOT NULL,
  `sourcetype` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `copynum` int(10) default '0',
  PRIMARY KEY  (`gcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_valid_ip`
--

DROP TABLE IF EXISTS `t_valid_ip`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_valid_ip` (
  `ipbeg` int(10) unsigned default '0',
  `ipend` int(10) unsigned default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `task_info`
--

DROP TABLE IF EXISTS `task_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task_info` (
  `taskgcid` varchar(40) NOT NULL,
  `starttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `tasktype` int(10) default '0',
  `tasktarget` int(10) default '0',
  `sourcetype` int(10) default '0',
  `copynum` int(10) default '0',
  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`taskgcid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-09-13 18:39:47
