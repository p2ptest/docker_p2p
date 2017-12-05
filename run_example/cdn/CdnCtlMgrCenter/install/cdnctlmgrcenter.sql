-- MySQL dump 10.11
--
-- Host: localhost    Database: cdnctlmgrcenter
-- ------------------------------------------------------
-- Server version	5.0.77

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
-- Table structure for table `CdnClusterRelation_tbl`
--

DROP TABLE IF EXISTS `CdnClusterRelation_tbl`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `CdnClusterRelation_tbl` (
  `ClusterId` int(11) NOT NULL,
  `Level` int(11) NOT NULL,
  `ParentClusterId` int(11) NOT NULL,
  `IfUserPublicIp` tinyint(3) NOT NULL default '1',
  `Comment` varchar(512) default NULL,
  PRIMARY KEY  (`ClusterId`,`ParentClusterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `CdnClusterRelation_tbl`
--

LOCK TABLES `CdnClusterRelation_tbl` WRITE;
/*!40000 ALTER TABLE `CdnClusterRelation_tbl` DISABLE KEYS */;
INSERT INTO `CdnClusterRelation_tbl` VALUES (2,2,1,1,NULL),(3,3,1,0,NULL);
/*!40000 ALTER TABLE `CdnClusterRelation_tbl` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `CdnInfo_tbl`
--

DROP TABLE IF EXISTS `CdnInfo_tbl`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `CdnInfo_tbl` (
  `ClusterId` int(11) NOT NULL,
  `HostId` int(11) NOT NULL,
  `PublicIp` varchar(30) NOT NULL,
  `PrivateIp` varchar(30) NOT NULL,
  `State` int(11) NOT NULL default '1',
  `UpdateTime` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `Comment` varchar(512) default NULL,
  PRIMARY KEY  (`HostId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `CdnInfo_tbl`
--

LOCK TABLES `CdnInfo_tbl` WRITE;
/*!40000 ALTER TABLE `CdnInfo_tbl` DISABLE KEYS */;
INSERT INTO `CdnInfo_tbl` VALUES (1,1,'192.168.1.1','192.168.1.2',0,'2010-12-30 10:04:29',NULL),(1,2,'192.168.1.3','192.168.1.4',1,'2010-12-30 10:04:29',NULL),(1,3,'192.168.1.5','192.168.1.6',1,'2010-12-30 10:04:29',NULL),(2,4,'192.168.1.7','192.168.1.8',1,'2010-12-30 10:04:29',NULL),(2,5,'192.168.1.9','192.168.1.10',1,'2010-12-30 10:04:29',NULL),(2,6,'192.168.1.11','192.168.1.12',1,'2010-12-30 10:04:29',NULL),(3,7,'192.168.1.13','192.168.1.14',1,'2010-12-30 10:04:29',NULL),(3,8,'192.168.1.15','192.168.1.16',1,'2010-12-30 10:04:29',NULL),(3,9,'192.168.1.37','192.168.1.18',1,'2010-12-30 10:04:29',NULL);
/*!40000 ALTER TABLE `CdnInfo_tbl` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-01-04 18:24:56
