#!/usr/bin/perl 
#===============================================================================
#
#         FILE: GenConnectivity.pl
#
#        USAGE: ./GenConnectivity.pl  
#
#  DESCRIPTION: 
#
#      OPTIONS: ---
# REQUIREMENTS: ---
#         BUGS: ---
#        NOTES: ---
#       AUTHOR: wangtiefeng (), wangtf418@163.com
#      COMPANY: BaoFeng Inc.
#      VERSION: 1.0
#      CREATED: 2011年10月05日 17时40分28秒
#     REVISION: ---
#===============================================================================

use strict;
use warnings;


BEGIN
{
	unshift @INC, 'mylib';
}

use MySql;

####################################
my %s_hashClusterInfo;
my %s_hashCncClusterInfo;
my %s_hashCtClusterInfo;
my %s_hashCoCtCluster;
my %s_hashCoCncCluster;
####################################

my $strDbName  = 'dynamicnetwork';
my $strDbHost = '127.0.0.1';
my $strDbUser = 'root';
my $strDbPasswd = '';

my $objMysql = new MySql($strDbName, $strDbHost, $strDbUser, $strDbPasswd);
my $nRet = $objMysql->Connect();
if ($nRet != 0)
{
	print "Connect Db Fail\n";
	exit -1;
}

my $strQuery = "select * from t_cluster_info;";
$nRet = $objMysql->Execute($strQuery);
if ($nRet != 0)
{
	print "select from t_cluster_info fail\n";
	exit -1;
}
while (my $refRet = $objMysql->FetchrowHashRef()) 
{
	my $nClusterId = $refRet->{clusterid};	
	my $nLevel = $refRet->{level};
	my $strIsp = $refRet->{isp};
	my $strProvince = $refRet->{province};
	my $strCity = $refRet->{city};
	my $nAttribute = $refRet->{attribute};
	if ($nClusterId < 3000)
	{
		$s_hashClusterInfo{$nClusterId} = [$strIsp, $strProvince, $strCity, $nAttribute];
		$s_hashCncClusterInfo{$nClusterId} = [$strIsp, $strProvince, $strCity, $nAttribute] if ($strIsp eq 'wangtong' && $nLevel == 1);
		$s_hashCtClusterInfo{$nClusterId} = [$strIsp, $strProvince, $strCity, $nAttribute] if ($strIsp eq 'dianxin' && $nLevel == 1);
	}
	else
	{
		if ($strIsp =~ /^([a-z]+)_cnc$/)	
		{
			$s_hashCoCncCluster{$nClusterId} = [$1, $strProvince, $strCity, $nAttribute];
		}
		elsif ($strIsp =~ /^([a-z]+)_ct$/)
		{
			$s_hashCoCtCluster{$nClusterId} = [$1, $strProvince, $strCity, $nAttribute];
		}
	}
}

my %hashCopy = %s_hashClusterInfo;
foreach my $nClusterId (sort keys %s_hashClusterInfo)
{
	if ($nClusterId < 1000 || $nClusterId > 3000)
	{
		next;
	}
	foreach my $nDstClusterId (sort keys %hashCopy)
	{
		next if ($nClusterId == $nDstClusterId);
		next if ($s_hashClusterInfo{$nClusterId}[0] ne $hashCopy{$nDstClusterId}[0] && $nDstClusterId > 1000);

		my $nConnectTity = 0;
		if ($nDstClusterId < 1000)
		{
			$nConnectTity = 90;		
		}
		elsif ($s_hashClusterInfo{$nClusterId}[1] ne $hashCopy{$nDstClusterId}[1])
		{
			if ($s_hashClusterInfo{$nClusterId}[3] != $hashCopy{$nDstClusterId}[3])
			{
				$nConnectTity = 75;
			}
			else
			{
				$nConnectTity = 80 if ($s_hashClusterInfo{$nClusterId}[3] == 1);
				$nConnectTity = 70 if ($s_hashClusterInfo{$nClusterId}[3] == 2);
			}
		}
		elsif ($s_hashClusterInfo{$nClusterId}[2] ne $hashCopy{$nDstClusterId}[2])
		{
			if ($s_hashClusterInfo{$nClusterId}[3] != $hashCopy{$nDstClusterId}[3])
			{
				$nConnectTity = 85;
			}
			else
			{
				$nConnectTity = 90 if ($s_hashClusterInfo{$nClusterId}[3] == 1);
				$nConnectTity = 80 if ($s_hashClusterInfo{$nClusterId}[3] == 2);
			}
		}
		else
		{
			$nConnectTity = 100 if ($s_hashClusterInfo{$nClusterId}[3] == 1);
			$nConnectTity = 95 if ($s_hashClusterInfo{$nClusterId}[3] == 2);
		}
		$strQuery = "replace into t_connectivity values($nClusterId, $nDstClusterId, $nConnectTity, 1);";
		$objMysql->Execute($strQuery);
	}
}

foreach my $nClusterId (sort keys %s_hashCoCtCluster)
{
	for my $nDstClusterId (sort keys %s_hashCtClusterInfo)
	{
		my $nConnectTity = 80;			
		$strQuery = "replace into t_connectivity values($nClusterId, $nDstClusterId, $nConnectTity, 1);";
		$objMysql->Execute($strQuery);
	}
}
foreach my $nClusterId (sort keys %s_hashCoCncCluster)
{
	for my $nDstClusterId (sort keys %s_hashCncClusterInfo)
	{
		my $nConnectTity = 80;			
		$strQuery = "replace into t_connectivity values($nClusterId, $nDstClusterId, $nConnectTity, 1);";
		$objMysql->Execute($strQuery);
	}
}







