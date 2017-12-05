#!/usr/bin/perl
package MyFile;
use strict;
use warnings;

use Fcntl;
use Fcntl qw(F_GETFL F_SETFL O_NONBLOCK);

sub new()
{
	my $refHashSelf = {};
	shift;
	my ($strFileName, $strOpenMode, $nIfChomp) = @_;
	$refHashSelf->{FileName} = $strFileName;
	$refHashSelf->{OpenMode} = $strOpenMode;
	$refHashSelf->{IfChomp} = (defined($nIfChomp))?$nIfChomp:0;
	$refHashSelf->{Handle} = undef;
	$refHashSelf->{CurFlag} = 0;
	$refHashSelf->{LastCurFlag} = 0;
	bless ($refHashSelf);
	return $refHashSelf;
}

 sub Open()
 {
 	my $refHashSelf = shift;
 	my ($strFileName, $strOpenMode) = ($refHashSelf->{FileName}, $refHashSelf->{OpenMode});
 	local *MYFILE;
 	my $nRet = open(MYFILE, $strOpenMode, $strFileName);
 	if(!defined($nRet))
 	{
 		return -1;
 	}
 	else
 	{
 		$refHashSelf->{Handle} = *MYFILE;
 	}
 	return 0;
 }
 
  sub Close()
 {
 	my $refHashSelf = shift;
 	my $nRet = close($refHashSelf->{Handle});
 	return -1 if (!defined($nRet));
 	return 0;
 }
 
 sub ReadLine()
 {
 	my $refHashSelf = shift;
 	my $hFileHandle = $refHashSelf->{Handle};
 	my $strLine = <$hFileHandle>;
 	if ($refHashSelf->{IfChomp})
 	{
 		if ($strLine)
 		{
 			$strLine =~ s/\n$// if (!($strLine =~ s/\r\n$//));
 		}
 	}
 	return $strLine;
 }
 
 sub Write()
 {
 	my $refHashSelf = shift;
 	my ($strWriteString) = @_;
 	my $hFileHandle = $refHashSelf->{Handle};
 	print {$hFileHandle} $strWriteString; 	
 }
 
 sub AddFileFlags()
 {
 	my $refHashSelf = shift;
 	my ($nAddFlag) = @_;
 	my $hFileHandle = $refHashSelf->{Handle};
 	
 	my $nFlags = fcntl($hFileHandle, F_GETFL, 0);
 	return -1 if($nFlags < 0);
 	
 	$refHashSelf->{LastCurFlag} = $nFlags;
 	$nFlags = fcntl($hFileHandle, F_SETFL, $nFlags | $nAddFlag);
	return -1 if($nFlags < 0);
	
	$refHashSelf->{CurFlag} = $nFlags;
 	return 0;
 }
 
 sub DelFileFlags()
 {
 	my $refHashSelf = shift;
 	my ($nAddFlag) = @_;
 	my $hFileHandle = $refHashSelf->{Handle};
 	
 	my $nFlags = fcntl($hFileHandle, F_GETFL, 0);
	return -1 if($nFlags < 0);
	
	$refHashSelf->{LastCurFlag} = $nFlags;
 	$nFlags = fcntl($hFileHandle, F_SETFL, $nFlags & (~$nAddFlag));
	return -1 if($nFlags < 0);
	
	$refHashSelf->{CurFlag} = $nFlags;
 	return 0;
 }
 
1;