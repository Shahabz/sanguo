<?php header("content-Type: text/html; charset=utf8");session_start();date_default_timezone_set("Etc/GMT"); ?>
<html>
<style type="text/css">
<!--
.c2 {
	font-family: "宋体";
	font-size: 14px;
	position: relative;
	width: 200px;
	float: left;
	height: 100px;
}
.c6 {
	font-family: "宋体";
	font-size: 14px;
	position: relative;
	width: 320px;
	float: left;
	height: 360px;
}
.c12 {
	font-family: "宋体";
	font-size: 14px;
	position: relative;
	width: 460px;
	float: left;
	height: 360px;
}
.d1 {
	font-family: "宋体";
	font-size: 14px;
	position: relative;
	width: 190px;
	float: left;
	height: 100px;
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
		.DataHeader_2 {text-decoration: bold; border-color: #000000; background-color: #e0e0e0;}
		.DataOfPrimaryKey {text-align: left; text-decoration: bold; border-color: #aaaaaa; background-color: #e0e0e0;}
		.DataOfUniqueKey {border-color: #aaaaaa;}
		.Data {border-color: #aaaaaa;}
		.DataRightAlign {text-align: right;}
		.DataLeftAlign {text-align: left;}
		.DataNull {color: #999999; border-color: #aaaaaa;}
	--></style>
<body>
<?php
require_once 'db.inc.php';
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}
// 将utf-8转换为gb
// $content = iconv("utf-8","gb2312//IGNORE",$content);

// 将gb转换为utf-8
// $content = iconv("gb2312","utf-8//IGNORE",$content);

// 61907d35-37fe-4ef4-9057-80b9e83099f0134627


$totle_online = 0;
$totle_actcount = 0;
$all_online = 0;
$all_actcount = 0;
/*function checkuser_today( $userid, $thisday, $db )
{
	$in_time='0000-00-00 00:00:00';
	$sql="select in_time from account where userid='$userid' and in_time >= '$thisday 00:00:00' and in_time <= '$thisday 23:59:59'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$in_time = $sth[0];
	}
	$db->free_result($query);
	return $in_time;
}*/

function calc( $firstday, $thisday, $db )
{
	global $totle_online;
	global $totle_actcount;
	global $all_online;
	global $all_actcount;

	// 活跃
	$actcount = 0;
	$sql="select count(distinct(userid)) from `account` where `in_time` >= '$firstday 00:00:00' and `in_time` <= '$thisday 23:59:59'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$actcount = $sth[0];
	}
	$db->free_result($query);
	
	// 当前在线
	$onlinecount = 0;
	$sql="select game from gamecount order by id desc limit 1 ";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$onlinecount = $sth[0];
	}
	$db->free_result($query);
	
	// 最低在线
	$low_onlinecount = 0;
	$sql="select min(game) from gamecount  where game > 0 and `optime` >= '$firstday 00:00:00' and `optime` <= '$thisday 23:59:59'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$low_onlinecount = $sth[0];
	}
	$db->free_result($query);
	
	// 最高在线
	$high_onlinecount = 0;
	$sql="select max(game) from gamecount  where `optime` >= '$firstday 00:00:00' and `optime` <= '$thisday 23:59:59'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$high_onlinecount = $sth[0];
	}
	$db->free_result($query);
	
	// 平均在线
	$average_onlinecount = 0;
	$sql="select round(avg(game)) from gamecount  where game > 0 and `optime` >= '$firstday 00:00:00' and `optime` <= '$thisday 23:59:59'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$average_onlinecount = $sth[0];
	}
	$db->free_result($query);
	
	echo "<td class='Data DataRightAlign'>$onlinecount</td><td class='Data DataRightAlign'>$low_onlinecount</td><td class='Data DataRightAlign'>$high_onlinecount</td><td class='Data DataRightAlign'>$average_onlinecount</td><td class='Data DataRightAlign'>$actcount</td>";
	$totle_online = $totle_online+$onlinecount;
	$totle_actcount = $totle_actcount+$actcount;
	$all_online = $all_online+$onlinecount;
	$all_actcount = $all_actcount+$actcount;
}


// f=2013-03-12&d=2013-03-13
echo "<p>";
echo "<a href='tj1.php?f=".date("Y-m-d")."&d=".date("Y-m-d")."'>".date("Y-m-d")."<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",strtotime("-1 day"))."&d=".date("Y-m-d",strtotime("-1 day"))."'>".date("Y-m-d",strtotime("-1 day"))."<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",strtotime("-2 day"))."&d=".date("Y-m-d",strtotime("-2 day"))."'>".date("Y-m-d",strtotime("-2 day"))."<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",strtotime("-3 day"))."&d=".date("Y-m-d",strtotime("-3 day"))."'>".date("Y-m-d",strtotime("-3 day"))."<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",strtotime("-4 day"))."&d=".date("Y-m-d",strtotime("-4 day"))."'>".date("Y-m-d",strtotime("-4 day"))."<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",strtotime("-5 day"))."&d=".date("Y-m-d",strtotime("-5 day"))."'>".date("Y-m-d",strtotime("-5 day"))."<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",strtotime("-6 day"))."&d=".date("Y-m-d",strtotime("-6 day"))."'>".date("Y-m-d",strtotime("-6 day"))."<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",strtotime("-7 day"))."&d=".date("Y-m-d",strtotime("-7 day"))."'>".date("Y-m-d",strtotime("-7 day"))."<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",strtotime("-8 day"))."&d=".date("Y-m-d",strtotime("-8 day"))."'>".date("Y-m-d",strtotime("-8 day"))."<a>　";
echo "</p>";

$timestamp = time();
echo "<p>";
echo "<a href='tj1.php?f=".date("Y-m-d",mktime(0,0,0,date("m"),1,date("Y")))."&d=".date("Y-m-d",mktime(23,59,59,date("m"),date("t"),date("Y")))."'>本月<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",mktime(0,0,0,date("m",strtotime("-1 month")),1,date("Y",strtotime("-1 month"))))."&d=".date("Y-m-d",mktime(23,59,59,date("m",strtotime("-1 month")),date("t",strtotime("-1 month")),date("Y",strtotime("-1 month"))))."'>上月<a>　";
echo "<a href='tj1.php?f=".date("Y-m-d",mktime(0,0,0,date("m",strtotime("-2 month")),1,date("Y",strtotime("-2 month"))))."&d=".date("Y-m-d",mktime(23,59,59,date("m",strtotime("-2 month")),date("t",strtotime("-2 month")),date("Y",strtotime("-1 month"))))."'>上上月<a>　";
echo "</p>";

echo "<p>";
echo "<a href='awardad.php'>操作<a>　";
echo "</p>";

if (!get_magic_quotes_gpc())
{
	$firstday=addslashes(@$_GET['f']);
	$thisday=addslashes(@$_GET['d']);
}
else
{
	$firstday=@$_GET['f'];
	$thisday=@$_GET['d'];
}

if( strlen($firstday) < 10 )
	$firstday = date("Y-m-d");
if( strlen($thisday) < 10 )
	$thisday = date("Y-m-d");


#打开数据库
$db = new dbstuff;
$pconnect = 0;
$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="select id,dbh,platname,uip,user_dbd,log_dbd,name,dbu,dbp from server_list where ip<>'127.0.0.1' order by id";
$query = $db->query($sql);

$uid = array();
$uhost = array();
$udb = array();
$amount = array();
$payuser = array();
$todyusercount = array();
$totalusercount = array();
$dhost = array();
$hostname = array();
$servdb = array();
$servname = array();
$dbuser = array();
$dbpw = array();
$server_count = 0;

while($row = $db->fetch_row($query))
{
	$uid[]=$row[0];
	$dhost[]=$row[1];
	$hostname[]=$row[2];
	$uhost[]=$row[3];
	$udb[]=$row[4];
	$servdb[]=$row[5];
	$servname[]=$row[6];
	$dbuser[]=$row[7];
	$dbpw[]=$row[8];
	$amount[]=0;
	$payuser[]=0;
	$server_count++;
}
$db->free_result($query);
$db->close();

$totle_amount = 0;
$totle_pay = 0;
$totle_todyusercount = 0;
$total_totalusercount = 0;
$total_todaynewuserpaycount = 0;
$total_todaynewuserpaycount_money = 0;

$db->connect($user_dbh, $user_dbu, $user_dbp, $user_dbd, $pconnect);
if ( !$db )
	echo "$udb[$tmpi] connect fail!";
	
// 总付费
$amount[$tmpi] = 0;
$sql="select sum(money) from paylog where optime >= '$firstday 00:00:00' and optime <= '$thisday 23:59:59' ";
$query = $db->query($sql);
if($sth = $db->fetch_row($query))
{
	$amount[$tmpi] = $sth[0];
}
$db->free_result($query);

// 付费用户
$payuser[$tmpi] = 0;
$sql="select count(distinct userid) from paylog where optime >= '$firstday 00:00:00' and optime <= '$thisday 23:59:59' ";
$query = $db->query($sql);
if($sth = $db->fetch_row($query))
{
	$payuser[$tmpi] = $sth[0];
}
$db->free_result($query);
$totle_amount = $totle_amount + $amount[$tmpi];
$totle_pay = $totle_pay + $payuser[$tmpi];

// 当日新增用户中付费人数
$todaynewuserpaycount[$tmpi] = 0;
$sql="select count(distinct a.userid), sum(money) from paylog a, (select id from user where FROM_UNIXTIME(create_time, '%Y-%m-%d %H:%i:%S') >= '$firstday 00:00:00' and FROM_UNIXTIME(create_time, '%Y-%m-%d %H:%i:%S') <= '$thisday 23:59:59') b where a.userid=b.id";
$query = $db->query($sql);
if($sth = $db->fetch_row($query))
{
	$todaynewuserpaycount[$tmpi] = $sth[0];
	$total_todaynewuserpaycount_money += $sth[1];
}
$db->free_result($query);
$total_todaynewuserpaycount += $todaynewuserpaycount[$tmpi];

// 当日新增用户数
$todyusercount[$tmpi] = 0;
$sql="select count(*) from user where FROM_UNIXTIME(create_time, '%Y-%m-%d %H:%i:%S') >= '$firstday 00:00:00' and FROM_UNIXTIME(create_time, '%Y-%m-%d %H:%i:%S') <= '$thisday 23:59:59' ";
$query = $db->query($sql);
if($sth = $db->fetch_row($query))
{
	$todyusercount[$tmpi] = $sth[0];
}
$db->free_result($query);
$totle_todyusercount = $totle_todyusercount + $todyusercount[$tmpi];

// 总用户
$totalusercount[$tmpi] = 0;
$sql="select count(*) from user";
$query = $db->query($sql);
if($sth = $db->fetch_row($query))
{
	$totalusercount[$tmpi] = $sth[0];
}
$db->free_result($query);
$total_totalusercount = $total_totalusercount + $totalusercount[$tmpi];

$db->close();


echo "<div class=\"c12\">";
echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th></th><th>充值</th><th>付费用户</th><th>付费用户ARPU</th><th>新增用户</th><th>新增人数中的付费人数</th><th>总用户</th></tr>";

/*for( $tmpi = 0; $tmpi < $server_count; $tmpi++ )
{
		$aupu = 0;
		if( $payuser[$tmpi] > 0 )
		{
			$aupu = $amount[$tmpi]/$payuser[$tmpi];
		}
		echo"<tr class='Record_1'><td width='35%' class='Data DataLeftAlign'>$hostname[$tmpi]</td><td class='Data DataRightAlign'>$amount[$tmpi]</td><td class='Data DataRightAlign'>$payuser[$tmpi]</td><td class='Data DataRightAlign'>".sprintf("%.2f", $aupu)."</td><td class='Data DataRightAlign'>$todyusercount[$tmpi]</td><td class='Data DataRightAlign'>$totalusercount[$tmpi]</td></tr>";
}*/

// 总arup
$totalaupu = $totle_amount/$totle_pay;
echo"<tr class='Record_2'><td width='35%' class='Data DataRightAlign'>总计</td><td class='Data DataRightAlign'>$totle_amount</td><td class='Data DataRightAlign'>$totle_pay</td><td class='Data DataRightAlign'>".sprintf("%.2f", $totalaupu)."</td><td class='Data DataRightAlign'>$totle_todyusercount</td><td class='Data DataRightAlign'>$total_todaynewuserpaycount($total_todaynewuserpaycount_money)</td><td class='Data DataRightAlign'>$total_totalusercount</td></tr>";
echo "</table>";
echo "</div>";


echo "<div class=\"c6\">";
echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader_2'>服务器</th><th class='DataHeader'>当前</th><th class='DataHeader'>最低</th><th class='DataHeader'>最高</th><th class='DataHeader'>平均</th><th class='DataHeader'>活跃用户</th></tr>";
for( $tmpi = 0; $tmpi < $server_count; $tmpi++ )
{
		echo"<tr class='Record_1'><td class='Data DataRightAlign'><a href='talk.php?logdb=$servdb[$tmpi]'>$servname[$tmpi]<a></td>";
		
		$db->connect($dhost[$tmpi], $dbuser[$tmpi], $dbpw[$tmpi], $servdb[$tmpi], $pconnect);
		if (!$db)
		{
			echo "connect $servdb[$tmpi] error!\n";	
		}
		calc( $firstday, $thisday, $db );
		$db->close();
	
		echo"</tr>";
}
echo "<tr class='Record_2'><td class='Data DataRightAlign'>总计</td><td class='Data DataRightAlign'>$totle_online</td><td class='Data DataRightAlign'>-</td><td class='Data DataRightAlign'>-</td><td class='Data DataRightAlign'>-</td><td class='Data DataRightAlign'>$totle_actcount</td>";
echo "</table>";
echo "</div>";

echo "<div class=\"d1\">";
echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>总充值</th><th class='DataHeader'>$totle_amount</th></tr>";
echo "<tr class='TableHeader'><th class='DataHeader'>总付费用户</th><th class='DataHeader'>$totle_pay</th></tr>";
echo "<tr class='TableHeader'><td class='DataHeader'>总计人数</th><th class='DataHeader'>$all_online/$all_actcount</th></tr>";
$payodds = round( $totle_pay/$all_actcount*100, 2 );
echo "<tr class='TableHeader'><td class='DataHeader'>付费率</th><th class='DataHeader'>$payodds%</th></tr>";
echo "</table>";
echo "</div>";

?>

</body>
</html>
