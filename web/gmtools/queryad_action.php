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

$actorid=$_POST["actorid"];
$proc_type=$_POST["type"];
$proc_num=$_POST["num"];
$proc_num2=$_POST["num2"];

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

if ( isset($_POST["favor"]) )
		$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
else
{
	echo"没有选择服务器";
	exit;
}
$num = count($array);           //计算一共有多少项被选择
$interest = "";                 //设置一个空变量interest

$hasactorid = 0;
if(isset($actorid) && $actorid != "" )
{
	$hasactorid = 1;
}
$replace_serverid = 0;
$replace_userid = 0;
$replace_username = 0;
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
 
 function queryad_action() 
 { 
    document.form1.submit(); 
 } 
     
 function actordel_action() 
 { 
 	 	var pwd = prompt("请输入密码","");
    if ( pwd != null && pwd != "" )
    {
      if (pwd=="xjdmg")
      {
        document.form1.submit();
        return true;
      }
      else
      {
        alert("错误！");
        return false;
      }
    }
    return false;
 }
 
 function pay_action() 
 { 
 	 	var pwd = prompt("请输入密码","");
    if ( pwd != null && pwd != "" )
    {
      if (pwd=="119")
      {
        document.form1.submit();
        return true;
      }
      else
      {
        alert("错误！");
        return false;
      }
    }
    return false;
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

<form id='form1' name='form1' method='post' action='awardad_action.php' target='_blank'>
<?php

$g_itemkind = array(0=>'无');

function getdbdatalist( $db )
{
	Global $g_itemkind;
	$result = 0;

	$sql="select itemkind,name from item where used = 1 order by itemkind";
	$query = $db->query($sql);
	while($row = $db->fetch_row($query))
	{
		$g_itemkind[$row[0]] = $row[1];
	}
	return $result;
}

$db2 = new dbstuff;
$db2->connect( $data_dbh, $data_dbu, $data_dbp, $data_dbd, 0 );
getdbdatalist( $db2 );
$db2->close();

function getdbdatalist_tech( $db )
{
	Global $g_techkind;
	$result = 0;

	$sql="select kind, name from city_tech;";
	$query = $db->query($sql);
	while($row = $db->fetch_row($query))
	{
		$g_techkind[$row[0]] = $row[1];
	}
	return $result;
}

$db2 = new dbstuff;
$db2->connect( $data_dbh, $data_dbu, $data_dbp, $data_dbd, 0 );
getdbdatalist_tech( $db2 );
$db2->close();

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo"<th class='DataHeader'>选项</th>";
echo"<th class='DataHeader'>角色ID</th>";
echo"<th class='DataHeader'>角色名字</th>";
echo"<th class='DataHeader'>角色等级</th>";
echo"<th class='DataHeader'>在线</th>";
echo"<th class='DataHeader'>锁定</th>";
echo"<th class='DataHeader'>禁言</th>";
echo"<th class='DataHeader'>用户ID</th>";
echo"<th class='DataHeader'>平台</th>";
echo"<th class='DataHeader'>平台uid</th>";
echo"<th class='DataHeader'>渠道</th>";
echo"<th class='DataHeader'>建号时间</th>";
echo"<th class='DataHeader'>下线时间</th>";
echo"<th class='DataHeader'>删除</th>";
echo"<th class='DataHeader'>总充值</th>";
echo"<th class='DataHeader'>当前钻石</th>";
echo"<th class='DataHeader'>登陆IP</th>";
echo"<th class='DataHeader'>设备</th>";
echo"<th class='DataHeader'>系统</th>";
echo"<th class='DataHeader'>网络</th>";
echo"<th class='DataHeader'>查看城池</th>";
echo"<th class='DataHeader'>查看道具</th>";
echo"<th class='DataHeader'>行为LOG</th>";
echo"<th class='DataHeader'>上下线LOG</th>";
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
			
			echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>";
			$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );


			if( $hasactorid == 1 )
			{
				$actor_array = explode( ",", $actorid );
				$count = count($actor_array);
				if ( $count > 1 )
				{
					$sql="select a.actorid,a.name,a.level,a.online,a.lock_time,b.forbidtime,a.userid,a.platid,a.username,a.channelid,from_unixtime(a.create_time),from_unixtime(a.logout_time),a.offset,b.charge_dollar,b.token,b.lastip,a.devdata from actor_list a,actor b where (";
					
					for ( $tmpi=0; $tmpi < $count-1; $tmpi++ )
					{
						$i = $actor_array[$tmpi];
						$sql .= "a.actorid = '$i' or ";
					}
					
					$i = $actor_array[$count-1];
					$sql .= "a.actorid = '$i' ";
					$sql .= ")and a.actorid=b.actorid order by a.online desc limit 100;";
				}
				else
				{
					$sql="select a.actorid,a.name,a.level,a.online,a.lock_time,b.forbidtime,a.userid,a.platid,a.username,a.channelid,from_unixtime(a.create_time),from_unixtime(a.logout_time),a.offset,b.charge_dollar,b.token,b.lastip,a.devdata from actor_list a,actor b where a.actorid = '$actorid' and a.actorid=b.actorid order by a.online desc limit 100;";
				}
			}
			else if ( $proc_type == -10 )
			{ // 根据创建时间
				$sql="select a.actorid,a.name,a.level,a.online,a.lock_time,b.forbidtime,a.userid,a.platid,a.username,a.channelid,from_unixtime(a.create_time),from_unixtime(a.logout_time),a.offset,b.charge_dollar,b.token,b.lastip,a.devdata from actor_list a,actor b where a.create_time >='$begintimestamp' and a.create_time <='$endtimestamp' and a.actorid=b.actorid order by a.online desc ,a.level desc";
			}
			else if ( $proc_type == -11 )
			{ // 根据等级
				$sql="select a.actorid,a.name,a.level,a.online,a.lock_time,b.forbidtime,a.userid,a.platid,a.username,a.channelid,from_unixtime(a.create_time),from_unixtime(a.logout_time),a.offset,b.charge_dollar,b.token,b.lastip,a.devdata from actor_list a,actor b where a.level = '$systalk_info' and a.actorid=b.actorid order by a.online desc, a.create_time desc limit 200;";
			}
			else if ( $proc_type == -13 )
			{ // 根据充值
				$sql="select a.actorid,a.name,a.level,a.online,a.lock_time,b.forbidtime,a.userid,a.platid,a.username,a.channelid,from_unixtime(a.create_time),from_unixtime(a.logout_time),a.offset,b.charge_dollar,b.token,b.lastip,a.devdata from actor_list a,actor b where a.actorid=b.actorid order by b.charge_dollar desc limit 200;";
			}
			else
			{ // 根据等级
				$sql="select a.actorid,a.name,a.level,a.online,a.lock_time,b.forbidtime,a.userid,a.platid,a.username,a.channelid,from_unixtime(a.create_time),from_unixtime(a.logout_time),a.offset,b.charge_dollar,b.token,b.lastip,a.devdata from actor_list a,actor b where a.name like '%$systalk_info%' and a.actorid=b.actorid order by a.online desc ,a.level desc limit 100;";
			}
			
			$query = $db->query($sql);
			while($row = $db->fetch_row($query))
			{
				$actorid_q=$row[0];
				$name=$row[1];
				$level=$row[2];
				$online=$row[3];
				$locktime=$row[4];
				$forbidtime=$row[5];
				$userid=$row[6];
				$platid=$row[7];
				$username=$row[8];
				$channelid=$row[9];
				$createtime=$row[10];
				$logouttime=$row[11];
				$offset=$row[12];
				$charge_dollar=$row[13];
				$token=$row[14];
				$lastip=$row[15];
				$devdata=$row[16];
				if( $online == 1 )
					$recno = 1;
				else
					$recno = 2;
					
				// 转移进度专用
				$replace_userid = $userid;
				$replace_username = $username;
				$replace_serverid = $array[$i];
					
				echo"<tr class='Record_$recno'>";
			
				echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysip:$sysport|$actorid_q' ></td>";
				echo"<td class='Data DataRightAlign'>$actorid_q</td>";
				echo"<td class='Data DataRightAlign'>$name</td>";
				echo"<td class='Data DataRightAlign'>$level</td>";
				echo"<td class='Data DataRightAlign'>$online</td>";
				
				$lockdesc = "0";
				if ( $locktime == -1 )
					$lockdesc="永久";
				else if ( $locktime > time() )
					$lockdesc=date("Y-m-d H:i:s", $locktime) ;
				echo"<td class='Data DataRightAlign'>$lockdesc</td>";
				
				$forbiddesc = "0";
				if ( $forbidtime == -1 )
					$forbiddesc="永久";
				else if ( $forbidtime > time() )
					$forbiddesc=date("Y-m-d H:i:s", $forbidtime) ;
				echo"<td class='Data DataRightAlign'>$forbiddesc</td>";
				
				echo"<td class='Data DataRightAlign'>$userid</td>";
				echo"<td class='Data DataRightAlign'>$platid</td>";
				echo"<td class='Data DataRightAlign'>$username</td>";
				echo"<td class='Data DataRightAlign'>$channelid</td>";
				echo"<td class='Data DataRightAlign'>$createtime</td>";
				echo"<td class='Data DataRightAlign'>$logouttime</td>";
				echo"<td class='Data DataRightAlign'>$offset</td>";
				echo"<td class='Data DataRightAlign'>$charge_dollar</td>";
				echo"<td class='Data DataRightAlign'>$token</td>";
				echo"<td class='Data DataRightAlign'>$lastip</td>";
				
				
				$dev = explode( "|", $devdata );
				echo"<td class='Data DataRightAlign'>$dev[4]</td>";
				echo"<td class='Data DataRightAlign'>$dev[5]</td>";
				echo"<td class='Data DataRightAlign'>$dev[6]</td>";
				
				$curdate = date('Y-m-d');
				echo"<td class='Data DataRightAlign'>";
				echo"<a href=\"javascript:post( 'viewcity.php', {host:'$sysip:$sysport', actorid:'$actorid_q', online:'$online', sysdbh:'$sysdbh', sysdbu:'$sysdbu', sysdbp:'$sysdbp', sysdbd:'$sysdbd'} );\" target='_blank'>城池</a>";
				echo"</td>";
				
				echo"<td class='Data DataRightAlign'>";
				echo"<a href=\"javascript:post( 'viewitem.php', {host:'$sysip:$sysport', actorid:'$actorid_q', online:'$online', sysdbh:'$sysdbh', sysdbu:'$sysdbu', sysdbp:'$sysdbp', sysdbd:'$sysdbd'} );\" target='_blank'>道具</a>";
				echo"</td>";
				
				echo"<td class='Data DataRightAlign'>";
				echo"<a href=\"javascript:post( 'log_actor.php', {actorid:'$actorid_q', sysid:'$sysid', op:'0', path:'0', date:'$curdate',begintime:'00:00:00:', endtime:'23:59:59', page:'0'} );\" target='_blank'>LOG</a>";
				echo"</td>";
				
				echo"<td class='Data DataRightAlign'>";
				echo"<a href=\"javascript:post( 'log_account.php', {actorid:'$actorid_q', sysid:'$sysid', date:'$curdate',begintime:'00:00:00:', endtime:'23:59:59', page:'0'} );\" target='_blank'>ACCOUNT</a>";
				echo"</td>";
				
				echo"</tr>";
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
<tr class='Record_1'>
<td class='Data DataRightAlign'><input type=checkbox onclick=sel('favor[]')>全选</td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>		
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>	
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
<td class='Data DataRightAlign'></td>
</tr>
</table>


<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='Record_2'>
		<th class='DataOfPrimaryKey DataRightAlign'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;指令选择&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</th>
		<td class='Data DataRightAlign'>
	    <select id='command' name='command' onChange='opselect(this)'>
		<option value='0'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;请选择&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</option>
<?php
			if( $hasactorid == 1 )
			{
				$actor_array = explode( ",", $actorid );
				$count = count($actor_array);
				if ( $count == 1 )
				{
					echo "<option value='-99'>重设用户关联</option>";
				}
			}
?>
				<option value='-98'>还原删除</option>
				<option value='4'>领主经验</option>
				<option value='5'>城市资源</option>
	      <option value='6'>钻石</option>
	      <option value='8'>道具</option>
	      <option value='23'>部队</option>
	      <option value='49'>清空伤兵（BUG处理）</option>
	      <option value='50'>清空治疗（BUG处理）</option>
	      <option value='51'>科技</option>
	      <option value='52'>天赋</option>
	      <option value='53'>成就数据</option>
	      <option value='10'>城池操作</option>
	      <option value='43'>建筑立即完成</option>
	      <option value='42'>攻城事件</option>
	      <option value='19'>邮件(文字表方式)</option>
	      <option value='54'>邮件(可编辑文字)</option>
	      <option value='37'>踢下线并锁定</option>
	      <option value='38'>禁言</option>
	      <option value='44'>删除角色</option>
	      <option value='26'>充值接口</option>
	      <option value='56'>改名</option>
	      <option value='60'>卡兵回城</option>
	      <option value='62'>行为礼包推送</option>
	      <option value='72'>版主标识</option>
		</select>
	    </td>
	</tr>
	<tr class='Record_1'>
		<th class='DataOfPrimaryKey DataRightAlign'><div id="proc_value1"></div></th>
		<td class='Data DataRightAlign'>
		<div id="pv1"></div>
	    </td>
	</tr>
	<tr class='Record_2'>
		<th class='DataOfPrimaryKey DataRightAlign'><div id="proc_value2"></div></th>
		<td class='Data DataRightAlign'>
		<div id="pv2"></div>
	    </td>
	</tr>
	<tr class='Record_1'>
		<th class='DataOfPrimaryKey DataRightAlign'><div id="proc_value3"></div></th>
		<td class='Data DataRightAlign'>
		<div id="pv3"></div>
	    </td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><div id="proc_string"></th>
		<td class="Data DataRightAlign">
		<div id="pvs"></div>
		
	    </td>
	</tr>
	<tr class='Record_1'>
		<th class='DataOfPrimaryKey DataRightAlign'> </th>
		<td class='Data DataRightAlign' id='ok'>
		<input type='submit' id='submit1' name='submit1' value='提交' onclick="" />
	    </td>
	</tr>
</table>

</form>

<script language="javascript">
function opselect(obj)
{
	document.getElementById("proc_value1").innerHTML = "无";
	document.getElementById("proc_value2").innerHTML = "无";
	document.getElementById("proc_value3").innerHTML = "无";
	document.getElementById("pv1").innerHTML = "";
	document.getElementById("pv2").innerHTML = "";
	document.getElementById("pv3").innerHTML = "";
	document.getElementById("ok").innerHTML = "<input type='submit' id='submit1' name='submit1' value='提交' onclick='queryad_action()' />";
	document.getElementById("proc_string").innerHTML = "<font color='red'></font>";
		
	switch( obj.value )
	{
	case '-99':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>用户ID</font>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>平台UID</font>";
		document.getElementById("proc_value3").innerHTML = "<font color='red'>所在服务器ID</font>";
		document.getElementById("pv1").innerHTML = 
<?php 
		echo "\"<input type='text' name='proc_value1' size='30' value='$replace_userid'>\";";
?>
		document.getElementById("pv2").innerHTML = 
<?php 
		echo "\"<input type='text' name='proc_value2' size='30' value='$replace_username'>\";";
?>
		document.getElementById("pv3").innerHTML = 
<?php 
		echo "\"<input type='text' name='proc_value3' size='30' value='$replace_serverid'>\";";
?>	
		document.getElementById("proc_string").innerHTML = "<font color='red'>注意：设置之前，角色信息一定复制下来做备份</font>";		
		break;
	case '-98':
	document.getElementById("pv3").innerHTML = 
<?php 
		echo "\"<input type='text' name='proc_value3' size='30' value='$replace_serverid'>\";";
?>
		break;
	case '0':
		break;
	case '4':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>增加经验</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='100'>";
		break;
	case '5':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>资源种类</font>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>资源数量</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='1'>[1]木材</option><option value='2'>[2]粮食</option><option value='3'>[3]铁</option><option value='4'>[4]秘银</option><option value='11'>[11]安全木材</option><option value='12'>[12]安全食物</option><option value='13'>[13]安全铁矿</option><option value='14'>[14]安全秘银</option><option value='6'>[6]联盟荣誉</option><option value='7'>[7]魔神之血</option></select>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='999'>";
		break;
	case '6':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>数量</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='100'>";
		break;
	case '8':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>道具选择</font>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>数量</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_itemkind as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='1'>";
		break;
	case '23':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>兵种</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_CorpsName as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
	
		document.getElementById("proc_value2").innerHTML = "<font color='red'>等级</font>";
		document.getElementById("pv2").innerHTML = 
<?php
	echo "\"<select id='proc_value2' name='proc_value2'>";
foreach($g_CorpsLV as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value2 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("proc_value3").innerHTML = "<font color='red'>数量</font>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value='1'>";
		break;
	case '10':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='-3'>重置天赋</option><option value='-4'>士兵回城</option><option value='-5'>Google5星推送</option></select>";
		break;
	case '19':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>标题</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='6060'>[6060]KOR游戏工作室</option><option value='2050'>[2050]旧版本</option></select>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>内容</font>";
		document.getElementById("pv2").innerHTML = "<select id='proc_value2' name='proc_value2'><option value='6256'>[6256]奖励</option><option value='6257'>[6257]补偿</option><option value='2050'>[2050]旧版本</option></select>";
		document.getElementById("proc_string").innerHTML = "<font color='red'>附件内容（例子：100005,200@1,2@102,3@）：</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff '></textarea>";
		break;
	case '54':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>标题</font>";
		document.getElementById("pv1").innerHTML = "<textarea name='proc_value1' cols='60' rows='4' style='background-color:#ffffff ' maxlength='28'></textarea>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>内容(非英文不要超过80个字)</font>";
		document.getElementById("pv2").innerHTML = "<textarea name='proc_value2' cols='60' rows='4' style='background-color:#ffffff ' maxlength='200'></textarea>";
		document.getElementById("proc_string").innerHTML = "<font color='red'>附件内容（例子：100005,200@1,2@102,3@）：</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff ' maxlength='64'></textarea>";
		break;
	case '37':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>锁定时间（分钟） -1永久禁言 0解除禁言</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='60'>";
		break;
	case '38':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>禁言时间（分钟） -1永久锁定 0解除锁定</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='60'>";
		break;
	case '42':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='0'>触发攻城事件</option><option value='1'>清除攻城事件</option></select>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>事件ID</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value3' size='30' value='1'>";
		break;
	case '43':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>建筑队列</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='0'>免费队列</option><option value='1'>花钱队列</option></select>";
		break;
	case '44':
		document.getElementById("ok").innerHTML = "<input type='button' id='submit1' name='submit1' value='删除' onclick='actordel_action()' />";
		break;
	case '49':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='-1'>全部清空</option></select>";
		break;
	case '50':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='-1'>全部清空</option></select>";
		break;
	case '51':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='-1'>全部清空</option><option value='0'>选择科技</option></select>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>科技</font>";
		document.getElementById("pv2").innerHTML = 
<?php
	echo "\"<select id='proc_value2' name='proc_value2'>";
foreach($g_techkind as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("proc_value3").innerHTML = "<font color='red'>等级</font>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value='1'>";
		break;
	case '52':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='-1'>全部清空</option></select>";
		break;
	case '53':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='-1'>全部清空</option></select>";
		break;
	case '26':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>goodsid</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='1'>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>订单号（有可写，没就不写）</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='0'>";
		document.getElementById("proc_value3").innerHTML = "<font color='red'>价值</font>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value='0'>";
		document.getElementById("ok").innerHTML = "<input type='button' id='submit1' name='submit1' value='确定' onclick='pay_action()' />";
		break;
	case '56':
		document.getElementById("proc_string").innerHTML = "<font color='red'>名称[16个字节（约5个汉字）]，不允许带特殊字符：</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff ' maxlength='16'></textarea>";
		break;
	case '60':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='0'>回城</option></select>";
		break;
	case '62':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>goodsid</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='0'>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>持续时长(秒)</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='60'>";
		document.getElementById("proc_value3").innerHTML = "<font color='red'>限购次数</font>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value='1'>";
		break;
	case '72':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='1'>设为版主</option><option value='0'>取消版主</option></select>";
		break;
	default:
		break;
	}
}

	opselect(document.getElementById('command'));
</script>
</div>
</body>
</html>
