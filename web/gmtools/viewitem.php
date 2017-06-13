<?php 
require_once 'db.inc.php';
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

<form id='form1' name='form1' method='post' action='viewitem_action.php'>
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

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo"<th class='DataHeader'>选择</th>";
echo"<th class='DataHeader'>位置</th>";
echo"<th class='DataHeader'>名称</th>";
echo"<th class='DataHeader'>种类</th>";
echo"<th class='DataHeader'>数量</th>";
echo"<th class='DataHeader'>颜色</th>";
echo "<tr class='Record_2'><th class='DataHeader'>$actorid</th><td></td><td></td><td></td><td></td><td></td></tr>";

$db = new dbstuff;
$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );
$sql="select itemid, offset, kind, num, color_level from actor_item where actorid = '$actorid' order by offset desc;";
$query = $db->query($sql);
while($row = $db->fetch_row($query))
{
		$itemid=$row[0];
		$offset=$row[1];
		$kind=$row[2];
		$num=$row[3];
		$color_level=$row[4];
		$recno = 2;
		if( $offset >= 1000 )
			$recno = 1;
		else
			$recno = 2;
		echo"<tr class='Record_$recno'>";
		echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$kind' ></td>";
		echo"<td class='Data DataRightAlign'>$offset</td>";
		echo"<td class='Data DataRightAlign'>$g_itemkind[$kind]</td>";
		echo"<td class='Data DataRightAlign'>$kind</td>";
		echo"<td class='Data DataRightAlign'>$num</td>";
		echo"<td class='Data DataRightAlign'>$color_level</td>";
		echo"</tr>";
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
</tr>
</table>


<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='Record_2'>
		<th class='DataOfPrimaryKey DataRightAlign'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;指令选择&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</th>
		<td class='Data DataRightAlign'>
	    <select id='command' name='command' onChange='opselect(this)'>
		<option value='0'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;请选择&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</option>
		<option value='0'>无</option>
		<option value='61'>删除</option>
		</select>
	    </td>
	</tr>
	
	<input type='hidden' id='server' name='server' value="<?php echo $host ?>"/>
	<input type='hidden' id='actorid' name='actorid' value="<?php echo $actorid ?>"/>
	
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
	case '61':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>数量（0全部删除）</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='1'>";
<?php		
		if ( $online == 0 )
		{
?>		
			document.getElementById("ok").innerHTML = "<input type='button' id='submit1' name='submit1' value='不在线不能用' />";
<?php	
		}

?>
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
