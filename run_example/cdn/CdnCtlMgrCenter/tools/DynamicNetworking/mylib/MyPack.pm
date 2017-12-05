#!/usr/bin/perl
package MyPack;
use strict;
use warnings;

sub NTOHLL()
{
	#my ($nULongLong) = @_;
	my @arrBuf = unpack('N2', pack('Q', $_[0]));
	return unpack('Q',pack('I2', $arrBuf[1],$arrBuf[0]));
}

sub HTONLL()
{
	#my ($nULongLong) = @_;
	my @arrBuf = unpack('N2', pack('Q', $_[0]));
	return unpack('Q',pack('I2', $arrBuf[1],$arrBuf[0]));
}

sub NTOHL()
{
	#my ($nUInt) = @_;
	return unpack('L', pack('N', $_[0]))
}

sub HTONL()
{
	#my ($nUInt) = @_;
	return unpack('L', pack('N', $_[0]))
}

sub NTOHS() 
{
	#my ($nUShort) = @_;
	return unpack('S', pack('n', $_[0]))
}

sub HTONS()
{
	#my ($nUShort) = @_;
	return unpack('S', pack('n', $_[0]))
}

sub HexToBi()
{
	#my ($strHexString) = @_;
	my $strRet = '';
	my $nLen = length($_[0]);
	if($nLen % 2 != 0)
	{
		return $strRet;
	}
	if($_[0] =~ /[0-9a-fA-F]{$nLen,$nLen}/)
	{
		$strRet .= pack('H*', $_[0]);
		return $strRet
	}
	else
	{
		return $strRet;
	}
}

sub BiToHex()
{
	#my ($strBinString) = @_;
	my $strRet = unpack('H*',$_[0]);
	$strRet =~ tr/[a-z]/[A-Z]/;
	return $strRet;
}

sub ToUpper()
{
	my ($strString) = @_;
	$strString  =~ tr/[a-z]/[A-Z]/;
	return $strString;
}

sub ToLower()
{
	my ($strString) = @_;
	$strString  =~ tr/[A-Z]/[a-z]/;
	return $strString;
}

sub ShortELFHash()
{
	my ($strString) = @_;
	my $nH = 0;
	my $nG = 0;
	my $chChar = '';
	my $nChar = 0;
	my $nLength = length($strString);

	for(my $nA = 0; $nA < $nLength; ++ $nA)
	{
		$chChar = substr($strString, $nA, 1);
		$nChar = unpack('c', $chChar);
		$nH = ($nH << 4) + $nChar;
		$nG = $nH & 0xF0000000;
		if($nG)
		{
			$nH ^= $nG >> 24;
		}
		$nH &= ~$nG;
	}
	return $nH;
}

sub HexShortELFHash()
{
	#my ($strHexString) = @_;
	my $strBiString = &HexToBi($_[0]);
	return &ShortELFHash($strBiString);
}

#pack 和 unpack模式：#############################################################
#a -> string(C风格字符串)
#c -> char
#C -> unsigned char
#s -> short
#S -> unsigned short
#i -> int
#I -> unsigned int
#l -> long
#L -> unsigned long
#j -> long long
#J -> unsigned long long(a Perl internal integer)
#j -> long long(a Perl internal unsigned integer, UV)
#J -> unsigned long long
#q -> long long
#Q -> unsigned long long

#b -> A bit string (ascending bit order inside each byte, like vec()).
#B -> A bit string (descending bit order inside each byte).
#h -> A hex string (low nybble first).
#H -> A hex string (high nybble first).

#n -> An unsigned short in "network" (big-endian) order
#N -> An unsigned long in "network" (big-endian) order.
#v -> An unsigned short in "VAX" (little-endian) order.
#V -> An unsigned long in "VAX" (little-endian) order.

################################################################################
1;
