#!/usr/bin/perl
package MyConfig;
use strict;
use warnings;

sub new()
{
	my $refHashSelf = {};
	shift;
	my ($strConfName) = @_;
	$refHashSelf->{ConfFileName} = $strConfName;
	$refHashSelf->{Conf} = {};
	bless ($refHashSelf);
	return $refHashSelf;
}

sub Init()
{
	my $refHashSelf = shift;
	my ($strConfigFile, $refHashConf) = ($refHashSelf->{ConfFileName}, $refHashSelf->{Conf});
	return -1 if (!(-f $strConfigFile));
	my $nRet = open(CONFFILE, "<", $strConfigFile);
	if (!defined($nRet))
	{
		print "ReadConf: open file $strConfigFile fail\n";
		return -1;
	}
	my $strCurSection = "";
	while(my $strLine = <CONFFILE>)
	{
		$strLine =~ s/\n$// if (!($strLine =~ s/\r\n$//));
		next if ($strLine =~ /^\s*$/);
		next if (substr($strLine , 0, 1) eq '#');
		next if ($strLine eq '');
		if ($strLine =~ /\[(\w+)\]/)
		{
			$strCurSection = $1;
			$refHashConf->{$strCurSection} = {};
			next;
		}
		
		my @arrRet = split('=', $strLine);
		if(@arrRet != 2 && @arrRet != 1)
		{
			print "ReadConf: $strConfigFile ErrorLine: $strLine\n";
			close(CONFFILE);	
			return -1;
		}
		if ($strCurSection eq '')
		{
			print "ReadConf[$strConfigFile] : $arrRet[0] not in any section\n";
			return -1;
		}
		$refHashConf->{$strCurSection}{$arrRet[0]} = $arrRet[1] if (@arrRet == 2);
		$refHashConf->{$strCurSection}{$arrRet[0]} = '' if (@arrRet == 1);
		#print "ReadConf[$strConfigFile] :[$strCurSection] $arrRet[0]=$refHashConf->{$strCurSection}{$arrRet[0]}\n";
	}
	close(CONFFILE);	
	return 0;
}

sub PrintConf()
{
	my $refHashSelf = shift;
	my ($strConfigFile, $refHashConf) = ($refHashSelf->{ConfFileName}, $refHashSelf->{Conf});
	print "---$strConfigFile---\n";
	foreach my $strSection (sort keys %{$refHashConf})
	{
		print "[$strSection]\n";
		for my $strKey (sort keys %{$refHashConf->{$strSection}})
		{
			print "\t$strKey=$refHashConf->{$strSection}{$strKey}\n";
		}
	}
	return 0;
}

sub GetConfName()
{
	my $refHashSelf = shift;
	return $refHashSelf->{ConfFileName};
}

sub GetConf()
{
	my $refHashSelf = shift;
	if (@_ == 0)
	{
		return $refHashSelf->{Conf};
	}
	elsif (@_ == 1)
	{
		return $refHashSelf->{Conf}{$_[0]};
	}
	elsif (@_ == 2)
	{
		return $refHashSelf->{Conf}{$_[0]}{$_[1]};
	}
	else
	{
		return undef;
	}
}

sub WriteConf()
{
	my $refHashSelf = shift;
	my ($strSection, $strKey, $strValue) = @_;
	my ($strConfigFile, $refHashConf) = ($refHashSelf->{ConfFileName}, $refHashSelf->{Conf});
	$refHashConf->{$strSection}{$strKey} = $strValue;
	
	my $nRet = open(CONFFILE, ">", $strConfigFile);
	if (!defined($nRet))
	{
		print "WriteConf: open file $strConfigFile fail\n";
		return -1;
	}
	
	foreach my $strSection (sort keys %{$refHashConf})
	{
		print CONFFILE "[$strSection]\n";
		for my $strKey (sort keys %{$refHashConf->{$strSection}})
		{
			print CONFFILE "$strKey=$refHashConf->{$strSection}{$strKey}\n";
		}
	}
	close(CONFFILE);
	return 0;
}

1;
