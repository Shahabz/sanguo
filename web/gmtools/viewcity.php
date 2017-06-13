<?php 
require_once 'db.inc.php';
require_once 'define.php';
error_reporting(0);
date_default_timezone_set("Etc/GMT");
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}


$host = $_POST['host'];
$actorid = $_POST['actorid'];
$online = $_POST['online'];
$sysdbh = $_POST['sysdbh'];
$sysdbu = $_POST['sysdbu'];
$sysdbp = $_POST['sysdbp'];
$sysdbd = $_POST['sysdbd'];
$server = $host;
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
</script>
</head>

<body align="center">
<div width="980" align="center">

<?php
	$cmd = 7;
	$string = "";
	$arg1 = 0;
	$arg2 = 0;
	$arg3 = 0;
	$arg4 = 0;
	$sock = fsockopen ("$host" );
	if( !$sock )
	{
		echo "$server not connect!";
	}
	else
	{
		wlogop( "$cmd|0|0|0|$actorid|$string\0" );
		fputs( $sock, "$cmd|0|0|0|$actorid|$string\0" );
		$recvbuff = "";
	  while(!feof($sock)) 
	  {     
      $data = fread($sock,81920);
      $recvbuff .= $data;        
    }
    
		$recvarray = explode('|',$recvbuff); 
		
		$result = 100 - $recvarray[0];
		if ( $result != 0 )
			echo "$g_result[$result] 指令执行完毕(服务器:$server 角色ID:$actorid) 指令:$cmd|$arg1|$arg2|$arg3|$arg4|$string<br>";
		else
			echo "$g_result[$result] 指令执行失败(服务器:$server 角色ID:$actorid)<br>";
		
		$result_string = $recvarray[1];
		//$result_string = iconv("GB2312", "UTF-8//IGNORE", $recvarray[1] );
		
		// 根据utf8的编码范围，是可以剔除掉非法utf8字符的
		$result_string = preg_replace('/[\x00-\x08\x10\x0B\x0C\x0E-\x19\x7F]'.
 '|[\x00-\x7F][\x80-\xBF]+'.
 '|([\xC0\xC1]|[\xF0-\xFF])[\x80-\xBF]*'.
 '|[\xC2-\xDF]((?![\x80-\xBF])|[\x80-\xBF]{2,})'.
 '|[\xE0-\xEF](([\x80-\xBF](?![\x80-\xBF]))|(?![\x80-\xBF]{2})|[\x80-\xBF]{3,})/S', '', $result_string );
 
		str_replace("'", '"', $result_string);//将json数据中的单引替换成双引
		preg_replace('/,\s*([\]}])/m', '$1', $result_string); // 不能有多余的逗号 如：[1,2,]
//echo $result_string;
		$jsonArray = json_decode( $result_string, true );
		$serverid = $jsonArray['serverid'];
		$cityid = $jsonArray['cityid'];
		$posx = $jsonArray['posx'];
		$posy = $jsonArray['posy'];
		$state = $jsonArray['state'];
		$sflag = $jsonArray['sflag'];
		$laird_actorid = $jsonArray['laird_actorid'];
		$laird_name = $jsonArray['laird_name'];
		$laird_shape = $jsonArray['laird_shape'];
		$laird_level = $jsonArray['laird_level'];
		$headid = $jsonArray['headid'];
		$country = $jsonArray['country'];
		$language = $jsonArray['language'];	
		$clubname = $jsonArray['clubname'];
		$vipexp = $jsonArray['vipexp'];
		$viplevel = $jsonArray['viplevel'];
		$vipduration = $jsonArray['vipduration'];
		$wood =  number_format($jsonArray['wood']);
		$wood_safe = number_format($jsonArray['wood_safe']);
		$food = number_format($jsonArray['food']);
		$food_safe = number_format($jsonArray['food_safe']);
		$iron = number_format($jsonArray['iron']);
		$iron_safe = number_format($jsonArray['iron_safe']);
		$mithril = number_format($jsonArray['mithril']);
		$mithril_safe = number_format($jsonArray['mithril_safe']);
		$battlepower = number_format($jsonArray['battlepower']);
		$body = $jsonArray['body'];
		$honorpoint = $jsonArray['honorpoint'];
		$monster_killlevel = $jsonArray['monster_killlevel'];
		$questid1 = $jsonArray['questid1'];
		$questid2 = $jsonArray['questid2'];
		$questid3 = $jsonArray['questid3'];
		$questid4 = $jsonArray['questid4'];
		$questid5 = $jsonArray['questid5'];
		$mission_id = $jsonArray['mission_id'];
		$mission_value = $jsonArray['mission_value'];
		$mission_completenum = $jsonArray['mission_completenum'];
		$attack_eventid = $jsonArray['attack_eventid'];
		$attack_eventwait = $jsonArray['attack_eventwait'];
		$cdepth = $jsonArray['cdepth'];
		$rai = $jsonArray['rai'];
		$rcd = $jsonArray['rcd'];
		$ratkcd = $jsonArray['ratkcd'];
		$worker_kind1 = $jsonArray['worker_kind1'];
		$worker_sec1 = $jsonArray['worker_sec1'];
		$worker_kind2 = $jsonArray['worker_kind2'];
		$worker_sec2 = $jsonArray['worker_sec2'];	
		$headfdate = floor($headid/86400);
		$headfile = $serverid."_".$cityid."_".$headid.".jpg";
		
		// 基本信息
		echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>城池ID</td><td class='Data DataRightAlign'>$cityid</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>位置</td><td class='Data DataRightAlign'>X:$posx,Y:$posy</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>状态</td><td class='Data DataRightAlign'>$state</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>标记</td><td class='Data DataRightAlign'>$sflag</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>领主ID</td><td class='Data DataRightAlign'>$laird_actorid</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>领主名称</td><td class='Data DataRightAlign'>$laird_name</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>领主形象</td><td class='Data DataRightAlign'>$laird_shape</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>领主等级</td><td class='Data DataRightAlign'>$laird_level</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>自定义头像</td><td class='Data DataRightAlign'>$headid<a href='http://kor.res.gohogame.com/head/verify/$serverid/$headfdate/$headfile', target='_blank'>查看</a>|<a href='viewcity_action.php?cmd=64&host=$host&cityid=$cityid', target='_blank'>删除</a></td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>国家</td><td class='Data DataRightAlign'>$country</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>语言</td><td class='Data DataRightAlign'>$language</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>联盟</td><td class='Data DataRightAlign'>$clubname</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>VIP经验</td><td class='Data DataRightAlign'>$vipexp</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>VIP等级</td><td class='Data DataRightAlign'>$viplevel</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>VIP持续时间</td><td class='Data DataRightAlign'>$vipduration</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>木材(安全木材)</td><td class='Data DataRightAlign'>$wood($wood_safe)</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>魂力(安全魂力)</td><td class='Data DataRightAlign'>$food($food_safe)</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>铁矿(安全铁矿)</td><td class='Data DataRightAlign'>$iron($iron_safe)</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>水银(安全水银)</td><td class='Data DataRightAlign'>$mithril($mithril_safe)</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>战力</td><td class='Data DataRightAlign'>$battlepower</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>体力</td><td class='Data DataRightAlign'>$body</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>联盟荣誉</td><td class='Data DataRightAlign'>$honorpoint</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>击杀最高级野怪</td><td class='Data DataRightAlign'>$monster_killlevel</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>主任务</td><td class='Data DataRightAlign'>$questid1</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>支线任务</td><td class='Data DataRightAlign'>$questid2</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>支线任务</td><td class='Data DataRightAlign'>$questid3</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>支线任务</td><td class='Data DataRightAlign'>$questid4</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>支线任务</td><td class='Data DataRightAlign'>$questid5</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>使命任务</td><td class='Data DataRightAlign'>$mission_id</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>使命数值</td><td class='Data DataRightAlign'>$mission_value</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>完成使命数量</td><td class='Data DataRightAlign'>$mission_completenum</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>攻城事件ID</td><td class='Data DataRightAlign'>$attack_eventid</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>攻城事件等待秒</td><td class='Data DataRightAlign'>$attack_eventwait</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>千层楼</td><td class='Data DataRightAlign'>$cdepth</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>机器人AI</td><td class='Data DataRightAlign'>$rai</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>机器人升级倒计时</td><td class='Data DataRightAlign'>$rcd</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>机器攻击倒计时</td><td class='Data DataRightAlign'>$ratkcd</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>建筑队列1</td><td class='Data DataRightAlign'>$worker_kind1 剩余时间: $worker_sec1</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>建筑队列2</td><td class='Data DataRightAlign'>$worker_kind2 剩余时间: $worker_sec2</td></tr>";
		echo"</table>";
		
		// 部队
		echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
			// 部队
			echo "<tr>";
				echo "<td>";
				echo "<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
				echo "<th class='DataHeader'>已有兵种</th>";
				echo "<th class='DataHeader'>等级</th>";
				echo "<th class='DataHeader'>数量</th>";
				echo "<th class='DataHeader'>操作</th>";
				$corpscount = count($jsonArray['corpslist']);
				for ( $tmpi = 0; $tmpi < $corpscount; $tmpi++ )
				{
					$corps = $jsonArray['corpslist'][$tmpi]['corps'];
					$level = $jsonArray['corpslist'][$tmpi]['level']+1;
					$num = $jsonArray['corpslist'][$tmpi]['num'];
					$name = $g_CorpsName[$corps+1];
					echo "<tr class='Record_2'><td class='Data DataRightAlign'>$name</td><td class='Data DataRightAlign'>Lv.$level</td><td class='Data DataRightAlign'>$num</td><td><a href='viewcity_action.php?cmd=34&host=$host&cityid=$cityid&corps=$corps&level=$level&num=-$num', target='_blank'>全删</a></td></tr>";
				}
				echo "</table>";
				echo "</td>";
			
			  // 伤兵
			  echo "<td>";
				echo "<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
				echo "<th class='DataHeader'>伤兵兵种</th>";
				echo "<th class='DataHeader'>等级</th>";
				echo "<th class='DataHeader'>数量</th>";
				$woundcount = count($jsonArray['woundlist']);
				for ( $tmpi = 0; $tmpi < $woundcount; $tmpi++ )
				{
					$corps = $jsonArray['woundlist'][$tmpi]['corps'];
					$level = $jsonArray['woundlist'][$tmpi]['level']+1;
					$num = $jsonArray['woundlist'][$tmpi]['num'];
					$name = $g_CorpsName[$corps+1];
					echo "<tr class='Record_2'><td class='Data DataRightAlign'>$name</td><td class='Data DataRightAlign'>Lv.$level</td><td class='Data DataRightAlign'>$num</td></tr>";
				}
				echo "</table>";
				echo "</td>";
				
				// 治疗
			  echo "<td>";
				echo "<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
				echo "<th class='DataHeader'>治疗兵种</th>";
				echo "<th class='DataHeader'>等级</th>";
				echo "<th class='DataHeader'>数量</th>";
				$healcount = count($jsonArray['heallist']);
				for ( $tmpi = 0; $tmpi < $healcount; $tmpi++ )
				{
					$corps = $jsonArray['heallist'][$tmpi]['corps'];
					$level = $jsonArray['heallist'][$tmpi]['level']+1;
					$num = $jsonArray['heallist'][$tmpi]['num'];
					$name = $g_CorpsName[$corps+1];
					echo "<tr class='Record_2'><td class='Data DataRightAlign'>$name</td><td class='Data DataRightAlign'>Lv.$level</td><td class='Data DataRightAlign'>$num</td></tr>";
				}
				echo "</table>";
				echo "</td>";
				
			echo "</tr>";
			
		echo "</table>";
		
		// 建筑列表
		echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
		echo"<th class='DataHeader'>建筑</th>";
		echo"<th class='DataHeader'>等级</th>";
		echo"<th class='DataHeader'>状态</th>";
		echo"<th class='DataHeader'>value1</th>";
		echo"<th class='DataHeader'>value2</th>";
		echo"<th class='DataHeader'>value3</th>";
		echo"<th class='DataHeader'>value4</th>";
		$buildingcount = count($jsonArray['buildinglist']);
		for ( $tmpi = 0; $tmpi < $buildingcount; $tmpi++ )
		{
			$kind = $jsonArray['buildinglist'][$tmpi]['kind'];
			$level = $jsonArray['buildinglist'][$tmpi]['level'];
			$state = $jsonArray['buildinglist'][$tmpi]['state'];
			$value1 = $jsonArray['buildinglist'][$tmpi]['value1'];
			$value2 = $jsonArray['buildinglist'][$tmpi]['value2'];
			$value3 = $jsonArray['buildinglist'][$tmpi]['value3'];
			$value4 = $jsonArray['buildinglist'][$tmpi]['value4'];
			$name = $g_BuildingName[$kind];
			echo "<tr class='Record_2'><td class='Data DataRightAlign'>$name</td><td class='Data DataRightAlign'>$level</td><td class='Data DataRightAlign'>$state</td><td class='Data DataRightAlign'>$value1</td><td class='Data DataRightAlign'>$value2</td><td class='Data DataRightAlign'>$value3</td><td class='Data DataRightAlign'>$value4</td></tr>";
		}
		echo"</table>";
		
		echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
			echo "<tr>";
				// 出征队列
				echo "<td>";
				echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
				echo"<th class='DataHeader'>出征目标</th>";
				echo"<th class='DataHeader'>位置</th>";
				echo"<th class='DataHeader'>名称</th>";
				echo"<th class='DataHeader'>状态</th>";
				echo"<th class='DataHeader'>动作</th>";
				echo"<th class='DataHeader'>详细</th>";
				$battlelistcount = count($jsonArray['battlelist']);
				for ( $tmpi = 0; $tmpi < $battlelistcount; $tmpi++ )
				{
					$ttype = $jsonArray['battlelist'][$tmpi]['ttype'];
					$tposx = $jsonArray['battlelist'][$tmpi]['tposx'];
					$tposy = $jsonArray['battlelist'][$tmpi]['tposy'];
					$tname = $jsonArray['battlelist'][$tmpi]['tname'];
					$state = $jsonArray['battlelist'][$tmpi]['state'];
					$action = $jsonArray['battlelist'][$tmpi]['action'];
					$armyindex = $jsonArray['battlelist'][$tmpi]['index'];
					
					$statename = $g_ArmyStateName[$state];
					$actionname = $g_ArmyActionName[$action];
					echo "<tr class='Record_2'><td class='Data DataRightAlign'>$ttype</td><td class='Data DataRightAlign'>$tposx,$tposy</td><td class='Data DataRightAlign'>$tname</td><td class='Data DataRightAlign'>$statename</td><td class='Data DataRightAlign'>$actionname</td><td><a href='viewarmy.php?host=$host&armyindex=$armyindex', target='_blank'>详细</a></td></tr>";
				}
				echo"</table>";
				echo "</td>";
				
				// 挨打队列
				echo "<td>";
				echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
				echo"<th class='DataHeader'>来者</th>";
				echo"<th class='DataHeader'>位置</th>";
				echo"<th class='DataHeader'>名称</th>";
				echo"<th class='DataHeader'>状态</th>";
				echo"<th class='DataHeader'>动作</th>";
				echo"<th class='DataHeader'>详细</th>";
				$underfirelistcount = count($jsonArray['underfirelist']);
				for ( $tmpi = 0; $tmpi < $underfirelistcount; $tmpi++ )
				{
					$ftype = $jsonArray['underfirelist'][$tmpi]['ftype'];
					$fposx = $jsonArray['underfirelist'][$tmpi]['fposx'];
					$fposy = $jsonArray['underfirelist'][$tmpi]['fposy'];
					$fname = $jsonArray['underfirelist'][$tmpi]['fname'];
					$state = $jsonArray['underfirelist'][$tmpi]['state'];
					$action = $jsonArray['underfirelist'][$tmpi]['action'];
					$armyindex = $jsonArray['underfirelist'][$tmpi]['index'];
					
					$statename = $g_ArmyStateName[$state];
					$actionname = $g_ArmyActionName[$action];
					echo "<tr class='Record_2'><td class='Data DataRightAlign'>$ftype</td><td class='Data DataRightAlign'>$fposx,$fposy</td><td class='Data DataRightAlign'>$fname</td><td class='Data DataRightAlign'>$statename</td><td class='Data DataRightAlign'>$actionname</td><td><a href='viewarmy.php?host=$host&armyindex=$armyindex', target='_blank'>详细</a></td></tr>";
				}
				echo"</table>";
				echo "</td>";
				
				// 帮助队列
				echo "<td>";
				echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
				echo"<th class='DataHeader'>援助者</th>";
				echo"<th class='DataHeader'>位置</th>";
				echo"<th class='DataHeader'>名称</th>";
				echo"<th class='DataHeader'>状态</th>";
				echo"<th class='DataHeader'>详细</th>";
				$helplistcount = count($jsonArray['helplist']);
				for ( $tmpi = 0; $tmpi < $helplistcount; $tmpi++ )
				{
					$ftype = $jsonArray['helplist'][$tmpi]['ftype'];
					$fposx = $jsonArray['helplist'][$tmpi]['fposx'];
					$fposy = $jsonArray['helplist'][$tmpi]['fposy'];
					$fname = $jsonArray['helplist'][$tmpi]['fname'];
					$state = $jsonArray['helplist'][$tmpi]['state'];
					$armyindex = $jsonArray['helplist'][$tmpi]['index'];
					$statename = $g_ArmyStateName[$state];
					echo "<tr class='Record_2'><td class='Data DataRightAlign'>$ftype</td><td class='Data DataRightAlign'>$fposx,$fposy</td><td class='Data DataRightAlign'>$fname</td><td class='Data DataRightAlign'>$statename</td><td><a href='viewarmy.php?host=$host&armyindex=$armyindex', target='_blank'>详细</a></td></tr>";
				}
				echo"</table>";
				echo "</td>";
				
			echo "</tr>";
		echo "</table>";

	}
	fclose ($sock);

?>

</div>
</body>
</html>
