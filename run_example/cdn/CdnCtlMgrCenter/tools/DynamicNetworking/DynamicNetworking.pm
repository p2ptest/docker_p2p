#!/usr/bin/perl
package DynamicNetworking;
use strict;
use warnings;

BEGIN
{
	unshift @INC, 'mylib';
	#unshif会导致警告
}

use MySql;

my %s_hashCDNCluster;		    	#所有CDN集群信息
									#{
									#	clusterid => [level, max_parent,max_child,is_online,comment],
									#}       

my %s_hashClusterConnectivity;   	#集群亲密度
									#{
									#	clusterid =>	{ 
									#						clusterid => 'connectivity',
									#					},
									#} 

my %s_hashNetworkCluster;			#已经组网的集群
									#{
									#   level => [clusterid, clusterid...]
									#}

my %s_hashClusterParents;           #记录每个集群的父集群
									#{
									#	clusterid => [parent0,parent1...],
									#} 
									
my %s_hashClusterChildren;          #记录每个集群的子集群
									#{
									#	clusterid => [child0,child1...],
									#} 
									
my %hashLevelCluster;				#将健康集群按level存放
									#{
									#   level => [clusterid, clusterid...]
									#}

#加载数据库信息
sub LoadDB($)
{
	my $objDbConf = $_[0];
	
	my $strDbHost = $objDbConf->GetConf('DynamicNetworkDB', 'DbHost');
	my $strDbName = $objDbConf->GetConf('DynamicNetworkDB', 'DbName');
	my $strDbUser = $objDbConf->GetConf('DynamicNetworkDB', 'DbUser');
	my $strDbPasswd = $objDbConf->GetConf('DynamicNetworkDB', 'DbPasswd');
	
	#print "$strDbHost, $strDbName,$strDbUser,$strDbPasswd, \n";
	
	my $s_objDBClusterInfo = MySql::->new($strDbName, $strDbHost, $strDbUser, $strDbPasswd);
	my $nRet = $s_objDBClusterInfo->Connect();
	return -1 if ($nRet != 0);
		
	#读取集群信息
	my $strQuery = "select * from t_cluster_info";	
	$nRet = $s_objDBClusterInfo->Execute($strQuery);
	if ($nRet != 0)
	{
		print "LoadClusterInfo : load t_cluster_info fail\n";
		return -1;
	}
	
	while(my $refRow = $s_objDBClusterInfo->FetchrowHashRef())
	{
		$s_hashCDNCluster{$$refRow{clusterid}} =
			[$$refRow{level}, $$refRow{maxparents}, $$refRow{maxchild}];
			
		#print $$refRow{clusterid}, ' | ', $$refRow{level}, ' | ', $$refRow{maxparents},' | ', $$refRow{maxchild},"\n";
	}
	
	#读取亲密度信息
	$strQuery = "select * from t_connectivity";	
	$nRet = $s_objDBClusterInfo->Execute($strQuery);
	if ($nRet != 0)
	{
		print "LoadClusterInfo : load t_connectivity fail\n";
		return -1;
	}
	
	while(my @refRow = $s_objDBClusterInfo->FetchrowArray())
	{
		$s_hashClusterConnectivity{$refRow[0]}{$refRow[1]}= [$refRow[2], $refRow[3]];
		#print "$refRow[0] -> $refRow[1] : $refRow[2], $refRow[3]\n";
	}
	
	#从监控数据库中读取集群在线状态及集群描述
	$strDbHost = $objDbConf->GetConf('MonitorCenterDB', 'DbHost');
	$strDbName = $objDbConf->GetConf('MonitorCenterDB', 'DbName');
	$strDbUser = $objDbConf->GetConf('MonitorCenterDB', 'DbUser');
	$strDbPasswd = $objDbConf->GetConf('MonitorCenterDB', 'DbPasswd');

	my $s_objDBClusterConf = MySql::->new($strDbName, $strDbHost, $strDbUser, $strDbPasswd);
	$nRet = $s_objDBClusterConf->Connect();
	return -1 if ($nRet != 0);
		
	#读取集群信息
	$strQuery = "select * from t_cluster_conf";	
	$nRet = $s_objDBClusterConf->Execute($strQuery);
	if ($nRet != 0)
	{
		print "LoadClusterInfo : load t_cluster_conf fail\n";
		print "$strDbHost, $strDbName,$strDbUser,$strDbPasswd, \n";
		return -1;
	}
	
	while(my $refRow = $s_objDBClusterConf->FetchrowHashRef())
	{
		if( defined( @{$s_hashCDNCluster{$$refRow{ClusterId}}} ) )
		{
			push @{$s_hashCDNCluster{$$refRow{ClusterId}}},($$refRow{status}, $$refRow{Comment});	
		}
#		print " $$refRow{ClusterId} @{$s_hashCDNCluster{$$refRow{ClusterId}}}\n";
	}	
	
	return 0;
}

sub CheckIfNetworkChange($)
{
	my $nIfChange = 0;	

	my $objDbConf = $_[0];
	
	my $strDbHost = $objDbConf->GetConf('CdnctlmgrCenterDB', 'DbHost');
	my $strDbName = $objDbConf->GetConf('CdnctlmgrCenterDB', 'DbName');
	my $strDbUser = $objDbConf->GetConf('CdnctlmgrCenterDB', 'DbUser');
	my $strDbPasswd = $objDbConf->GetConf('CdnctlmgrCenterDB', 'DbPasswd');

	my $s_objDBClusterRelation = MySql::->new($strDbName, $strDbHost, $strDbUser, $strDbPasswd);
	my $nRet = $s_objDBClusterRelation->Connect();
	return -1 if ($nRet != 0);
	
	my $strSql = "select * from CdnClusterRelation_tbl;";	
	$nRet = $s_objDBClusterRelation->Execute($strSql);
	if ($nRet != 0)
	{
		print "CheckIfNetworkChange : select from CdnClusterRelation_tbl fail\n";		
		return -1;
	}

	my %hashCurClusterParents;
	while (my $refRow = $s_objDBClusterRelation->FetchrowHashRef())
	{
		my $nClusterId = $refRow->{ClusterId};
		my $nParentClusterId = $refRow->{ParentClusterId};
		if (!defined($s_hashClusterConnectivity{$nClusterId}{$nParentClusterId}))
		{
			$nIfChange = 1;
			return $nIfChange;
		}
		push @{$hashCurClusterParents{$nClusterId}}, $nParentClusterId;
	}

	my @arrKey = sort (keys %hashCurClusterParents);
	my @arrNewKey =  sort (keys %s_hashClusterParents);
	if (scalar(@arrKey) != scalar(@arrNewKey))
	{
		$nIfChange = 1;
		return $nIfChange;
	}
	my $nArrLen = @arrKey;	
	for (my $nA = 0; $nA < $nArrLen; $nA ++)
	{
		if ($arrKey[$nA] != $arrNewKey[$nA])	
		{
			$nIfChange = 1;				
			return $nIfChange;
		}
		my @arrTmp1 = sort @{$hashCurClusterParents{$arrKey[$nA]}};
		my @arrTmp2 = sort @{$s_hashClusterParents{$arrNewKey[$nA]}};
		if (scalar(@arrTmp1) != scalar(@arrTmp2))
		{
			$nIfChange = 1;
			return $nIfChange;
		}
		if (join(" ", @arrTmp1) ne join(" ", @arrTmp2))
		{
			$nIfChange = 1;
			return $nIfChange;
		}
	}
	return $nIfChange;			
}

sub WriteDB($)
{
	my $objDbConf = $_[0];
	
	my $strDbHost = $objDbConf->GetConf('CdnctlmgrCenterDB', 'DbHost');
	my $strDbName = $objDbConf->GetConf('CdnctlmgrCenterDB', 'DbName');
	my $strDbUser = $objDbConf->GetConf('CdnctlmgrCenterDB', 'DbUser');
	my $strDbPasswd = $objDbConf->GetConf('CdnctlmgrCenterDB', 'DbPasswd');

	my $s_objDBClusterRelation = MySql::->new($strDbName, $strDbHost, $strDbUser, $strDbPasswd);
	my $nRet = $s_objDBClusterRelation->Connect();
	return -1 if ($nRet != 0);
	
	#创建t_relationbackup表，如果它不存在
	my $strSql = "create table if not exists t_relationbackup(
	              ClusterId int(11), Level int(11) not null, ParentClusterId int(11), IfUserPublicIp tinyint(3) not null default '1', Comment varchar(512), 
	              Primary Key(ClusterId,ParentClusterId))";
	              
	$nRet = $s_objDBClusterRelation->Execute($strSql);
	if ($nRet != 0)
	{
		print "WriteDB : create table t_relationbackup fail\n";
		return -1;
	}
	
	my $strQuery = "delete from t_relationbackup";	
	$nRet = $s_objDBClusterRelation->Execute($strQuery);
	if ($nRet != 0)
	{
		print "WriteDB : delete from t_relationbackup fail\n";
		return -1;
	}
	
	foreach my $nId(sort keys %s_hashClusterParents)
	{		
		foreach my $nParentId(@{$s_hashClusterParents{$nId}})
		{
			my $comment = ($s_hashCDNCluster{$nId}[4]).'->'.($s_hashCDNCluster{$nParentId}[4]);
			my $nIfPublic = $s_hashClusterConnectivity{$nId}{$nParentId}[1];
			$strQuery = "insert into t_relationbackup values($nId, $s_hashCDNCluster{$nId}[0], $nParentId, $nIfPublic, '$comment');";	
			$nRet = $s_objDBClusterRelation->Execute($strQuery);
			if ($nRet != 0)
			{
				print "WriteDB : insert into t_relationbackup failed!\n";
				return -1;
			}
		}
	}
	
	$strQuery = "alter table t_relationbackup rename to t_tmp;";
	$nRet = $s_objDBClusterRelation->Execute($strQuery);
	if ($nRet != 0)
	{
		print "WriteDB : alter table t_relationbackup rename to t_tmp failed!\n";
		return -1;
	}

	$strQuery = "alter table CdnClusterRelation_tbl rename to t_relationbackup;";
	$nRet = $s_objDBClusterRelation->Execute($strQuery);
	if ($nRet != 0)
	{
		print "WriteDB : alter table CdnClusterRelation_tbl rename to t_relationbackup failed!\n";
		return -1;
	}
	
	$strQuery = "alter table t_tmp rename to CdnClusterRelation_tbl;";
	$nRet = $s_objDBClusterRelation->Execute($strQuery);
	if ($nRet != 0)
	{
		print "WriteDB : alter table t_tmp rename to CdnClusterRelation_tbl failed!\n";
		return -1;
	}
}

#$nId1和$nId2的亲密度，
sub Connectivity($$)
{
	my ($nId1, $nId2) = @_;
	
	if(!defined($s_hashClusterConnectivity{$nId1}{$nId2}))
	{
		return 0;
	}
	return $s_hashClusterConnectivity{$nId1}{$nId2}[0];
}

sub InitLevelCluster()
{
	foreach my $nClusterId(keys %s_hashCDNCluster) 
	{
		my $nLevel = $s_hashCDNCluster{$nClusterId}[0];
		if( defined($s_hashCDNCluster{$nClusterId}[3]) && (1 == $s_hashCDNCluster{$nClusterId}[3]) )
		{
			push @{$hashLevelCluster{$nLevel}}, $nClusterId;	
		}		
	}
	
	return 0;
}

#找出父节点候选集
sub FindParentArray($)
{
	my $nLevel = $_[0];

	my @arrayTmpParents;					
	my $nParentLevel = $nLevel - 1;
	while( $nParentLevel >= 0 )
	{
		last if( defined(@{$s_hashNetworkCluster{$nParentLevel}}) && @{$s_hashNetworkCluster{$nParentLevel}} > 0 );
		
		--$nParentLevel;					
	}
	
	push @arrayTmpParents, @{$s_hashNetworkCluster{$nParentLevel}};
	(defined(@{$s_hashNetworkCluster{$nLevel}})) && push(@arrayTmpParents, @{$s_hashNetworkCluster{$nLevel}});
	
	@arrayTmpParents = sort{$a <=> $b} @arrayTmpParents;
	
	#print "P( @arrayTmpParents )\n";
	
	my @arrayFullChildParents;
	my @arrayUnFullChildParents;
	
	foreach my $nTmpParentId(@arrayTmpParents)
	{
		if( !defined(@{$s_hashClusterChildren{$nTmpParentId}}) || (@{$s_hashClusterChildren{$nTmpParentId}} < $s_hashCDNCluster{$nTmpParentId}->[2]) )
		{
			push @arrayUnFullChildParents, $nTmpParentId;
		}
		else
		{
			push @arrayFullChildParents, $nTmpParentId;
		}
	}
	@arrayUnFullChildParents = sort{$a <=> $b} @arrayUnFullChildParents;
	@arrayFullChildParents = sort{$a <=> $b} @arrayFullChildParents;
	
	return (\@arrayUnFullChildParents, \@arrayFullChildParents);
}

sub FindInUnFullParents(\@ $ $ $)
{
	my @arrayUnFullChildParents = @{$_[0]};
	my $nLevel = $_[1];
	
	my $max_connectivity = 0;
	my %hashParentAndChild;
	my $nRetPId =  0;
	foreach my $nPId( @arrayUnFullChildParents )
	{
		foreach my $nHId( @{$hashLevelCluster{$nLevel}} )
		{
			my $nConnectivity = &Connectivity($nHId, $nPId);
			next if( 0 == $nConnectivity);

			if( $max_connectivity < $nConnectivity )
			{
				$max_connectivity = $nConnectivity;
				
				%hashParentAndChild = ();
				$hashParentAndChild{$nPId} = $nHId;
				$nRetPId = $nPId;
			}
			elsif( $max_connectivity == $nConnectivity )
			{
				next if(defined($hashParentAndChild{$nPId}));
				$hashParentAndChild{$nPId} = $nHId;							
				$nRetPId = $nPId;
			}
		}
	}
	
	my $nChildCounts = 10000;
	foreach my $nParent( sort{$a <=> $b} keys(%hashParentAndChild) )
	{
		my $nCounts = @{$s_hashClusterChildren{$nParent}};
		if($nCounts < $nChildCounts)
		{
			$_[2] = $nParent;
			$_[3] = $hashParentAndChild{$nParent};
			$nChildCounts = $nCounts;
		}
	}				
	
	return $max_connectivity;
}

sub FindInFullParents(\@ $ $ $)
{
	my @arrayFullChildParents = @{$_[0]};
	my $nLevel = $_[1];
		
	my $nChildCounts = 10000;
	my %hashParentAndChild;
	my $nFound = 0;
	foreach my $nPId( @arrayFullChildParents )
	{
		foreach my $nHId( @{$hashLevelCluster{$nLevel}} )
		{
			my $nConnectivity = &Connectivity($nHId, $nPId);
			if( 0 < $nConnectivity)
			{
				if(defined($hashParentAndChild{$nPId}))
				{
					my $nCId = $hashParentAndChild{$nPId};
					if( $nConnectivity > &Connectivity($nCId, $nPId))
					{
						$hashParentAndChild{$nPId} = $nHId;						
					}
				}
				else
				{
					$hashParentAndChild{$nPId} = $nHId;					
				}												
			}
		}
	}					

	foreach my $nPId( sort{$a <=> $b} keys(%hashParentAndChild) ) 
	{
#		print "($nPId,  $hashParentAndChild{$nPId}) \n";
		if( @{$s_hashClusterChildren{$nPId}} < $nChildCounts )   
		{
			$nChildCounts = @{$s_hashClusterChildren{$nPId}} ;							
			$_[2]	= $nPId;
			$_[3]   = $hashParentAndChild{$nPId};
			$nFound = 1;
		}
	}
	
	return ($nFound ? &Connectivity($_[2],$_[3]) : 0 );
}

sub DynamicNetwork()
{
	&InitLevelCluster();
	
	#逐层建立
	foreach my $nLevel( sort {$a <=> $b} keys(%hashLevelCluster) )
	{		
		print "\n\n----------------------  level ", $nLevel, "  ---------------------\n\n";
		#片源节点不考虑父节点
		if( 0 == $nLevel)
		{
			push @{$s_hashNetworkCluster{0}}, (sort {$a <=> $b} @{$hashLevelCluster{$nLevel}});
			foreach my $cid(@{$s_hashNetworkCluster{0}})
			{
				print "  $s_hashCDNCluster{$cid}[4]  ";
			}
			#print "   @{$s_hashNetworkCluster{0}} \n";
		}
		else
		{
			@{$hashLevelCluster{$nLevel}} = sort{$a <=> $b} @{$hashLevelCluster{$nLevel}};
			while(@{$hashLevelCluster{$nLevel}})
			{
				#找出父节点候选集
				my ($array1, $array2) = &FindParentArray($nLevel);
				my @arrayUnFullChildParents = @{$array1};
				my @arrayFullChildParents = @{$array2};
				
				my $nConnectivity = 0;
				my $nMyId;
				my $nParentId;	#第一个父节点

				if( $nConnectivity = &FindInUnFullParents(\@arrayUnFullChildParents, $nLevel, $nParentId,$nMyId) )
				{
					@arrayUnFullChildParents = grep { $_ != $nParentId} @arrayUnFullChildParents;
				}
				elsif( $nConnectivity = &FindInFullParents(\@arrayFullChildParents, $nLevel, $nParentId,$nMyId) )
				{
					@arrayFullChildParents = grep { $_ != $nParentId} @arrayFullChildParents;
				}
				else
				{					
					foreach my $cid(@{$hashLevelCluster{$nLevel}})
					{						
						print "\n\n", $s_hashCDNCluster{$cid}[4], "($cid) can not find parents!\n";
					}
					#print "\n\n@{$hashLevelCluster{$nLevel}} can not find parents!\n";
					return -1;
				}			
								
				@{$hashLevelCluster{$nLevel}} = grep { $_ != $nMyId }  @{$hashLevelCluster{$nLevel}};
				push @{$s_hashNetworkCluster{$nLevel}}, $nMyId;
				push @{$s_hashClusterParents{$nMyId}},$nParentId;
				push @{$s_hashClusterChildren{$nParentId}},$nMyId;
				
#				print $nParentId, '-->', $nMyId, "\n";
				
				#为$nMyId选择其他父节点				
				while(@{$s_hashClusterParents{$nMyId}} < $s_hashCDNCluster{$nMyId}->[1])
				{
					my $max_conn = 0;
					my $nMyParent;
					my $bFound = 0;
					foreach my $nTmpParent(@arrayUnFullChildParents)
					{
						#每个节点的父节点要么只能是片源，要么只能是非片源节点
						my $nFirstParentLevel = $s_hashCDNCluster{$nParentId}[0];
						my $nTmpParentLevel = $s_hashCDNCluster{$nTmpParent}[0];
						next if( (0 == $nFirstParentLevel) && (0 != $nTmpParentLevel));
						next if( (0 != $nFirstParentLevel) && (0 == $nTmpParentLevel));
						
						my $nConn = &Connectivity($nMyId, $nTmpParent);
						if( 100 == $nConnectivity)	#如果与第一个父节点的亲密度为100，则其他父节点亲密度必须也为100
						{
							next if($nConn < 100);
			
							$nMyParent = $nTmpParent;
							$bFound = 1; 
							last;
						}
						elsif($max_conn < $nConn)
						{
							$max_conn = $nConn;
							$nMyParent = $nTmpParent;
							$bFound = 1;
						}
					}
					
					if(1 == $bFound)
					{
						push @{$s_hashClusterParents{$nMyId}},$nMyParent;
						push @{$s_hashClusterChildren{$nMyParent}},$nMyId;
						
#						print $nMyParent, '-->', $nMyId, "\n";
						
						@arrayUnFullChildParents = grep { $_ != $nMyParent} @arrayUnFullChildParents;
					}
					else
					{
						last;
					}
				}
				my @arrayParentInfo;
				foreach my $cid(@{$s_hashClusterParents{$nMyId}})
				{
					push @arrayParentInfo, $s_hashCDNCluster{$cid}[4];
				}
				print $s_hashCDNCluster{$nMyId}->[4], '{', "@arrayParentInfo", '}, ';
			}		
		}
	}
	
	print "\n -----------------------------  end  ----------------------- \n";
	return 0;
}

1;
