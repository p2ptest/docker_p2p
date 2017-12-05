-- MySQL dump 10.13  Distrib 5.6.30, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: movie_box_source
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
-- Table structure for table `p2pvod_videos`
--

DROP TABLE IF EXISTS `p2pvod_videos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p2pvod_videos` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '视频ID',
  `status` tinyint(4) DEFAULT '0' COMMENT '视频状态 0.新增,1.更新,2.下线',
  `name` varchar(500) NOT NULL COMMENT '视频名称',
  `img_url` varchar(500) DEFAULT NULL COMMENT '视频图片',
  `url` varchar(500) NOT NULL COMMENT '视频播放地址 (p2p地址)',
  `flvs` varchar(2000) DEFAULT NULL COMMENT '没用',
  `swf` varchar(2000) DEFAULT NULL COMMENT '没用',
  `editor_score` tinyint(4) DEFAULT '0' COMMENT '高清为100 xml的hd为1时是100,其他是0',
  `user_score` tinyint(4) DEFAULT '0' COMMENT '没用',
  `video_time` int(11) DEFAULT '0' COMMENT '播放时长(毫秒)',
  `video_type` int(11) DEFAULT '0' COMMENT '高清为100 xml的hd为1时是100,其他是0',
  `iid` varchar(1000) NOT NULL COMMENT '播放源唯一标识',
  `update_time` bigint(20) DEFAULT NULL COMMENT '上线更新时间(毫秒)',
  `site_category_id` smallint(6) DEFAULT NULL COMMENT '视频分类ID 对应着site_categories.id',
  `description` text COMMENT '视频描述',
  `tags` varchar(1000) DEFAULT NULL COMMENT '视频的tags 以!!!分隔',
  `created_at` bigint(20) DEFAULT NULL COMMENT '视频第一次上线时间(毫秒)',
  `extension` text COMMENT '视频扩展信息',
  `sub_title` varchar(255) DEFAULT NULL COMMENT '视频副标题',
  `ispay` tinyint(1) unsigned DEFAULT '0' COMMENT '视频是否收费 1.是',
  `dzone` tinyint(1) unsigned NOT NULL COMMENT '视频是否下载 1.pc下载 2.是手机下载 3.手机pc全部下载',
  `phone_status` tinyint(1) DEFAULT NULL COMMENT '手机视频视频是否可用,1可用,2不可用',
  `icp` varchar(100) NOT NULL DEFAULT '' COMMENT '备案号',
  `icp_start_time` varchar(10) NOT NULL DEFAULT '0' COMMENT '备案号开始时间,格式时分秒',
  `icp_end_time` varchar(10) NOT NULL DEFAULT '0' COMMENT '备案号结束时间,格式时分秒',
  `m_status` tinyint(1) unsigned DEFAULT NULL COMMENT '视频播放源是否可用 0.新增,1.更新,2.下线',
  `m_update_time` bigint(20) unsigned DEFAULT NULL COMMENT '视频播放源建立时间(毫秒)',
  `m_create_time` bigint(20) unsigned DEFAULT NULL COMMENT '视频播放源更新时间(毫秒)',
  `update_at` bigint(20) unsigned DEFAULT NULL COMMENT 'cms最后编辑时间(毫秒)',
  `publish_date` bigint(20) unsigned DEFAULT NULL COMMENT '视频的发行日期',
  `isimage` tinyint(1) unsigned DEFAULT NULL COMMENT '视频是否有图 1.是',
  `hide` tinyint(1) unsigned DEFAULT NULL COMMENT '视频是否隐藏 1.是',
  `box_tpl` tinyint(1) unsigned DEFAULT '0' COMMENT '视频模板 4.综艺模板,0默认模板',
  `brief` text COMMENT '视频看点',
  `3d` tinyint(1) unsigned DEFAULT '0' COMMENT '是否是3d 0.不是 1是',
  `right_ear` tinyint(1) unsigned DEFAULT '0' COMMENT '是否右耳 0.不是,1.是',
  `left_eye` tinyint(1) unsigned DEFAULT '0' COMMENT '是否左眼推荐 0.不是,1.是',
  `exclusive` tinyint(1) unsigned DEFAULT '0' COMMENT '是否全网独家 0.不是,1.是',
  `is_encryption` tinyint(1) unsigned DEFAULT '0' COMMENT '是否加密 0.未加密,1.加密',
  `encryption_key` varchar(500) DEFAULT NULL COMMENT '加密密钥',
  `guid` varchar(500) DEFAULT NULL COMMENT '视频GUID',
  `caption` tinyint(1) unsigned DEFAULT '0' COMMENT '视频字幕',
  `flash` tinyint(1) DEFAULT '0' COMMENT '是否支持flash播放。1，支持；0，不支持',
  `wvid` varchar(200) DEFAULT NULL COMMENT '合作方的影片id',
  `size` bigint(20) unsigned DEFAULT '0' COMMENT '视频大小（B）',
  `file_md5` char(32) NOT NULL COMMENT '文件md5值',
  `is_preview` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否为预告片',
  `start` int(11) NOT NULL DEFAULT '0' COMMENT '片头时间',
  `end` int(11) NOT NULL DEFAULT '0' COMMENT '片尾时间',
  `tl` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否有时间线信息',
  `is_vadd` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否v+',
  `encrypt_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '加密类型,1:六大类型',
  PRIMARY KEY (`id`),
  UNIQUE KEY `iid_index_unique` (`iid`(255)),
  KEY `videos_url_index` (`url`(255)),
  KEY `update_time_index` (`update_time`),
  KEY `status` (`status`),
  KEY `guid` (`guid`(333)),
  KEY `wvid_idx` (`wvid`)
) ENGINE=MyISAM AUTO_INCREMENT=2877533 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-26 10:43:07
