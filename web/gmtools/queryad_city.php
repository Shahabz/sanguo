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
if(isset($actorid) && $actorid > 0 )
{
	$hasactorid = 1;
}
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

<form id='form1' name='form1' method='post' action='awardad_action.php'>
<?php

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo"<th class='DataHeader'>选项</th>";
echo"<th class='DataHeader'>城池ID</th>";
echo"<th class='DataHeader'>城池名字</th>";
echo"<th class='DataHeader'>城池位置</th>";

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
			
			echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td><td></td></tr>";
			$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );


			if( $hasactorid == 1 )
			{
				$sql="select cityid, laird_name, posx, posy from city where cityid = '$actorid';";
			}
			else
			{
				$sql="select cityid, laird_name, posx, posy from city where laird_name like '%$systalk_info%' limit 100;";
			}
			$query = $db->query($sql);
			while($row = $db->fetch_row($query))
			{
				$cityid=$row[0];
				$laird_name=$row[1];
				$posx=$row[2];
				$posy=$row[3];
			
				echo"<tr class='Record_2'>";
				echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysip:$sysport|$cityid' ></td>";
				echo"<td class='Data DataRightAlign'>$cityid</td>";
				echo"<td class='Data DataRightAlign'>$laird_name</td>";
				echo"<td class='Data DataRightAlign'>$posx</td>";
				echo"<td class='Data DataRightAlign'>$posy</td>";
				
				/*echo"<td class='Data DataRightAlign'>";
				echo"<a href=\"javascript:post( 'viewclub.php', {host:'$sysip:$sysport', clubid:'$clubid', sysdbh:'$sysdbh', sysdbu:'$sysdbu', sysdbp:'$sysdbp', sysdbd:'$sysdbd'} );\" target='_blank'>详情</a>";
				echo"</td>";
				
				echo"<td class='Data DataRightAlign'>";
				echo"<a href=\"javascript:post( 'log_club.php', {clubid:'$clubid', sysid:'$sysid', op:'0', path:'0', date:'2016-10-20',begintime:'00:00:00:', endtime:'23:59:59', page:'0'} );\" target='_blank'>LOG</a>";
				echo"</td>";*/
				
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
</tr>
</table>


<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='Record_2'>
		<th class='DataOfPrimaryKey DataRightAlign'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;指令选择&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</th>
		<td class='Data DataRightAlign'>
	    <select id='command' name='command' onChange='opselect(this)'>
				<option value='0'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;请选择&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</option>
				<option value='39'>立即完成当前科技</option>
				<option value='40'>设置当前科技</option>
				<option value='41'>龙技捐献</option>
				<option value='57'>改名</option>
				<option value='58'>改公告</option>
				<option value='59'>该职位名</option>
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
		<td class='Data DataRightAlign'>
		<input type='submit' id='submit' name='submit' value='提交' />
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

	switch( obj.value )
	{
	case '0':
		break;
	case '39':
		break;
	case '40':
	document.getElementById("proc_value1").innerHTML = "<font color='red'>科技选择</font>";
	document.getElementById("pv1").innerHTML = 
<?php
		echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_clubtechkind as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>

		document.getElementById("proc_value2").innerHTML = "<font color='red'>等级</font>";
		document.getElementById("proc_value3").innerHTML = "<font color='red'>星级</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='1'>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value='1'>";
		break;
		case '41':
	document.getElementById("proc_value1").innerHTML = "<font color='red'>龙技选择</font>";
	document.getElementById("pv1").innerHTML = 
<?php
		echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_clubdragon as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>

		document.getElementById("proc_value2").innerHTML = "<font color='red'>捐献数量</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='1'>";
		break;
	case '57':
		document.getElementById("proc_string").innerHTML = "<font color='red'>名称[14个英文]，不允许其它语言：</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff ' maxlength='14'></textarea>";
		break;
	case '58':
		document.getElementById("proc_string").innerHTML = "<font color='red'>公告[64个字节]：</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff ' maxlength='64'></textarea>";
		break;
	case '59':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>职位</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='0'>R1</option><option value='1'>R2</option><option value='2'>R3</option><option value='3'>R4</option><option value='4'>R5</option></select>";
		document.getElementById("proc_string").innerHTML = "<font color='red'>职位名称[16个字节]：</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff ' maxlength='16'></textarea>";
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
