#!/usr/bin/perl
package MyBuffer;
use strict;
use warnings;

sub PutChar()
{
	#my ($szBuf, $nChar) = @_;
	$_[0] .= pack("C", $_[1]);
	return 0;
}

sub PutShort()
{
	#my ($szBuf, $nShort) = @_;
	$_[0] .= pack("S", $_[1]);
	return 0;
}

sub PutInt()
{
	#my ($szBuf, $nInt) = @_;
	$_[0] .= pack("I", $_[1]);
	return 0;
}

sub PutLong64()
{
	#my ($szBuf, $nLong64) = @_;
	$_[0] .= pack("Q", $_[1]);
	return 0;
}

sub PutString()
{
	#my ($szBuf, $strString) = @_;
	my $nLenth = length($_[1]);
	$_[0] .= pack("I", $nLenth);
	$_[0] .= pack("a$nLenth", $_[1]);
	return 0;
}

sub PutBuffer()
{
	#my ($szBuf, $strString) = @_;
	my $nLenth = length($_[1]);
	$_[0] .= pack("a$nLenth", $_[1]);
	return 0;
}

#==================================================
sub GetChar()
{
	#my ($szBuf, $nChar) = @_;
	my $nLenth = length($_[0]);
	if($nLenth < 1)
	{
		return -1;
	}
	$_[1] = unpack("C", $_[0]);
	$_[0] = substr($_[0], 1, $nLenth - 1);
	return 0;
}

sub GetShort()
{
	#my ($szBuf, $nShort) = @_;
	my $nLenth = length($_[0]);
	if($nLenth < 2)
	{
		return -1;
	}
	$_[1] = unpack("S", $_[0]);
	$_[0] = substr($_[0], 2, $nLenth - 2);
	return 0;
}

sub GetInt()
{
	#my ($szBuf, $nInt) = @_;
	my $nLenth = length($_[0]);
	if($nLenth < 4)
	{
		return -1;
	}
	$_[1] = unpack("I", $_[0]);
	$_[0] = substr($_[0], 4, $nLenth - 4);
	return 0;
}

sub GetLong64()
{
	#my ($szBuf, $nLong64) = @_;
	my $nLenth = length($_[0]);
	if($nLenth < 8)
	{
		return -1;
	}
	$_[1] = unpack("Q", $_[0]);
	$_[0] = substr($_[0], 8, $nLenth - 8);
	return 0;
}

sub GetString()
{
	#my ($szBuf, $nString) = @_;
	my $nLenth = length($_[0]);
	if($nLenth < 4)
	{
		return -1;
	}
	my $nStrLen = unpack("I", $_[0]);
	$_[0] = substr($_[0], 4, $nLenth - 4);
	$nLenth -= 4;
	if($nStrLen < 0 || $nStrLen > $nLenth)
	{
		return -1;
	}
	$_[1] = unpack("a$nStrLen", $_[0]);
	$_[0] = substr($_[0], $nStrLen, $nLenth - $nStrLen);
	return 0;
}

sub GetBuffer()
{
	#my ($szBuf, $strString, $nGetLength) = @_;
	my $nLenth = length($_[0]);
	if($nLenth < $_[2])
	{
		return -1;
	}
	$_[1] = unpack("a$_[2]", $_[0]);
	$_[0] = substr($_[0], $_[2], $nLenth - $_[2]);
	return 0;
}
1;