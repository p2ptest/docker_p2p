#!/usr/bin/perl
package MySql;
use strict;
use warnings;
use DBI;

sub new()
{
	my $refHashSelf = {};
	shift;
	my ($strDBName, $strDBHost, $strDBUser, $strDBPassword) = @_;
	$refHashSelf->{DBName} = $strDBName;
	$refHashSelf->{DBHost} = $strDBHost;
	$refHashSelf->{DBUser} = $strDBUser;
	$refHashSelf->{DBPassword} = $strDBPassword;
	
	$refHashSelf->{Query} = "";
	$refHashSelf->{DbHandle} = undef;
	$refHashSelf->{RetHandle} = undef;
	bless ($refHashSelf);
	return $refHashSelf;
}

sub Connect()
{
	my $refHashSelf = shift;
	
	my $strDBName = $refHashSelf->{DBName};
	my $strDBHost = $refHashSelf->{DBHost};
	my $strDBUser = $refHashSelf->{DBUser};
	my $strDBPassword = $refHashSelf->{DBPassword};

	eval
	{
		$refHashSelf->{DbHandle} = DBI->connect("DBI:mysql:database=$strDBName;$strDBHost",$strDBUser,$strDBPassword, {'RaiseError' => 1, 'PrintError' => 0});
	};
	if($@)
	{
		print "Connect DB $strDBHost $strDBName Fail : $DBI::errstr\n";
		return -1;
	}
	return 0;
}

sub DisConnect()
{
	my $refHashSelf = shift;
	my $strDBHost = $refHashSelf->{DBHost};
	eval
	{
		$refHashSelf->{DbHandle}->disconnect();	
	};
	if($@)
	{
		print "DisConnect DB $strDBHost Fail : $DBI::errstr\n";
		return -1;
	}
	else 
	{
		return 0;
	}	
}

sub SetQuery()
{
	my $refHashSelf = shift;
	if (@_ != 1)
	{
		print "SetQuery Fail\n";
		return -1;
	}
	$refHashSelf->{Query} = $_[0];
	return 0;	
}

sub PrintQuery()
{
	my $refHashSelf = shift;
	print "$refHashSelf->{Query}\n";
	return 0;
}

sub Escape()
{
	my $refHashSelf = shift;
	my $strQuery = $_[0];
	eval
	{
		$strQuery = $refHashSelf->{DbHandle}->quote($strQuery);
	};
	if($@)
	{
		print "Escape fail : $DBI::errstr\n";
		return undef;
	}
	return $strQuery;
}

sub Execute()
{
	my $refHashSelf = shift;
	my $nNum = @_;
	if (@_ == 1)
	{
		$refHashSelf->{Query} = $_[0];	
	}
	my $strQuery = $refHashSelf->{Query};
	
	my $sthMysql;
	eval
	{
		$sthMysql = $refHashSelf->{DbHandle}->prepare($strQuery);
		$sthMysql->execute();
	};
	if($@)
	{
		print "Execute : $DBI::errstr\n";
		$refHashSelf->{RetHandle} = undef;
		return -1;
	}
	$refHashSelf->{RetHandle} = $sthMysql;
	return 0;
}

sub FetchrowArray()
{
	my $refHashSelf = shift;
	my @arrFetch  = ();
	eval
	{
		@arrFetch = $refHashSelf->{RetHandle}->fetchrow_array();
	};
	if($@)
	{
		print "FetchrowArray Fail :  $DBI::errstr\n";
		return ();
	}
	return @arrFetch;
}

sub FetchrowArrayRef()
{
	my $refHashSelf = shift;
	my $refArrFetch;
	eval
	{
		$refArrFetch = $refHashSelf->{RetHandle}->fetchrow_arrayref();
	};
	if($@)
	{
		print "FetchrowArrayRef Fail : $DBI::errstr\n";
		return undef;
	}
	return $refArrFetch;
}

sub FetchrowHashRef()
{
	my $refHashSelf = shift;
	my $refHashFetch;
	eval
	{
		$refHashFetch =  $refHashSelf->{RetHandle}->fetchrow_hashref();
	};
	if($@)
	{
		print "FetchrowHashRef Fail: $DBI::errstr\n";
		return undef;
	}
	return $refHashFetch;
}

sub CreateDatabase()
{
	my $refHashSelf = shift;
	my $strDbName = $_[0];
	my $strMySql = "create database if not exists ".$strDbName;
	return $refHashSelf->Execute($strMySql);
}

sub CreateTableLike()
{	
	my $refHashSelf = shift;
	my ($strTbName, $strTbLikeName) = @_;;
	my $strMySql = "create table if not exists $strTbName like $strTbLikeName";
	return $refHashSelf->Execute($strMySql);
}

sub GetDbName()
{
	my $refHashSelf = shift;
	return $refHashSelf->{DBName};
}

1;
