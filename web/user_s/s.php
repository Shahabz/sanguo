<?php
require_once '../db.inc.php';
//error_reporting(0); 

// 找一个推荐服务器并且人最少的
function getserver( $db, $userid, $channelId, $sys, $version, &$serverid, &$host, &$port, &$close, &$force_android_ver, &$force_ios_ver )
{
	$sql="select id, ip, port, close, force_android_ver, force_ios_ver from `gate_list` where new=1 and tempnum > 0 limit 1;";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{ 
		$serverid = $sth[0];
		$host = $sth[1];
		$port = $sth[2];
		$close = $sth[3];
		$force_android_ver = $sth[4];
		$force_ios_ver = $sth[5];
		$db->free_result($query);
		$db->query("UPDATE user SET serverid='$serverid' WHERE id='$userid'");
		$db->query("UPDATE gate_list SET usernum=usernum+1,tempnum=tempnum-1 WHERE id='$serverid'");
	}
	else
	{
		$db->query("UPDATE gate_list SET tempnum=weight where new = 1;");
		$sql="select id, ip, port, close, force_android_ver, force_ios_ver from `gate_list` where new=1 and tempnum > 0 limit 1;";
		$query = $db->query($sql);
		if($sth = $db->fetch_row($query))
		{ 
			$serverid = $sth[0];
			$host = $sth[1];
			$port = $sth[2];
			$close = $sth[3];
			$force_android_ver = $sth[4];
			$force_ios_ver = $sth[5];
			$db->free_result($query);
			$db->query("UPDATE user SET serverid='$serverid' WHERE id='$userid'");
			$db->query("UPDATE gate_list SET usernum=usernum+1,tempnum=tempnum-1 WHERE id='$serverid'");
		}
		else
		{
			$sql="select id,ip,port,close,force_android_ver,force_ios_ver,weight,tempnum from `gate_list` where new=1 order by usernum asc limit 1;";
			$query = $db->query($sql);
			if($sth = $db->fetch_row($query))
			{
				$serverid = $sth[0];
				$host = $sth[1];
				$port = $sth[2];
				$close = $sth[3];
				$force_android_ver = $sth[4];
				$force_ios_ver = $sth[5];
				$db->free_result($query);
				$db->query("UPDATE user SET serverid='$serverid' WHERE id='$userid'");
				$db->query("UPDATE gate_list SET usernum=usernum+1 WHERE id='$serverid'");
			}
		}
	}
	
	
	/*$sql="select id,ip,port,close,force_android_ver,force_ios_ver,weight,tempnum from `gate_list` where new=1 order by usernum asc limit 1;";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$serverid = $sth[0];
		$host = $sth[1];
		$port = $sth[2];
		$close = $sth[3];
		$force_android_ver = $sth[4];
		$force_ios_ver = $sth[5];
		$db->free_result($query);
		$db->query("UPDATE user SET serverid='$serverid' WHERE id='$userid'");
		$db->query("UPDATE gate_list SET usernum=usernum+1 WHERE id='$serverid'");
	}*/
}

// 根据IP地址获取所在国家
function get_ipcountry( $db, $clientip )
{
	$ipcountry = "";
	$sql="select country from ipdb where inet_aton('$clientip') between inet_aton(`ip_start`) and inet_aton(`ip_end`)";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$ipcountry = $sth[0];
	}
	$db->free_result($query);
	return $ipcountry;
}

// 更新IP地址获取的国家
function update_ipcountry( $db, $userid, $ipcountry )
{
	$sql="update user set ipcountry='$ipcountry' where id='$userid'";
	$query = $db->query($sql);
}

function access( $v1, $v2, $v3, $v4, $v5, $lang, $country, $channelId, $sys, $version, $clientip, $db )
{
	//wlogaccess( $db, $v1, "access begin" );
	
	$userid = 0;
	$serverid = 0;
	$host = 0;
	$port = 0;
	$close = 0;
	
	$force_android_ver = 0;
	$force_ios_ver = 0;
	$ipcountry = "";
	
	$result = -1;
	$uid = mysql_real_escape_string($v1);
	$sign = mysql_real_escape_string($v2);
	$timestamp = $v3;
	
	// 查找这个用户
	$sql="select id, serverid, ipcountry from `user` where name='$uid'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{ // 找到了这个用户
		$userid = $sth[0];
		$serverid = $sth[1];
		$ipcountry = $sth[2];
		$db->free_result($query);
		
		// 查找这个用户所在服务器
		$sql="select ip, port, close, force_android_ver, force_ios_ver from `gate_list` where id='$serverid'";
		$query = $db->query($sql);
		if($sth = $db->fetch_row($query))
		{
			$host = $sth[0];
			$port = $sth[1];
			$close = $sth[2];
			$force_android_ver = $sth[3];
			$force_ios_ver = $sth[4];
			$db->free_result($query);
		}
		else
		{ // 没查到，分配一个
			getserver( $db, $userid, $channelId, $sys, $version, $serverid, $host, $port, $close, $force_android_ver, $force_ios_ver );
			wlogaccess( $db, $uid, "serverid:".$serverid." host:".$host." port:".$port." close:".$close );
		}
		
		// 如果没有IP地址对应国家，那么获取并更新
		if ( $ipcountry == "" )
		{
				$ipcountry = get_ipcountry( $db, $clientip );
				update_ipcountry( $db, $userid, $ipcountry );
		}
		
	}
	else
	{ // 没找到这个用户，就插入一条
		
		// 获取IP地址所在国家
		$ipcountry = get_ipcountry( $db, $clientip );
		
		wlogaccess( $db, $v1, "access new ip:$clientip country:$ipcountry" );
		$thistime = time();	
		$sql="insert into user ( name, passwd, activeflag, serverid, lang, country, create_time, regip, channelid, sys, ipcountry ) values( '$uid', '', '1', '$serverid', '$lang', '$country', '$thistime', '$clientip', '$channelId', '$sys', '$ipcountry') ";
		if( !($db->query($sql) ) )
		{
			wlogaccess( $db, $uid, "access insert error:".$sql );
			$host = 0;
			$port = 0;
		}
		else
		{
			// 分配一个服务器
			$userid = mysql_insert_id();
			getserver( $db, $userid, $channelId, $sys, $version, $serverid, $host, $port, $close, $force_android_ver, $force_ios_ver );
			wlogaccess( $db, $uid, "serverid:".$serverid." host:".$host." port:".$port." close:".$close );
		}
		
	}

	// 白名单
	if ( admincheck( $uid ) == 1 )
	{
		$close = 0;
	}
	return "\"s\":\"$serverid\", \"h\":\"$host\" , \"p\":\"$port\" , \"close\":\"$close\", \"force_android_ver\":\"$force_android_ver\", \"force_ios_ver\":\"$force_ios_ver\", \"ipcountry\":\"$ipcountry\"";
}

// 选择服务器
function select( $v1, $v2, $db )
{
	wlogaccess( $db, $v1, "select begin uid:".$v1." serverid:".$v2 );
	
	$userid = 0;
	$host = 0;
	$port = 0;
	$close = 0;

	$result = -1;
	$uid = mysql_real_escape_string($v1);
	$serverid = mysql_real_escape_string($v2);
	
	if ( $uid == "" )
	{
		// 查找这个用户选择的新服务器
		$sql="select ip, port, close from `gate_list` where id='$serverid'";
		$query = $db->query($sql);
		if($sth = $db->fetch_row($query))
		{
			$host = $sth[0];
			$port = $sth[1];
			$close = $sth[2];
			$db->free_result($query);
			if ( $close == 1 )
			{
				$serverid = 0;
			}
		}
		else
		{ 
			// 没查到
			$db->free_result($query);
			$serverid = 0;
		}
	}
	else
	{
		// 查找这个用户
		$sql="select id from `user` where name='$uid'";
		$query = $db->query($sql);
		if($sth = $db->fetch_row($query))
		{ // 找到了这个用户
			$userid = $sth[0];
			$db->free_result($query);
			
			// 查找这个用户选择的新服务器
			$sql="select ip, port, close from `gate_list` where id='$serverid'";
			$query = $db->query($sql);
			if($sth = $db->fetch_row($query))
			{
				$host = $sth[0];
				$port = $sth[1];
				$close = $sth[2];
				$db->free_result($query);
				
				// 白名单
				if ( admincheck( $uid ) == 1 )
				{
					$close = 0;
				}
	
				if ( $close == 0 )
				{
					$db->query("UPDATE user SET serverid='$serverid' WHERE id='$userid'");
				}
				else
				{
					$serverid = 0;
				}
			}
			else
			{ 
				// 没查到
				$db->free_result($query);
				$serverid = 0;
			}
			
		}
		else
		{
			$serverid = 0;
		}
	}

	return "\"s\":\"$serverid\", \"h\":\"$host\" , \"p\":\"$port\" , \"close\":\"$close\"";
}

// 入口
if (!get_magic_quotes_gpc())
{
	$command=addslashes(@$_GET['c']);
	$v1=addslashes(@$_GET['v1']);
	$v2=addslashes(@$_GET['v2']);
	$v3=addslashes(@$_GET['v3']);
	$v4=addslashes(@$_GET['v4']);
	$v5=addslashes(@$_GET['v5']);
	$v6=addslashes(@$_GET['v6']);
	$lang=addslashes(@$_GET['lang']);
	$country=addslashes(@$_GET['country']);
	$channelId=addslashes(@$_GET['channelId']);
	$sys=addslashes(@$_GET['sys']);
	$version=addslashes(@$_GET['version']);
	$platid=addslashes(@$_GET['platid']);
	$clientip = $_SERVER["REMOTE_ADDR"];
}
else
{
	$command=@$_GET['c'];
	$v1=@$_GET['v1'];
	$v2=@$_GET['v2'];
	$v3=@$_GET['v3'];
	$v4=@$_GET['v4'];
	$v5=@$_GET['v5'];
	$v6=@$_GET['v6'];
	$lang=@$_GET['lang'];
	$country=@$_GET['country'];
	$channelId=@$_GET['channelId'];
	$sys=@$_GET['sys'];
	$version=@$_GET['version'];
	$platid=@$_GET['platid'];
	$clientip = $_SERVER["REMOTE_ADDR"];
}

$result = 0;
$dbisopen = 0;
if( $result == 0 )
{
	#打开数据库
	$db = new dbstuff;
	$pconnect = 1;
	$db->connect($dbhost, $dbuser, $dbpw, $dbname, $pconnect);
	$dbhost = $dbuser = $dbpw = $dbname = $pconnect = NULL;
	$dbisopen = 1;
}

$msg = "";
if( $result == 0 )
{
	if( $command == 1 )
	{
		if ( $v1 == "315559232" || $v1 == "311196984" || $v1 == "314059098" || $v1 == "311155142" || $v1 == "314803476" || $v1=="318082374" || $v1=="315543288" )
		{ // 测试服
			// 获取IP地址所在国家
			$ipcountry = get_ipcountry( $db, $clientip );
			$msg = "\"s\":\"999\", \"h\":\"23.20.104.41\" , \"p\":\"10001\" , \"close\":\"0\", \"force_android_ver\":\"0\", \"force_ios_ver\":\"0\", \"ipcountry\":\"$ipcountry\"";
		}
		else
		{
			
			$tempversion = explode( ".", $version );
			if ( $tempversion != null && $tempversion != FALSE )
			{
				if ( count($tempversion) == 3 )
				{
					$version = $tempversion[2];
				}
			}

			// 指定版本去测试服
			$auditing_version = 0;
			if ( $auditing_version > 0 && $version >= $auditing_version )
			{
				// 获取IP地址所在国家
				$ipcountry = get_ipcountry( $db, $clientip );
				$msg = "\"s\":\"999\", \"h\":\"23.20.104.41\" , \"p\":\"10001\" , \"close\":\"0\", \"force_android_ver\":\"0\", \"force_ios_ver\":\"0\", \"ipcountry\":\"$ipcountry\"";
			}
			else
			{
				$msg = access( $v1, $v2, $v3, $v4, $v5, $lang, $country, $channelId, $sys, $version, $clientip, $db );
			}
		}
	}
	else if ( $command == 8 )
	{
		$msg = select( $v1, $v2, $db );
	}
}

if( $dbisopen )
{
	$db->close();
}


echo "{ \"cmd\":\"$command\", $msg }";

?>
