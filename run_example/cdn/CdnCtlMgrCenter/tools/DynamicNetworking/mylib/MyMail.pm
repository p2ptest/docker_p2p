#!/usr/bin/perl
package MyMail;
use strict;
use warnings;

sub new()
{
	my $refHashSelf = {};
	shift;
	my ($strMailUser) = @_;
	$refHashSelf->{User} = $strMailUser;
	$refHashSelf->{DstMailAccount} = {};
	bless ($refHashSelf);
	return $refHashSelf;
}

sub SetDstMailAccount()
{
	my $refHashSelf = shift;
	my ($refMailAccount) = @_;
	foreach my $strMailDes (sort keys %{$refMailAccount})
	{
		$refHashSelf->{DstMailAccount}{$strMailDes} = $refMailAccount->{$strMailDes};
		#print "$strMailDes ".$refMailAccount->{$strMailDes}."\n";
	}
	return 0;
}

sub Mail()
{
	my $refHashSelf = shift;
	my ($strSubject ,$strMsgString) = @_;
	my $strUser = $refHashSelf->{User};
	my $nRet = 0;
	foreach my $strMailDes (keys %{$refHashSelf->{DstMailAccount}})
	{
		#print "$strUser $strMailDes ".$refHashSelf->{DstMailAccount}{$strMailDes}."\n";
		`su $strUser -c "echo -e '$strMsgString\n' | mail -s '$strSubject' $refHashSelf->{DstMailAccount}{$strMailDes}"`;
		my $nRet = $? >> 8;
		if ($nRet != 0)
		{
			print "Mail To $strMailDes fail\n";
			$nRet ++;
		}
	}
	return $nRet;
}

#MailWithAttachment need uuencode
#yum install sharutils -y
#usage:MailWithAttachment('Subject', 'Message', ['file1.txt' ,'file2.txt'] );
sub MailWithAttachment()
{
	my $refHashSelf = shift;
	my ($strSubject ,$strMsgString, $refArrAttachFiles) = @_;
	
	my $nFileNum = @{$refArrAttachFiles};
	my $strCmdMakeAttachFiles = "(";
	for (my $nA = 0; $nA < $nFileNum; ++ $nA)
	{
		$strCmdMakeAttachFiles .= "uuencode $refArrAttachFiles->[$nA] $refArrAttachFiles->[$nA];";	
	}
	$strCmdMakeAttachFiles .= ")";
	
	my $strUUENFile = ".tmp.uuen";
	#`rm $strUUENFile`;
	`echo $strMsgString >$strUUENFile`;
	`$strCmdMakeAttachFiles >>$strUUENFile`;
	
	my $strUser = $refHashSelf->{User};
	my $nRet = 0;
	foreach my $strMailDes (keys %{$refHashSelf->{DstMailAccount}})
	{
		#print "$strUser $strMailDes ".$refHashSelf->{DstMailAccount}{$strMailDes}."\n";
		`su $strUser -c "cat $strUUENFile | mail -s '$strSubject' $refHashSelf->{DstMailAccount}{$strMailDes}"`;
		my $nRet = $? >> 8;
		if ($nRet != 0)
		{
			print "Mail To $strMailDes fail\n";
			$nRet ++;
		}
	}
	`rm -rf $strUUENFile`;
	return $nRet;
}

1;
