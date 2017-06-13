<?php 
require_once 'db.inc.php';
require_once 'define.php';
date_default_timezone_set("Etc/GMT");
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

if(!empty($_POST['radio'])){
$ds = $_POST['radio'];
}
if(!empty($_POST['systalk_type'])){
$systalk_type = $_POST['systalk_type'];
}
if(!empty($_POST['systalk_info'])){
$systalk_info = $_POST['systalk_info'];
}


$proc_type=$_POST["type"];
$begindate=$_POST["begindate"];
$enddate=$_POST["enddate"];
$begintime=$_POST["begintime"];
$endtime=$_POST["endtime"];


// 时间戳范围
$begintimestamp = strtotime($begindate." 00:00:00");
$endtimestamp = strtotime($enddate." 23:59:59");
if ( $begindate == $enddate )
{
	$begintimestamp = strtotime($begindate." ".$begintime);
	$endtimestamp = strtotime($enddate." ".$endtime);
}

// 时间字符串范围
$bdate = $begindate." 00:00:00";
$edate = $enddate." 23:59:59";
if ( $begindate == $enddate )
{
	$bdate = $begindate." ".$begintime;
	$edate = $enddate." ".$endtime;
}

$datestr = explode( "-", $begindate );
$tabname = "glog_".$datestr[0].$datestr[1].$datestr[2];

if ( isset($_POST["favor"]) )
		$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
else
{
	echo"没有选择服务器";
	exit;
}
$num = count($array);           //计算一共有多少项被选择
$interest = "";                 //设置一个空变量interest

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<style type="text/css"><!--
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

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script language="javascript">
 function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
 }
 
  function post(url, params) {
		var temp = document.createElement("form");
		temp.action = url;
		temp.method = "post";
		temp.target = '_blank';
		temp.style.display = "none";
		for (var x in params) {
			var opt = document.createElement("textarea");
			opt.name = x;
			opt.value = params[x];
			temp.appendChild(opt);
		}
		document.body.appendChild(temp);
		temp.submit();
		return temp;
}

</script>
</head>

<body align="center">
<div width="980" align="center">

<?php

$g_paygoods = array(0=>'无');
function getdbdatalist( $db2 )
{
	Global $g_paygoods;
	$sql="select goodsid, name from paygoods;";
	$query = $db2->query($sql);
	while($row = $db2->fetch_row($query))
	{
		$g_paygoods[$row[0]] = $row[1];
	}
	return 0;
}

$g_mission = array(0=>'无');
function getdbdatalist_mission( $db2 )
{
	Global $g_mission;
	$sql="select id, mission.desc from mission;";
	$query = $db2->query($sql);
	while($row = $db2->fetch_row($query))
	{
		$g_mission[$row[0]] = $row[1];
	}
	return 0;
}

$g_quest = array(0=>'无');
function getdbdatalist_quest( $db2 )
{
	Global $g_quest;
	$sql="select questid, quest.desc from quest;";
	$query = $db2->query($sql);
	while($row = $db2->fetch_row($query))
	{
		$g_quest[$row[0]] = $row[1];
	}
	return 0;
}


$db2 = new dbstuff;
$db2->connect( $data_dbh, $data_dbu, $data_dbp, $data_dbd, 0 );
getdbdatalist( $db2 );
getdbdatalist_mission( $db2 );
getdbdatalist_quest( $db2 );
$db2->close();

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
if ( $proc_type == -3 )
{ // 角色等级分布	
	echo"<th class='DataHeader'>等级</th>";
	echo"<th class='DataHeader'>人数</th>";
}
else if  ( $proc_type == -4 )
{ // 主城等级分布
	echo"<th class='DataHeader'>等级</th>";
	echo"<th class='DataHeader'>人数</th>";
}
else if  ( $proc_type == -5 )
{ // 充值记录
	echo"<th class='DataHeader'>游戏订单</th>";
	echo"<th class='DataHeader'>游戏用户ID</th>";
	echo"<th class='DataHeader'>游戏角色ID</th>";
	echo"<th class='DataHeader'>创角时间</th>";
	echo"<th class='DataHeader'>角色等级</th>";
	echo"<th class='DataHeader'>城池等级</th>";
	echo"<th class='DataHeader'>游戏商品ID</th>";
	echo"<th class='DataHeader'>游戏商品名称</th>";
	echo"<th class='DataHeader'>awardgroup</th>";
	echo"<th class='DataHeader'>IP</th>";
	echo"<th class='DataHeader'>时间</th>";
}
else if  ( $proc_type == -6 )
{ // 消费记录
	echo"<th class='DataHeader'>途径</th>";
	echo"<th class='DataHeader'>细分途径</th>";
	echo"<th class='DataHeader'>使用</th>";
	echo"<th class='DataHeader'>剩余</th>";
	echo"<th class='DataHeader'>角色ID</th>";
	echo"<th class='DataHeader'>时间</th>";
}
else if  ( $proc_type == -7 )
{ // 消费详情
	echo"<th class='DataHeader'>消费途径</th>";
	echo"<th class='DataHeader'>消费数量</th>";
}
else if  ( $proc_type == -8 )
{ // 使命
	echo"<th class='DataHeader'>使命ID</th>";
	echo"<th class='DataHeader'>使命名称</th>";
	echo"<th class='DataHeader'>人数</th>";
}
else if  ( $proc_type == -9 )
{ // 任务
	echo"<th class='DataHeader'>任务ID</th>";
	echo"<th class='DataHeader'>任务名称</th>";
	echo"<th class='DataHeader'>人数</th>";
}
else if  ( $proc_type == -13 )
{ // 充值榜
	echo"<th class='DataHeader'>角色ID</th>";
	echo"<th class='DataHeader'>总充值</th>";
}

$recno = 2;
for ( $i = 0; $i < $num; $i++ )
{
	if(isset($array[$i]))
	{
		$db = new dbstuff;
		$pconnect = 0;
		$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

		$sql="SELECT * FROM server_list where id='$array[$i]'";
		$query = $db->query($sql);
		if($row = $db->fetch_row($query))
		{
		
			$db->close();

			$sysid=$row[0];
			$sysname=$row[1];
			$sysip=$row[2];
			$sysport=$row[3];
			$sysdbh=$row[4];
			$sysdbu=$row[5];
			$sysdbp=$row[6];
			$sysdbd=$row[7];
			
			$syslogdb=$row[11];
			$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );
			
			if ( $proc_type == -3 )
			{ // 角色等级分布
				echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td></tr>";
				
				$sql="select level, count(*) from actor_list where create_time >= '$begintimestamp' and create_time <= '$endtimestamp' group by level order by level asc;";
				$query = $db->query($sql);
				$totalcount = 0;
				while($row = $db->fetch_row($query))
				{
					$level=$row[0];
					$count=$row[1];
					$totalcount += $count;
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$level</td>";
					echo"<td class='Data DataRightAlign'>$count</td>";
					echo"</tr>";
				}
				echo"总人数：$totalcount";
			}
			else if  ( $proc_type == -4 )
			{ // 主城等级分布
				echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td></tr>";
			
				$sql="select n.level, count(level) from (select cityid from (select actorid from actor_list where create_time >= '$begintimestamp' and create_time <= '$endtimestamp') a, city b where a.actorid=b.laird_actorid) m, city_building n where m.cityid=n.cityid and n.offset=0 group by n.level order by n.level asc;";
				$query = $db->query($sql);
				$totalcount = 0;
				while($row = $db->fetch_row($query))
				{
					$level=$row[0];
					$count=$row[1];
					$totalcount += $count;
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$level</td>";
					echo"<td class='Data DataRightAlign'>$count</td>";		
					echo"</tr>";
				}
				echo"总人数：$totalcount";
			}
			else if  ( $proc_type == -5 )
			{ // 充值记录
				echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>";
				
				$sql="select p.orderid,p.userid,p.actorid,p.actorlevel,p.citylevel,p.productid,p.goodsid,p.awardgroup,p.ip,p.status,p.optime,a.createtime from pay_order p,actor a where p.status=1 and p.optime >= '$begintimestamp' and p.optime <= '$endtimestamp' and a.actorid=p.actorid";
				$query = $db->query($sql);
				$totalrecharge = 0;
				while($row = $db->fetch_row($query))
				{
					$orderid=$row[0];
					$userid=$row[1];
					$actorid=$row[2];
					$actorlevel=$row[3];
					$citylevel=$row[4];
					$productid=$row[5];
					$goodsid=$row[6];
					$awardgroup=$row[7];
					$ip=$row[8];
					$status=$row[9];
					$optime=date("Y-m-d H:i:s", $row[10]) ;
					$createtime=date("Y-m-d H:i:s",$row[11]);
					$totalrecharge += $g_productid[$productid];
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$orderid</td>";
					echo"<td class='Data DataRightAlign'>$userid</td>";		
					echo"<td class='Data DataRightAlign'>$actorid</td>";	
					echo"<td class='Data DataRightAlign'>$createtime</td>";
					echo"<td class='Data DataRightAlign'>$actorlevel</td>";
					echo"<td class='Data DataRightAlign'>$citylevel</td>";
					echo"<td class='Data DataRightAlign'>$goodsid</td>";
					echo"<td class='Data DataRightAlign'>$g_paygoods[$goodsid]</td>";	
					echo"<td class='Data DataRightAlign'>$awardgroup</td>";
					echo"<td class='Data DataRightAlign'>$ip</td>";
					echo"<td class='Data DataRightAlign'>$optime</td>";
					echo"</tr>";
				}
				echo"$sysid 总收入约（美金）：$totalrecharge"."<br>";
			}
			else if  ( $proc_type == -6 )
			{ // 消费记录
				$db2 = new dbstuff;
				$db2->connect( $sysdbh, $sysdbu, $sysdbp, $syslogdb, 1 );
				echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td><td></td><td></td><td></td><td></td></tr>";
			
				$sql="select op,path,object,objectnum,objectid,actor1,actor2,optime from tokenlog where optime >= '$bdate' and optime <= '$edate' order by optime asc limit 2000";
				$query = $db2->query($sql);
				$total = 0;
				$totaltoken = 0;
				while($row = $db2->fetch_row($query))
				{
					$op=$row[0];
					$path=$row[1];
					$object=$row[2];
					$objectnum=$row[3];
					$objectid=$row[4];
					$actor1=$row[5];
					$actor2=$row[6];
					$optime=$row[7] ;
					$totaltoken += (-$objectnum);
					$total += 1;
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$log_path_text[$path]</td>";
					if ( $actor2 < 0 )
						echo"<td class='Data DataRightAlign'>$log_actorbuy_text[$actor2]</td>";
					else
						echo"<td class='Data DataRightAlign'>无</td>";
					echo"<td class='Data DataRightAlign'>$objectnum</td>";	
					echo"<td class='Data DataRightAlign'>$object</td>";	
					echo"<td class='Data DataRightAlign'>$actor1</td>";
					echo"<td class='Data DataRightAlign'>$optime</td>";
					echo"</tr>";
				}
				echo"记录数：$total		总消费钻石：$totaltoken";
				$db2->close();
			}
			else if  ( $proc_type == -7 )
			{ // 消费详情
				$db2 = new dbstuff;
				$db2->connect( $sysdbh, $sysdbu, $sysdbp, $syslogdb, 1 );
				echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td></tr>";
			
				$sql="select path, sum(objectnum) from tokenlog where optime >= '$bdate' and optime <= '$edate' group by path";
				$query = $db2->query($sql);
				$totaltoken = 0;
				while($row = $db2->fetch_row($query))
				{
					$path=$row[0];
					$sum=-$row[1];
					$totaltoken += ($sum);
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>[$path]$log_path_text[$path]</td>";
					echo"<td class='Data DataRightAlign'>$sum</td>";	
					echo"</tr>";
					
					if ( $path == 13 )
					{
						$sql="select actor2, sum(objectnum) from tokenlog where path=13 and optime >= '$bdate' and optime <= '$edate' group by actor2";
						$query2 = $db2->query($sql);
						while($row2 = $db2->fetch_row($query2))
						{
							$actor2=$row2[0];
							$sum=-$row2[1];
							echo"<tr class='Record_2'>";
							echo"<td class='Data DataRightAlign'>--$log_actorbuy_text[$actor2]</td>";
							echo"<td class='Data DataRightAlign'>$sum</td>";	
							echo"</tr>";
						}
					}
				}
				echo"总消费钻石：$totaltoken";
			
				$db2->close();
			}
			else if  ( $proc_type == -8 )
			{ // 使命
				echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td></tr>";
				
				$sql="select m.mission_id, count(*) from (select * from (select actorid from actor_list where create_time >= '$begintimestamp' and create_time <= '$endtimestamp') a, city b where a.actorid=b.laird_actorid) m group by m.mission_id;";
				$query = $db->query($sql);
				$totalcount = 0;
				while($row = $db->fetch_row($query))
				{
					$mission_id=$row[0];
					$count=$row[1];
					$totalcount += $count;
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$mission_id</td>";
					echo"<td class='Data DataRightAlign'>$g_mission[$mission_id]</td>";
					echo"<td class='Data DataRightAlign'>$count</td>";		
					echo"</tr>";
				}
				echo"总人数：$totalcount";
			}
			else if  ( $proc_type == -9 )
			{ // 任务
				echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td></tr>";
				
				$sql="select m.questid0, count(*) from (select * from (select actorid from actor_list where create_time >= '$begintimestamp' and create_time <= '$endtimestamp') a, city b where a.actorid=b.laird_actorid) m group by m.questid0;";
				$query = $db->query($sql);
				$totalcount = 0;
				while($row = $db->fetch_row($query))
				{
					$questid=$row[0];
					$count=$row[1];
					$totalcount += $count;
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$questid</td>";
					echo"<td class='Data DataRightAlign'>$g_quest[$questid]</td>";
					echo"<td class='Data DataRightAlign'>$count</td>";		
					echo"</tr>";
				}
				echo"总人数：$totalcount";
				
				$sql="select m.questid1, count(*) from (select * from (select actorid from actor_list where create_time >= '$begintimestamp' and create_time <= '$endtimestamp') a, city b where a.actorid=b.laird_actorid) m group by m.questid1;";
				$query = $db->query($sql);
				while($row = $db->fetch_row($query))
				{
					$questid=$row[0];
					if ( $questid <= 0 )
						continue;
					$count=$row[1];
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$questid</td>";
					echo"<td class='Data DataRightAlign'>$g_quest[$questid]</td>";
					echo"<td class='Data DataRightAlign'>$count</td>";		
					echo"</tr>";
				}
				
				$sql="select m.questid2, count(*) from (select * from (select actorid from actor_list where create_time >= '$begintimestamp' and create_time <= '$endtimestamp') a, city b where a.actorid=b.laird_actorid) m group by m.questid2;";
				$query = $db->query($sql);
				while($row = $db->fetch_row($query))
				{
					$questid=$row[0];
					if ( $questid <= 0 )
						continue;
					$count=$row[1];
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$questid</td>";
					echo"<td class='Data DataRightAlign'>$g_quest[$questid]</td>";
					echo"<td class='Data DataRightAlign'>$count</td>";		
					echo"</tr>";
				}
				
				$sql="select m.questid3, count(*) from (select * from (select actorid from actor_list where create_time >= '$begintimestamp' and create_time <= '$endtimestamp') a, city b where a.actorid=b.laird_actorid) m group by m.questid3;";
				$query = $db->query($sql);
				while($row = $db->fetch_row($query))
				{
					$questid=$row[0];
					if ( $questid <= 0 )
						continue;
					$count=$row[1];
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$questid</td>";
					echo"<td class='Data DataRightAlign'>$g_quest[$questid]</td>";
					echo"<td class='Data DataRightAlign'>$count</td>";		
					echo"</tr>";
				}
				
				$sql="select m.questid4, count(*) from (select * from (select actorid from actor_list where create_time >= '$begintimestamp' and create_time <= '$endtimestamp') a, city b where a.actorid=b.laird_actorid) m group by m.questid4;";
				$query = $db->query($sql);
				while($row = $db->fetch_row($query))
				{
					$questid=$row[0];
					if ( $questid <= 0 )
						continue;
					$count=$row[1];
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$questid</td>";
					echo"<td class='Data DataRightAlign'>$g_quest[$questid]</td>";
					echo"<td class='Data DataRightAlign'>$count</td>";		
					echo"</tr>";
				}
			}
			$db->close();
		}
		else
		{
			$db->close();
		}
	}
}
?>
</table>



</div>
</body>
</html>
