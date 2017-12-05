#!/usr/bin/perl
package MyLog;
use strict;
use warnings;

use MyTime;

sub new()
{
	my $refHashSelf = {};
	shift;
	my ($strLogFileName, $nOpenFlag, $nMaxLogSize) = @_;
	$refHashSelf->{LogFileName} = $strLogFileName;
	$refHashSelf->{OpenFlag} = (defined($nOpenFlag))?$nOpenFlag:1;
	my $nDefaultMaxLogSize = 10 * 1024 * 1024;
	$refHashSelf->{MaxLogSize} = (defined($nMaxLogSize))?$nMaxLogSize:$nDefaultMaxLogSize;
	bless ($refHashSelf);
	return $refHashSelf;
}

sub SetLogStatus()
{
	my $refHashSelf = shift;
	my ($nOpenFlag) = @_;
	$refHashSelf->{OpenFlag} = $nOpenFlag;
	return 0;
}

sub GetLogFileName()
{
	my $refHashSelf = shift;
	return $refHashSelf->{LogFileName};
}

sub WriteLog()
{
	my $refHashSelf = shift;
	my ($strLogString) = @_;

	if($refHashSelf->{OpenFlag})
	{
		my @arrFileStat = stat($refHashSelf->{LogFileName});
		if ($arrFileStat[7] >= $refHashSelf->{MaxLogSize})
		{
			`mv $refHashSelf->{LogFileName} .$refHashSelf->{LogFileName}.bak`;
		}
		open(LOGFILE, ">>", $refHashSelf->{LogFileName});
		print LOGFILE MyTime::GetLocalStringTime.': '.$strLogString."\n";
		close(LOGFILE);
	}
	return 0;
}

1;