<?php header("content-Type: text/html; charset=gbk");session_start(); date_default_timezone_set("Etc/GMT");?>
<html>
<style type="text/css">
<!--
.c1 {
	font-family: "宋体";
	font-size: 14px;
	position: relative;
	width: 200px;
	float: left;
	height: 300px;
}
.c2 {
	font-family: "宋体";
	font-size: 14px;
	position: relative;
	width: 190px;
	float: left;
	height: 100px;
}
.c3 {
	font-family: "宋体";
	font-size: 14px;
	position: relative;
	width: 800px;
	float: left;
	height: 400px;
}
.c12 {
	font-family: "宋体";
	font-size: 14px;
	position: relative;
	width: 260px;
	float: left;
	height: 360px;
}
		body {font-family: Arial,Helvetica,sans-serif; font-size: 12px;}
		h1 {font-size: 18px; text-decoration: bold;}
		h2 {font-size: 16px; text-decoration: bold;}
		h3 {font-size: 14px; text-decoration: bold;}
		th {font-size: 12px; border-style: solid; border-width: 1px; text-decoration: bold; padding: 1px;}
		td {font-size: 12px; border-style: solid; border-width: 1px; padding: 1px;}
		code {font-size: 12px; white-space: pre;}
		.TableObject {border-collapse: collapse; border-color: #000000; font-family: 宋体}
		.TableData {border-collapse: collapse; border-color: #000000; font-family: 宋体}
		.TableHeader {border-color: #000000; text-decoration: bold; background-color: #e0e0e0;}
		.ObjectHeader {padding-left: 5px; text-align: left; border-color: #000000; text-decoration: bold;}
		.ObjectOfPrimaryKey {text-align: left; border-color: #aaaaaa; text-decoration: bold; background-color: #e0e0e0;}
		.ObjectOfUniqueKey {text-align: left; border-color: #aaaaaa; background-color: #e0e0e0;}
		.Object {text-align: left; border-color: #aaaaaa;}
		.Record_1 {}
		.Record_2 {background-color: #f0f0f0;}
		.DataHeader {padding-left: 5px; text-align: left; text-decoration: bold; border-color: #000000; background-color: #e0e0e0;}
		.DataOfPrimaryKey {text-align: left; text-decoration: bold; border-color: #aaaaaa; background-color: #e0e0e0;}
		.DataOfUniqueKey {border-color: #aaaaaa;}
		.Data {border-color: #aaaaaa;}
		.DataRightAlign {text-align: right;}
		.DataNull {color: #999999; border-color: #aaaaaa;}
	--></style>
<body>
<?php
require_once 'db.inc.php';

$user_dbh="kor-s1.c6ekaseib22e.us-east-1.rds.amazonaws.com";
$user_dbu = "gamewrite";
$user_dbp = "9dceA698";
$user_dbd = "kor_user";

error_reporting(0);
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

// 将utf-8转换为gb
// $content = iconv("utf-8","gb2312//IGNORE",$content);

// 将gb转换为utf-8
// $content = iconv("gb2312","utf-8//IGNORE",$content);

// 61907d35-37fe-4ef4-9057-80b9e83099f0134627

// SET NAMES gbk;


function calc( $firstday, $thisday, $db )
{
	$thisday_deviceid = 0;
	$keep_deviceid = array();
	// 七日留存
	for ( $tmpi = 0; $tmpi < 15; $tmpi++ )
	{
			if ( $tmpi < 9 )
		  {
		  	$day = "day0".($tmpi+1);
		  }
		  else
		  {
		  	$day = "day".($tmpi+1);
		  }
			$sql="select count(*) from `retention` where `firsttime`>='$thisday  00:00:00' and `firsttime`<='$thisday  23:59:59' and ".$day.">=1;";
			$query = $db->query($sql);
			if($row = $db->fetch_row($query))
			{
				$keep_deviceid[$tmpi] = $row[0];
			}
			$db->free_result($query);
	}
	$day = "day30";
	$sql="select count(*) from `retention` where `firsttime`>='$thisday  00:00:00' and `firsttime`<='$thisday  23:59:59' and ".$day.">=1;";
	$query = $db->query($sql);
	if($row = $db->fetch_row($query))
	{
		$keep_deviceid[14] = $row[0];
	}
	$db->free_result($query);

	
	// 这天新增设备数
	$sql="select count(*) from `retention` where `firsttime` >= '$thisday 00:00:00' and `firsttime` <= '$thisday 23:59:59';";
	$query = $db->query($sql);
	if($row = $db->fetch_row($query))
	{
		$thisday_deviceid = $row[0];
	}
	$db->free_result($query);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>$thisday</th><th class='DataHeader'>留存设备数</th><th class='DataHeader'>留存率</th></tr>";

$odds =  sprintf("%.2f%%", ($keep_deviceid[0]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>次日留存</td><td class='Data DataRightAlign'>$keep_deviceid[0]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[1]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>3日留存</td><td class='Data DataRightAlign'>$keep_deviceid[1]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[2]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>4日留存</td><td class='Data DataRightAlign'>$keep_deviceid[2]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[3]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>5日留存</td><td class='Data DataRightAlign'>$keep_deviceid[3]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[4]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>6日留存</td><td class='Data DataRightAlign'>$keep_deviceid[4]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[5]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>7日留存</td><td class='Data DataRightAlign'>$keep_deviceid[5]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[6]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>8日留存</td><td class='Data DataRightAlign'>$keep_deviceid[6]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[7]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>9日留存</td><td class='Data DataRightAlign'>$keep_deviceid[7]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[8]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>10日留存</td><td class='Data DataRightAlign'>$keep_deviceid[8]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[9]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>11日留存</td><td class='Data DataRightAlign'>$keep_deviceid[9]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[10]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>12日留存</td><td class='Data DataRightAlign'>$keep_deviceid[10]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[11]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>13日留存</td><td class='Data DataRightAlign'>$keep_deviceid[11]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[12]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>14日留存</td><td class='Data DataRightAlign'>$keep_deviceid[12]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[13]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>15日留存</td><td class='Data DataRightAlign'>$keep_deviceid[13]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";

$odds = sprintf("%.2f%%", ($keep_deviceid[14]/$thisday_deviceid)*100 );
echo"<tr class='Record_1'><td class='DataHeader'>30日留存</td><td class='Data DataRightAlign'>$keep_deviceid[14]/$thisday_deviceid</td><td class='Data DataRightAlign'>$odds</td></tr>";
echo "</table>";

}
$dayspace = 0;
if (!get_magic_quotes_gpc())
{
	$firstday=addslashes(@$_GET['f']);
	$thisday=addslashes(@$_GET['d']);
	$dayspace=addslashes(@$_GET['s']);
}
else
{
	$firstday=@$_GET['f'];
	$thisday=@$_GET['d'];
	$dayspace=@$_GET['s'];
}

if( strlen($firstday) < 10 )
	$firstday = date("Y-m-d");
if( strlen($thisday) < 10 )
	$thisday = date("Y-m-d");
#打开数据库
$db = new dbstuff;
$pconnect = 0;
$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

$totle_deviceid = 0;
$totle_yesterday_deviceid = 0;
$totle_today_deviceid = 0;
	
// 总设备数
$sql="select count(*) from `retention`;";
$query = $db->query($sql);
if($row = $db->fetch_row($query))
{
	$totle_deviceid = $row[0];
}
$db->free_result($query);

// 昨日新增设备数
$space = $dayspace-1;
$yesterday = date('Y-m-d',strtotime($space.' day'));
$sql="select count(*) from `retention` where `firsttime` >= '$yesterday 00:00:00' and `firsttime` <= '$yesterday 23:59:59';";
$query = $db->query($sql);
if($row = $db->fetch_row($query))
{
	$totle_yesterday_deviceid = $row[0];
}
$db->free_result($query);

// 今日新增设备数
$today = $thisday;
$sql="select count(*) from `retention` where `firsttime` >= '$today 00:00:00' and `firsttime` <= '$today 23:59:59';";
$query = $db->query($sql);
if($row = $db->fetch_row($query))
{
	$totle_today_deviceid = $row[0];
}
$db->free_result($query);

$lastspace = $dayspace-1;
$nextspace = $dayspace+1;
$lastday = date('Y-m-d',strtotime($lastspace.' day'));
$nextday = date('Y-m-d',strtotime($nextspace.' day'));
echo "<div class=\"c3\">";
echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>总设备数</th><th class='DataHeader'>昨日新增设备数</th><th class='DataHeader'>今日新增设备数</th></tr>";
echo"<tr class='Record_1'><td class='Data DataRightAlign'>$totle_deviceid</td><td class='Data DataRightAlign'>$totle_yesterday_deviceid</td><td class='Data DataRightAlign'>$totle_today_deviceid</td></tr>";
echo"<tr class='Record_1'><td class='Data DataRightAlign'><a href='?f=$lastday&d=$lastday&s=$lastspace'>前一天</a></td><td class='Data DataRightAlign'>$today</td><td class='Data DataRightAlign'><a href='?f=$nextday&d=$nextday&s=$nextspace'>后一天</a></td></tr>";
echo "</table>";
echo "<br></br>";

// 留存计算
for ( $tmpi=1; $tmpi < 9; $tmpi++ )
{
	echo "<div style=\"float:left\">";
	$space = $dayspace-$tmpi;
	calc( 0, date('Y-m-d',strtotime($space.' day')), $db );
	echo "</div>";
}

echo "</div>";
$db->close();
?>

</body>
</html>
