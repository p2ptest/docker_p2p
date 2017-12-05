-- MySQL dump 10.13  Distrib 5.6.30, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: monitorcenter
-- ------------------------------------------------------
-- Server version	5.6.30-1

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
-- Table structure for table `t_cluster_conf`
--

DROP TABLE IF EXISTS `t_cluster_conf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cluster_conf` (
  `ClusterId` int(11) NOT NULL,
  `bandwidth` int(12) NOT NULL,
  `bandwidthuse` int(12) NOT NULL DEFAULT '0',
  `limitmode` int(3) NOT NULL DEFAULT '0',
  `limitspeed` int(11) NOT NULL,
  `maxspeed` int(11) NOT NULL DEFAULT '0',
  `multiplespeed` int(11) NOT NULL DEFAULT '0',
  `status` int(11) NOT NULL DEFAULT '1',
  `ishot` int(3) NOT NULL DEFAULT '0',
  `hash` int(3) NOT NULL DEFAULT '0',
  `isp` int(3) NOT NULL DEFAULT '0',
  `Comment` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`ClusterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_cluster_conf_nonvp`
--

DROP TABLE IF EXISTS `t_cluster_conf_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cluster_conf_nonvp` (
  `ClusterId` int(11) NOT NULL,
  `bandwidth` int(12) NOT NULL,
  `bandwidthuse` int(12) NOT NULL DEFAULT '0',
  `limitspeed` int(11) NOT NULL,
  `status` int(11) NOT NULL DEFAULT '1',
  `ishot` int(3) NOT NULL DEFAULT '0',
  `isp` int(3) NOT NULL DEFAULT '0',
  `Comment` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`ClusterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_cluster_info`
--

DROP TABLE IF EXISTS `t_cluster_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cluster_info` (
  `clusterid` int(10) unsigned NOT NULL,
  `hostid` int(12) unsigned NOT NULL,
  `vpexternalip` varchar(26) COLLATE utf8_bin NOT NULL,
  `vpinternalip` varchar(26) COLLATE utf8_bin NOT NULL,
  `enableflag` int(10) NOT NULL DEFAULT '1',
  `updatetime` datetime NOT NULL,
  `Comment` varchar(512) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`hostid`),
  UNIQUE KEY `clusterid` (`clusterid`,`vpexternalip`,`vpinternalip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_cluster_info_nonvp`
--

DROP TABLE IF EXISTS `t_cluster_info_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cluster_info_nonvp` (
  `clusterid` int(10) unsigned NOT NULL,
  `hostid` int(12) unsigned NOT NULL,
  `vpexternalip` varchar(26) COLLATE utf8_bin NOT NULL,
  `vpinternalip` varchar(26) COLLATE utf8_bin NOT NULL,
  `enableflag` int(10) NOT NULL DEFAULT '1',
  `updatetime` datetime NOT NULL,
  `Comment` varchar(512) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`hostid`),
  UNIQUE KEY `clusterid` (`clusterid`,`vpexternalip`,`vpinternalip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_core_info`
--

DROP TABLE IF EXISTS `t_core_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_core_info` (
  `hostid` int(11) unsigned NOT NULL,
  `srvname` varchar(100) COLLATE utf8_bin NOT NULL,
  `num` int(11) DEFAULT NULL,
  `time_earlier` datetime NOT NULL,
  `time_later` datetime DEFAULT NULL,
  `revision` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`hostid`,`srvname`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_core_info_nonvp`
--

DROP TABLE IF EXISTS `t_core_info_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_core_info_nonvp` (
  `hostid` int(11) unsigned NOT NULL,
  `srvname` varchar(100) COLLATE utf8_bin NOT NULL,
  `num` int(11) DEFAULT NULL,
  `time_earlier` datetime NOT NULL,
  `time_later` datetime DEFAULT NULL,
  `revision` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`hostid`,`srvname`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_cpuuse_info`
--

DROP TABLE IF EXISTS `t_cpuuse_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cpuuse_info` (
  `hostid` int(12) unsigned NOT NULL,
  `corenum` int(12) unsigned NOT NULL,
  `cpuuse` int(12) unsigned NOT NULL,
  `loadvalue1` float NOT NULL,
  `loadvalue5` float NOT NULL,
  `loadvalue15` float NOT NULL,
  PRIMARY KEY (`hostid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_cpuuse_info_nonvp`
--

DROP TABLE IF EXISTS `t_cpuuse_info_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cpuuse_info_nonvp` (
  `hostid` int(12) unsigned NOT NULL,
  `corenum` int(12) unsigned NOT NULL,
  `cpuuse` int(12) unsigned NOT NULL,
  `loadvalue1` float NOT NULL,
  `loadvalue5` float NOT NULL,
  `loadvalue15` float NOT NULL,
  PRIMARY KEY (`hostid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_disk_info`
--

DROP TABLE IF EXISTS `t_disk_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_disk_info` (
  `hostid` int(12) unsigned NOT NULL,
  `diskindex` int(12) unsigned NOT NULL,
  `devname` varchar(26) COLLATE utf8_bin NOT NULL,
  `mountdir` varchar(26) COLLATE utf8_bin NOT NULL,
  `totalDiskInKB` bigint(20) NOT NULL DEFAULT '0',
  `usedDiskInKB` bigint(20) NOT NULL DEFAULT '0',
  `status` int(12) unsigned NOT NULL,
  PRIMARY KEY (`hostid`,`mountdir`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_disk_info_nonvp`
--

DROP TABLE IF EXISTS `t_disk_info_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_disk_info_nonvp` (
  `hostid` int(12) unsigned NOT NULL,
  `diskindex` int(12) unsigned NOT NULL,
  `devname` varchar(26) COLLATE utf8_bin NOT NULL,
  `mountdir` varchar(26) COLLATE utf8_bin NOT NULL,
  `totalDiskInKB` bigint(20) NOT NULL DEFAULT '0',
  `usedDiskInKB` bigint(20) NOT NULL DEFAULT '0',
  `status` int(12) unsigned NOT NULL,
  PRIMARY KEY (`hostid`,`mountdir`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_hostip_info`
--

DROP TABLE IF EXISTS `t_hostip_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_hostip_info` (
  `hostid` int(12) unsigned NOT NULL,
  `vpexternalip` varchar(26) COLLATE utf8_bin NOT NULL,
  `vpinternalip` varchar(26) COLLATE utf8_bin NOT NULL,
  `enableflag` int(10) NOT NULL DEFAULT '1',
  PRIMARY KEY (`hostid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_manualctl_tbl`
--

DROP TABLE IF EXISTS `t_manualctl_tbl`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_manualctl_tbl` (
  `hostid` int(11) NOT NULL,
  `updatetime` datetime NOT NULL,
  `Comment` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`hostid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_memuse_info`
--

DROP TABLE IF EXISTS `t_memuse_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_memuse_info` (
  `hostid` int(12) unsigned NOT NULL,
  `totalMemInKB` bigint(20) NOT NULL DEFAULT '0',
  `usedMemInKB` bigint(20) NOT NULL DEFAULT '0',
  `usedVisualMemInKB` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`hostid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_memuse_info_nonvp`
--

DROP TABLE IF EXISTS `t_memuse_info_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_memuse_info_nonvp` (
  `hostid` int(12) unsigned NOT NULL,
  `totalMemInKB` bigint(20) NOT NULL DEFAULT '0',
  `usedMemInKB` bigint(20) NOT NULL DEFAULT '0',
  `usedVisualMemInKB` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`hostid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_networkcard_info`
--

DROP TABLE IF EXISTS `t_networkcard_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_networkcard_info` (
  `hostid` int(12) unsigned NOT NULL,
  `cardindex` int(12) unsigned NOT NULL,
  `vpip` varchar(26) COLLATE utf8_bin NOT NULL,
  `bandwidth` bigint(20) NOT NULL DEFAULT '0',
  `bandsendused` bigint(20) NOT NULL DEFAULT '0',
  `bandrecvused` bigint(20) NOT NULL DEFAULT '0',
  `ifpublic` int(12) unsigned NOT NULL,
  `netcardmode` int(12) unsigned NOT NULL,
  `status` int(12) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`hostid`,`cardindex`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_networkcard_info_nonvp`
--

DROP TABLE IF EXISTS `t_networkcard_info_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_networkcard_info_nonvp` (
  `hostid` int(12) unsigned NOT NULL,
  `cardindex` int(12) unsigned NOT NULL,
  `vpip` varchar(26) COLLATE utf8_bin NOT NULL,
  `bandwidth` bigint(20) NOT NULL DEFAULT '0',
  `bandsendused` bigint(20) NOT NULL DEFAULT '0',
  `bandrecvused` bigint(20) NOT NULL DEFAULT '0',
  `ifpublic` int(12) unsigned NOT NULL,
  `netcardmode` int(12) unsigned NOT NULL,
  `status` int(12) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`hostid`,`cardindex`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_special_ip_map`
--

DROP TABLE IF EXISTS `t_special_ip_map`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_special_ip_map` (
  `srcip` varchar(26) NOT NULL,
  `mapip` varchar(26) NOT NULL,
  `status` int(10) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`srcip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_special_ip_map_nonvp`
--

DROP TABLE IF EXISTS `t_special_ip_map_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_special_ip_map_nonvp` (
  `srcip` varchar(26) NOT NULL,
  `mapip` varchar(26) NOT NULL,
  `status` int(10) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`srcip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_tcp_netstate`
--

DROP TABLE IF EXISTS `t_tcp_netstate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_tcp_netstate` (
  `hostid` int(12) unsigned NOT NULL,
  `Total` int(12) unsigned NOT NULL,
  `LISTEN` int(12) unsigned NOT NULL,
  `SYN_SENT` int(12) unsigned NOT NULL,
  `SYN_RECV` int(12) unsigned NOT NULL,
  `ESTABLISHED` int(12) unsigned NOT NULL,
  `FIN_WAIT1` int(12) unsigned NOT NULL,
  `CLOSE_WAIT` int(12) unsigned NOT NULL,
  `LAST_ACK` int(12) unsigned NOT NULL,
  `FIN_WAIT2` int(12) unsigned NOT NULL,
  `TIME_WAIT` int(12) unsigned NOT NULL,
  `CLOSING` int(12) unsigned NOT NULL,
  `CLOSED` int(12) unsigned NOT NULL,
  PRIMARY KEY (`hostid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `t_tcp_netstate_nonvp`
--

DROP TABLE IF EXISTS `t_tcp_netstate_nonvp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_tcp_netstate_nonvp` (
  `hostid` int(12) unsigned NOT NULL,
  `Total` int(12) unsigned NOT NULL,
  `LISTEN` int(12) unsigned NOT NULL,
  `SYN_SENT` int(12) unsigned NOT NULL,
  `SYN_RECV` int(12) unsigned NOT NULL,
  `ESTABLISHED` int(12) unsigned NOT NULL,
  `FIN_WAIT1` int(12) unsigned NOT NULL,
  `CLOSE_WAIT` int(12) unsigned NOT NULL,
  `LAST_ACK` int(12) unsigned NOT NULL,
  `FIN_WAIT2` int(12) unsigned NOT NULL,
  `TIME_WAIT` int(12) unsigned NOT NULL,
  `CLOSING` int(12) unsigned NOT NULL,
  `CLOSED` int(12) unsigned NOT NULL,
  PRIMARY KEY (`hostid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-26 10:32:27
