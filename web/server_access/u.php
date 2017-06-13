<?php
require_once 'config.inc.php';
require_once 'lib/db.inc.php';

// 将utf-8转换为gb
// $content = iconv("utf-8","gb2312//IGNORE",$content);

// 将gb转换为utf-8
// $content = iconv("gb2312","utf-8//IGNORE",$content);

function wloglogin( $db, $username, $msg )
{
	$myip = $_SERVER["REMOTE_ADDR"];
	$sql="INSERT INTO loginlog (username,ip,optime,msg) values('$username','$myip',now(),'$msg')";

	$db->query($sql);
}

// v1用户名，v2密码，v5DeviceID
function login( $v1, $v2, $v4, $v5, $db )
{
	wloglogin( $db, $v1, "login begin" );
	$uid = 0;
	$usertype = 0;
	$locktime = 0;
	$activeflag = 0;
	$result = -1;
	$szUserID = "";
	$token = "";

	$v1 = mysql_real_escape_string($v1);
	$v2 = mysql_real_escape_string($v2);
	
	// 快速登陆，直接拿设备码作为登陆唯一标示
	if ( $v1 == "" && $v2 == "" && $v5 != "" )
	{
		$result = 0;
		$sql="select id,usertype,locktime,activeflag from `user` where name='$v5'";
		$query = $db->query($sql);
		if($sth = $db->fetch_row($query))
		{ // 找到了这个用户
			$v1 = $v5;
			$uid = $sth[0];
			$usertype = $sth[1];
			$locktime = $sth[2];
			$activeflag = $sth[3];
			$db->free_result($query);
			wloglogin( $db, $v1, "login fast" );
		}
		else
		{ // 没找到这个用户，就插入一条
			$thistime = time();
			
			$sql="select max(id) from user";
			$query = $db->query($sql);
			if($sth = $db->fetch_row($query))
			{
				$szUserID = $sth[0]+1;
			}
			$db->free_result($query);
			
			$sql="insert into user ( id, name, passwd, activeflag, create_time, regip, flashtime ) values('$szUserID','$v5','', '1', '$thistime', '$v4','$thistime') ";
			if(!( $db->query($sql) ))
			{
				$result = -1;
			}
			
			$uid = $szUserID;
			$v1 = $v5;
			return $result."\n".$usertype."\n"."0\n".$uid."\n".$v1."\n".$token."\n10000\n\n$v5\n";
		}
	}
	
	// 正常账号密码登陆
	if ( $v1 != "" && $v2 != ""  )
	{
		$sql="select id,usertype,locktime,activeflag,(passwd=MD5(CONCAT('$v2','freeskyuser'))),(token='$v2') from user where name='$v1'";
		$query = $db->query($sql);
		if($sth = $db->fetch_row($query))
		{
			$uid = $sth[0];
			$usertype = $sth[1];
			$locktime = $sth[2];
			$activeflag = $sth[3];
			$checkpass = $sth[4];
			$checktoken = $sth[5];
			$db->free_result($query);
	
			$result = 0;
			$token = '';
			if( $checkpass != 0 )
			{
				// 如果是密码
				$result = 1;
				// 刷新token
				$token = create_password( 32 );
				$thistime = time();
				$sql="UPDATE user SET token='$token',flashtime='$thistime' WHERE id='$uid'";
				$db->query($sql);
			}
			elseif( $checktoken != 0 )
			{
				// 如果是token
				$result = 1;
				$token = $v2;
			}
	
			if( $result == 0 )
			{
				$result = -2;
			}
			else if( $locktime != 0 )
			{
				$result = -3;	// 已经被锁定
			}
			else if( $activeflag == 0 )
			{
				$result = -4;	// 没有被激活
			}
			else
			{
				$result = 0;
			}
		}
		else
		{
			$result = -1;
		}
	}
	
	// 记录留存数据
	/*if( $result == 0 )
	{
		$dateday = -1;
		$sql="select datediff(now(),firsttime) from retention where deviceid='$v5'";
		$query = $db->query($sql);
		if($sth = $db->fetch_row($query))
		{
			$dateday = $sth[0];
			$db->free_result($query);
			if( $dateday > 0 && $dateday < 31 )
			{
				if( $dateday < 10 )
				{
					$sql="update retention set day0$dateday=1 where deviceid='$v5'";
				}
				else
				{
					$sql="update retention set day$dateday=1 where deviceid='$v5'";
				}
				$query = $db->query($sql);
			}
		}
		else
		{
			$sql="insert into retention (deviceid,firsttime) values('$v5',now())";
			$query = $db->query($sql);
		}
	}*/
	// 记录留存数据完毕
	return $result."\n".$usertype."\n"."0\n".$uid."\n".$v1."\n".$token."\n10000\n\n$v5\n";

}

function create_password( $pw_length )
{
	$randpwd = '';
	for ($i = 0; $i < $pw_length; $i++) 
	{
		if( mt_rand( 1, 3 ) == 1 )
			$randpwd .= chr(mt_rand(48, 57));
		elseif( mt_rand( 1, 3 ) == 2 )
			$randpwd .= chr(mt_rand(65, 90));
		else
			$randpwd .= chr(mt_rand(97, 122));
	}
	return $randpwd;
}

// userid,lockmin
function lockuser( $v1, $v2, $db )
{
	$locktime = 0;
	$result = 0;

	if( $v2 <= 0 )
		$locktime = 0;
	else
		$locktime = $v2*60 + time();

	$sql="update user set locktime='$locktime' where id='$v1'";
	if(!( $db->query($sql) ))
	{
		$result = -1;
	}
	$db->free_result($query);
	return $v1."\n".$result."\n";
}

// 兑换码
function award( $actorid, $serverid, $szUserID, $ip, $cardnumber, $cdkey, $sys, $channel, $db )
{
	// n|3
	$cardlen = strlen( $cardnumber );
	if( $cardlen < 5 )
	{
		return "0\n0\n-1\n"; // 卡号位数不对
	}
	$nolen = $cardlen-3;
	$cardid = substr( $cardnumber, 0, $nolen );
	$cardpsw = substr( $cardnumber, $nolen );
	
	$cdkey_index = -1;
	$limit_sys = 0;
	$limit_channel = 0;
	$awardgroup = 0;
	$overduetime = 0;
	$userid = 0;
	
	// 查询兑换码信息
	$result = 0;
	$sql="SELECT cdkey_index, limit_sys, limit_channel, awardgroup, overduetime, userid from card where id='$cardid' and passwd='$cardpsw'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$cdkey_index = $sth[0];
		$limit_sys = $sth[1];
		$limit_channel = $sth[2];
		$awardgroup = $sth[3];
		$overduetime = $sth[4];
		$userid = $sth[5];
	}
	else
	{
		$result = -2; // 兑换码不正确
	}
	$db->free_result($query);
	
	// 兑换码不正确
	if( $result < 0 )
	{
		return "0\n0\n$result\n";
	}
	
	// 这里判断$cdkey_index是否可以使用 通过cdkey "00000000000000000000000000000000"
	if( $cdkey_index >= 0 && $cdkey_index < 128 )
	{
		$pos_disable = substr( $cdkey, $cdkey_index, 1 );
		if( $pos_disable == "1" )
		{
			$result = -3; // 该玩家已经使用过该类型的卡了
			return $cdkey_index."\n0\n$result\n";
		}
	}
	
	// 这张卡是否被其它玩家使用
	if ( $userid > 0 )
	{
			$result = -4; // 这张卡已经被使用过了
			return $cdkey_index."\n0\n$result\n";
	}
	
	// 系统检查
	if ( $limit_sys == 1 && $limit_sys != $sys )
	{
			$result = -5; // 这张卡只能在ios平台使用
			return $cdkey_index."\n0\n$result\n";
	}
	if ( $limit_sys == 2 && $limit_sys != $sys )
	{
			$result = -6; // 这张卡只能在android平台使用
			return $cdkey_index."\n0\n$result\n";
	}
	
	// 渠道检查
	if ( $limit_channel > 0 && $limit_channel != $channel )
	{
			$result = -7; // 这张卡只能指定渠道用户使用
			return $cdkey_index."\n0\n$result\n";
	}
	
	// 过期检查
	if ( $overduetime > 0 && time() >= $overduetime )
	{
			$result = -8; // 这张卡已经过期
			return $cdkey_index."\n0\n$result\n";
	}
	
	// 更新使用者
	if ( $cardid > 100 )
	{
		$sql="UPDATE card set userid='$szUserID',serverid='$serverid',actorid='$actorid',usedtime=now() where id='$cardid'";
		if(!( $db->query($sql) ))
		{
			$result = -2; // 兑换码不正确
		}
	}
	
	if( $result < 0 )
	{
		return $cdkey_index."\n0\n$result\n";
	}
	
	return $cdkey_index."\n".$awardgroup."\n".$result."\n".$cardnumber;
}

if (!get_magic_quotes_gpc())
{
	$command=addslashes(@$_GET['c']);
	$client_index=addslashes(@$_GET['i']);
	$authid=addslashes(@$_GET['a']);
	$v1=addslashes(@$_GET['v1']);
	$v2=addslashes(@$_GET['v2']);
	$v3=addslashes(@$_GET['v3']);
	$v4=addslashes(@$_GET['v4']);
	$v5=addslashes(@$_GET['v5']);
	$v6=addslashes(@$_GET['v6']);
	$v7=addslashes(@$_GET['v7']);
	$v8=addslashes(@$_GET['v8']);
}
else
{
	$command=@$_GET['c'];
	$client_index=@$_GET['i'];
	$authid=@$_GET['a'];
	$v1=@$_GET['v1'];
	$v2=@$_GET['v2'];
	$v3=@$_GET['v3'];
	$v4=@$_GET['v4'];
	$v5=@$_GET['v5'];
	$v6=@$_GET['v6'];
	$v7=@$_GET['v7'];
	$v8=@$_GET['v8'];
}

$result = 0;
$dbisopen = 0;
if( $result == 0 )
{
	#打开数据库
	$db = new dbstuff;
	$pconnect = 0;
	$db->connect($dbhost, $dbuser, $dbpw, $dbname, $pconnect);
	$dbhost = $dbuser = $dbpw = $dbname = $pconnect = NULL;
	$dbisopen = 1;
}
$uid = 0;
$usertype = 0;
$locktime = 0;
$activeflag = 0;
$msg = "-1\n";

if( $result == 0 )
{
	if( $command == 1 )
	{ // 登陆
			$msg = login( $v1, $v2, $v4, $v5, $db );
	}
	else if( $command == 2 )
	{ // 兑换码
			$msg = award( $v1, $v2, $v3, $v4, $v5, $v6, $v7, $v8, $db );
	}
	else if( $command == 3 )
	{ // 锁账户
			$msg = lockuser( $v1, $v2, $db );
	}
}

if( $dbisopen )
{
	$db->close();
}
print $client_index."\n".$authid."\n".$msg;
?>
