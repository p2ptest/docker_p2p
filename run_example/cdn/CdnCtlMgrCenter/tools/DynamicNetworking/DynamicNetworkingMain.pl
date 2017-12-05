#!/usr/bin/perl
use strict;
use warnings;

BEGIN
{
	unshift @INC, 'mylib';
	#unshif»áµ¼ÖÂ¾¯¸æ
}

use ConfMgr;
use DynamicNetworking;

my $nRet = ConfMgr::Init();
if ($nRet != 0)
{
	print "ConfMgr::Init failed!\n";
	exit -1;
}

$nRet = DynamicNetworking::LoadDB($ConfMgr::g_objDbConf);
if ($nRet != 0)
{
	print "DynamicNetworking::LoadDB failed!\n";
	exit -1;
}

$nRet = DynamicNetworking::DynamicNetwork();
if ($nRet != 0)
{
	print "DynamicNetworking::DynamicNetwork failed!\n";
	exit -1;
}

$nRet = DynamicNetworking::CheckIfNetworkChange($ConfMgr::g_objDbConf);
if ($nRet == 0)
{
	print "Network not change\n";
	exit 0;
}
elsif ($nRet < 0)
{
	print "CheckIfNetworkChange fail\n";
	exit -1;
}

$nRet = DynamicNetworking::WriteDB($ConfMgr::g_objDbConf);
if ($nRet != 0)
{
	print "DynamicNetworking::WriteDB failed!\n";
	exit -1;
}

print "DynamicNetwork seccessful!\n";

