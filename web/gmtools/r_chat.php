<?php 
require_once 'db.inc.php';
require_once 'define.php';
error_reporting(1);
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
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
</head>

<script language="javascript">
 function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
 }
</script>
<body>
<div width="980" align="center">
<form id='form1' name='form1' method='post' action='r_action.php' target='_blank'>
<?php

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo"<th class='DataHeader'>选项</th>";
echo"<th class='DataHeader'>服务器ID</th>";
echo"<th class='DataHeader'>城池ID</th>";
echo"<th class='DataHeader'>头像ID</th>";
echo"<th class='DataHeader'>角色ID</th>";
echo"<th class='DataHeader'>名称</th>";
echo"<th class='DataHeader'>VIP等级</th>";
echo"<th class='DataHeader'>内容</th>";
echo"<th class='DataHeader'>被举报次数</th>";

$recno = 2;
		
$db = new dbstuff;
$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );
		
$sql="select id, serverid, cityid, headid, actorid, name, vip, msg, sign, count(*) from report_chat group by sign limit 50;";
$query = $db->query($sql);
while($row = $db->fetch_row($query))
{
	$id=$row[0];
	$serverid=$row[1];
	$cityid=$row[2];
	$headid=$row[3];
	$actorid=$row[4];
	$name=$row[5];
	$vip=$row[6];
	$msg=$row[7];
	$sign=$row[8];
	$count=$row[9];
	
	echo"<tr class='Record_$recno'>";
	echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sign|$serverid|$cityid|$actorid' ></td>";
	echo"<td class='Data DataRightAlign'>$serverid</td>";
	echo"<td class='Data DataRightAlign'>$cityid</td>";
	echo"<td class='Data DataRightAlign'>$headid</td>";
	echo"<td class='Data DataRightAlign'>$actorid</td>";
	echo"<td class='Data DataRightAlign'>$name</td>";
	echo"<td class='Data DataRightAlign'>$vip</td>";
	echo"<td class='Data DataRightAlign'>$msg</td>";		
	echo"<td class='Data DataRightAlign'>$count</td>";	
	echo"</tr>";
	
	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;
}
$db->close();
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
				<option value='-1'>不处理或删除已经处理过的</option>
	      <option value='19'>邮件警告</option>
	      <option value='37'>踢下线并锁定</option>
	      <option value='38'>禁言</option>
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
	document.getElementById("proc_string").innerHTML = "";
	document.getElementById("pvs").innerHTML = "";
	
	switch( obj.value )
	{
	case '0':
		break;
	case '-1':
		break;
	case '19':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>标题</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='6060'>[6060]KOR游戏工作室</option></select>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>内容</font>";
		document.getElementById("pv2").innerHTML = "<select id='proc_value2' name='proc_value2'><option value='6278'>[6278]聊天警告</option><option value='6277'>[6277]头像警告</option></select>";
		document.getElementById("proc_string").innerHTML = "<font color='red'>附件内容（例子：100005,200@1,2@102,3@）：</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff '></textarea>";
		break;
	case '37':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>锁定时间（分钟） -1永久禁言 0解除禁言</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='60'>";
		break;
	case '38':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>禁言时间（分钟） -1永久锁定 0解除锁定</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='60'>";
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
