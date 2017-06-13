<?php
error_reporting(0);
require_once '../db.inc.php';

if (!get_magic_quotes_gpc())
{
	$op=addslashes(@$_GET['op']);
	$serverid=addslashes(@$_GET['serverid']);
	$cityid=addslashes(@$_GET['cityid']);
	$headid=addslashes(@$_GET['headid']);
	$actorid=addslashes(@$_GET['actorid']);
	$name=addslashes(@$_GET['name']);
	$vip=addslashes(@$_GET['vip']);
	$r_actorid=addslashes(@$_GET['r_actorid']);
	$r_name=addslashes(@$_GET['r_name']);
	$msg=addslashes(@$_GET['msg']);
}
else
{
	$op=@$_GET['op'];
	$serverid=@$_GET['serverid'];
	$cityid=@$_GET['cityid'];
	$headid=@$_GET['headid'];
	$actorid=@$_GET['actorid'];
	$name=@$_GET['name'];
	$vip=@$_GET['vip'];
	$r_actorid=@$_GET['r_actorid'];
	$r_name=@$_GET['r_name'];
	$msg=@$_GET['msg'];
}



#打开数据库
$db = new dbstuff;
$pconnect = 0;
$db->connect($dbhost, $dbuser, $dbpw, $dbname, $pconnect);
$dbhost = $dbuser = $dbpw = $dbname = $pconnect = NULL;
$db->query("set names utf8");

$optime = time();

$name = mysql_real_escape_string( rawurldecode($name) );
$r_name = mysql_real_escape_string( rawurldecode($r_name) );

if ( $op == 0 ) // 举报头像
{
	$sign = md5( $serverid."+".$cityid."+".$headid );
	$sql="insert into report_head ( serverid, cityid, headid, actorid, name, vip, r_actorid, r_name, sign, optime ) values('$serverid','$cityid','$headid','$actorid','$name','$vip','$r_actorid','$r_name','$sign','$optime') ";
	$db->query($sql);
}
else if ( $op == 1 ) // 举报聊天
{
	$sign = md5( $serverid."+".$cityid."+".$msg );
	$msg = mysql_real_escape_string( rawurldecode($msg) );
	$sql="insert into report_chat ( serverid, cityid, headid, actorid, name, vip, r_actorid, r_name, sign, optime, msg ) values('$serverid','$cityid','$headid','$actorid','$name','$vip','$r_actorid','$r_name','$sign', '$optime','$msg') ";
	$db->query($sql);
}
else if ( $op == 2 ) // 举报名称
{
	$sign = md5( $serverid."+".$cityid."+".$headid );
	$sql="insert into report_name ( serverid, cityid, headid, actorid, name, vip, r_actorid, r_name, sign, optime ) values('$serverid','$cityid','$headid','$actorid','$name','$vip','$r_actorid','$r_name','$sign','$optime') ";
	$db->query($sql);
}

?>