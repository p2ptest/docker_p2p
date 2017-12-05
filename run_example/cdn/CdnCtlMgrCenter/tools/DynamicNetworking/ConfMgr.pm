#!/usr/bin/perl
package ConfMgr;
use strict;
use warnings;

BEGIN
{
	unshift @INC, 'mylib';
	#unshif»áµ¼ÖÂ¾¯¸æ
}

use MyConfig;

our $g_objDbConf;

sub Init()
{
	$g_objDbConf = MyConfig::->new("db.conf");
	my $nRet = $g_objDbConf->Init();
	return -1 if ($nRet != 0);
	#$g_objDbConf->PrintConf();

	return 0;
}

1;
