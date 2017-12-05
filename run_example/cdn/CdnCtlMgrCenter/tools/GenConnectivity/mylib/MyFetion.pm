#!/usr/bin/perl
package MyFetion;
use strict;
use warnings;

sub new()
{
	my $refHashSelf = {};
	shift;
	my ($strFetionUser, $strFetionPasswd) = @_;
	$refHashSelf->{User} = $strFetionUser;
	$refHashSelf->{Password} = $strFetionPasswd;
	$refHashSelf->{LibDir} = './fetion2010';
	$refHashSelf->{DstFetionAccount} = {};
	bless ($refHashSelf);
	return $refHashSelf;
}

sub SetFetionAccount()
{
	my $refHashSelf = shift;
	my ($strFetionUser, $strFetionPasswd) = @_;
	$refHashSelf->{User} = $strFetionUser;
	$refHashSelf->{Password} = $strFetionPasswd;
	return 0;
}

sub SetDstFetionAccount()
{
	my $refHashSelf = shift;
	my ($refFetionAccount) = @_;
	foreach my $strFetionDes (sort keys %{$refFetionAccount})
	{
		$refHashSelf->{DstFetionAccount}{$strFetionDes} = $refFetionAccount->{$strFetionDes};
		#print "$strFetionDes ".$refFetionAccount->{$strFetionDes}."\n";
	}
	return 0;
}

sub SendMsg()
{
	my $refHashSelf = shift;
	my ($strSubject ,$strMsgString) = @_;
	my $strUser = $refHashSelf->{User};
	my $strPasswd = $refHashSelf->{Password};
	my $strLibDir = $refHashSelf->{LibDir};	

	my $nRet = 0;
	$strMsgString = "$strSubject\n$strMsgString";
	
	foreach my $strFestionDes (keys %{$refHashSelf->{DstFetionAccount}})
	{
		my $strToMobile = $refHashSelf->{DstFetionAccount}{$strFestionDes};
		#print "$strUser $strFestionDes ".$refHashSelf->{DstFetionAccount}{$strFestionDes}."\n";
		`LD_LIBRARY_PATH=$strLibDir $strLibDir/fetion --msg-type=1 --mobile=$strUser  --pwd=$strPasswd --to=$strToMobile --msg-utf8='\n$strMsgString' --exit-on-verifycode`;
		my $nRet = $? >> 8;
		if ($nRet != 0)
		{
			print "Fetion Send To $strFestionDes Fail\n";
			$nRet ++;
		}
	}
	return $nRet;
}

1;
