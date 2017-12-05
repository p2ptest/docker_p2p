#!/usr/bin/perl
package MyThread;
use strict;
use warnings;

use threads;
use threads::shared;
use Thread::Semaphore; 
use Thread::Queue;

sub new()
{
	my $refHashSelf = {};
	shift;
	$refHashSelf->{Handle} = {};
	$refHashSelf->{MsgQueue} = Thread::Queue->new();
	bless ($refHashSelf);
	return $refHashSelf;
}

sub Start()
{
	my $refHashSelf = shift;
	my $refThreadEntity = shift;
	my @arrThreadArgs = @_;
	$refHashSelf->{Handle} = threads->create($refThreadEntity, @arrThreadArgs, $refHashSelf);
	if (!defined($refHashSelf))
	{
		return -1;
	}
	return 0;
}

sub Join()
{
	my $refHashSelf = shift;
	return $refHashSelf->{Handle}->join();
}

sub Detach()
{
	my $refHashSelf = shift;
	return $refHashSelf->{Handle}->detach();
}

sub PutMsgToThread()
{
	my $refHashSelf = shift;
	return $refHashSelf->{MsgQueue}->enqueue($_[1]);
}

sub GetMsg()
{
	my $refHashSelf = shift;
	return $refHashSelf->{MsgQueue}->dequeue();
}

1;