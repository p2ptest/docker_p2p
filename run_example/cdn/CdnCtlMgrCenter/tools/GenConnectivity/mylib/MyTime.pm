#!/usr/bin/perl
package MyTime;
use strict;
use warnings;

use Time::Local;
use Time::HiRes;

sub Sleep()
{
	#my ($nSeepMicroSecond) = (@_);
	my $nSeepTime = $_[0] / 1000;
	select(undef, undef, undef, $nSeepTime);
}

sub GetTime()
{
	return time();
}

sub GetTimeOfDay()
{
	my ($nSecond, $nMicroSecond) = Time::HiRes::gettimeofday();
	return ($nSecond, $nMicroSecond);
}

sub GetLocalDateString()
{
	my @arrDate;
	if(@_ == 1)
	{
		@arrDate = localtime($_[0]);
	}
	else
	{
		@arrDate = localtime();
	}
	return sprintf("%04d%02d%02d", 1900 + $arrDate[5], $arrDate[4] + 1, $arrDate[3]);
}

sub GetLocalTime()
{
	#  0    1    2     3     4    5     6     7     8
	#($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime;
	if(@_ == 1)
	{
		return localtime($_[0]);
	}
	else
	{
		return localtime();
	}
}

sub GetLocalStringTime()
{
	my @arrLocalTime;
	if(@_ == 1)
	{
		@arrLocalTime = localtime($_[0]);
	}
	else
	{
		@arrLocalTime = localtime();
	}
	 
	$arrLocalTime[4] ++; 
	$arrLocalTime[5] += 1900;
	my $strStringTime =  sprintf("%04d", $arrLocalTime[5])."-";
	$strStringTime .= sprintf("%02d", $arrLocalTime[4])."-";
	$strStringTime .= sprintf("%02d", $arrLocalTime[3])." ";
	$strStringTime .= sprintf("%02d", $arrLocalTime[2]).":";
	$strStringTime .= sprintf("%02d", $arrLocalTime[1]).":";
	$strStringTime .= sprintf("%02d", $arrLocalTime[0]);
	return $strStringTime;
}

sub GetGMTTime()
{
	#  0    1    2     3     4    5     6     7     8
	#($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = gmtime;
	return gmtime();
}

sub GetGMTStringTime()
{
	my @arrLocalTime = gmtime();
	$arrLocalTime[4] ++; 
	$arrLocalTime[5] += 1900;
	my $strStringTime =  sprintf("%04d", $arrLocalTime[5])."-";
	$strStringTime .= sprintf("%02d", $arrLocalTime[4])."-";
	$strStringTime .= sprintf("%02d", $arrLocalTime[3])." ";
	$strStringTime .= sprintf("%02d", $arrLocalTime[2]).":";
	$strStringTime .= sprintf("%02d", $arrLocalTime[1]).":";
	$strStringTime .= sprintf("%02d", $arrLocalTime[0]);
	return $strStringTime;
}

sub GetMonthString()
{
	#my ($nLocalTimeMonth) = @_;
	return (qw(Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec))[$_[0]];
}

sub GetWeekString()
{
	#my ($nLocalTimeWeekDay) = @_;
	return (qw(Sun Mon Tue Wed Thu Fri Sat))[$_[0]];
}

sub TransToUnixTime()
{
	my ($nSec, $nMin, $nHour, $nDay, $nMon, $nYear);
	if (@_ == 1)
	{
		if ($_[0] =~ /^(\d{4})-(\d{1,2})-(\d{1,2})[ ]+(\d{1,2}):(\d{1,2}):(\d{1,2})$/)
		{
			($nSec, $nMin, $nHour, $nDay, $nMon, $nYear) = ($6, $5, $4, $3, $2, $1);
		}
		elsif ($_[0] =~ /^(\d{4})-(\d{1,2})-(\d{1,2}) (\d{1,2}):(\d{1,2})$/)
		{
			($nSec, $nMin, $nHour, $nDay, $nMon, $nYear) = (0, $5, $4, $3, $2, $1);
		}
		elsif ($_[0] =~ /^(\d{4})-(\d{1,2})-(\d{1,2})$/)
		{
			($nSec, $nMin, $nHour, $nDay, $nMon, $nYear) = (0, 0, 0, $3, $2, $1);
		}
	}
	elsif (@_ == 6)
	{
		($nSec, $nMin, $nHour, $nDay, $nMon, $nYear) = reverse @_;
		
	}
	else
	{
		return -1;
	}
	
	$nYear = $nYear - 1900;
	$nMon = $nMon -1;
	return timelocal($nSec, $nMin, $nHour, $nDay, $nMon, $nYear);
}

sub TransFromUnixTime()
{
	my ($nUnixTime) = @_;
	return &GetLocalStringTime($nUnixTime);
}

1;