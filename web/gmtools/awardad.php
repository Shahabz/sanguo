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
function checkconfirm()
{
	var info = document.forms[0].systalk_info.value;
	var r=confirm(info+"\n要立即提交吗？");

	return r;
}

 function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
 }
</script>
</head>

<body align="center">
<div width="980" align="center">

<a href="queryad.php" type=_blank>查询玩家</a>
<?php

$g_townname = array(0=>'无');
function getdbdatalist( $db )
{
	Global $g_townname;
	$result = 0;
	$sql="select townid, name, posx, posy from map_town_cfg";
	$query = $db->query($sql);
	while($row = $db->fetch_row($query))
	{
		$g_townname[$row[0]] = $row[1];
		$g_townname[$row[0]] .= "($row[2],$row[3])";
	}
	return $result;
}
$db2 = new dbstuff;
$db2->connect( $data_dbh, $data_dbu, $data_dbp, $data_dbd, 0 );
getdbdatalist( $db2 );
$db2->close();

echo "<form id='form1' name='form1' method='post' action='awardad_action.php' onsubmit='return checkconfirm()' >";

$db = new dbstuff;
$pconnect = 0;
$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="SELECT * FROM server_list where id > 100 order by id";
$query = $db->query($sql);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>选项</th><th class='DataHeader'>服务器ID</th><th class='DataHeader'>服务器名称</th><th class='DataHeader'>服务器IP</th></tr>";
$recno = 2;
$firstchecked = 0;
while($row = $db->fetch_row($query))
{
	$sysid=$row[0];
	$sysname=$row[1];
	$sysip=$row[2];
	$sysport=$row[3];

	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;
	echo"<tr class='Record_$recno'>";
	if( $firstchecked == 0 )
		echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysip:$sysport|0' checked></td>";
	else
		echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysip:$sysport|0'></td>";
	echo"<td class='Data DataRightAlign'>$sysid</td>";
	echo"<td class='Data DataRightAlign'>$sysname</td>";
	//echo"<td class='Data DataRightAlign'>$sysip:$sysport</td>";
	echo"<td class='Data DataRightAlign'>0.0.0.0</td>";
	echo"</tr>";
	$firstchecked = 1;
}
echo"<tr class='TableHeader'>";
echo"<td class='Data DataRightAlign'><input type=checkbox onclick=sel('favor[]')></td>";
echo"<td class='Data DataRightAlign'>全选</td>";
echo"<td class='Data DataLeftAlign'> </td>";
echo"<td class='Data DataLeftAlign'> </td>";
echo"</table>";
?>             

<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">指令选择</th>
		<td class="Data DataRightAlign">
	    <select id="command" name="command" onChange="opselect(this)">
	      <option value="18">滚动公告</option>
	      <option value="12">战斗测试</option>
	      <option value="14">表数据重读</option>
	      <option value="9">重载Lua</option>
	      <option value="2">脚本执行</option>
	      <option value="24">紧急关闭活动</option>
	      <option value="63">开启关闭系统</option>
	      <option value="27">登陆队列</option>
	      <option value="46">城镇保护时间</option>
	      <option value="16">城镇状态</option>
	      <option value="45">城镇逻辑</option>
	      <option value="47">城镇立即攻击</option>
	      <option value="29">机器人</option>
	      <option value="30">全服邮件</option>
	      <option value="31">排行榜刷新</option>
	      <option value="32">所有城市buff</option>
	      <option value="36">所有城市重置天赋</option>
	      <option value="69">迁城</option>
	     	<option value="66">推送礼包添加</option>
	      <option value="67">推送礼包删除</option>
	      <option value="65">位面</option>
	      <option value="48">存档</option>
	    </select>
	    </td>
	</tr>
	<tr class='Record_2'>
		<th class='DataOfPrimaryKey DataRightAlign'><div id="proc_value1"></div></th>
		<td class='Data DataRightAlign'>
		<div id="pv1"></div>
	    </td>
	</tr>
	<tr class='Record_1'>
		<th class='DataOfPrimaryKey DataRightAlign'><div id="proc_value2"></div></th>
		<td class='Data DataRightAlign'>
		<div id="pv2"></div>
	    </td>
	</tr>
	<tr class='Record_2'>
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
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"> </th>
		<td class="Data DataRightAlign">
		<input type="submit" id="submit" name="submit" value="提交" />
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
	document.getElementById("proc_string").innerHTML = "无";
	document.getElementById("pv1").innerHTML = "";
	document.getElementById("pv2").innerHTML = "";
	document.getElementById("pv3").innerHTML = "";
	document.getElementById("pvs").innerHTML = "";

	switch( obj.value )
	{
	case '0':
		break;
	case '18':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>公告类型</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='0'>直接发文字</option><option value='1'>发文字表ID</option>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>语言（直接发文字时候有用）</font>";
		document.getElementById("pv2").innerHTML = 
<?php
	echo "\"<select id='proc_value2' name='proc_value2'>";
foreach($g_language as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value2 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>

	document.getElementById("proc_value3").innerHTML = "<font color='red'>渠道（只针对国内）</font>";
	document.getElementById("pv3").innerHTML = 
<?php
	echo "\"<select id='proc_value3' name='proc_value3'>";
foreach($g_channeid as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value3 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("proc_string").innerHTML = "<font color='red'>公告内容：</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff '></textarea>";
		break;
	case '12':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>攻击方enemyid</font>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>防御方enemyid</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='1'>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='2'>";
		break;
	case '14':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择系统</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='1'>[1]全局表</option><option value='4'>[4]奖励表</option><option value='10'>[10]shop</option><option value='26'>[26]world_data</option><option value='12'>[12]paygoods</option><option value='17'>[17]item</option><option value='18'>[18]锻造</option><option value='3'>[3]enemy</option><option value='20'>[20]mission</option><option value='9'>[9]quest</option><option value='21'>[21]robot_base</option><option value='22'>[22]robot_ai</option></select>";
		break;
		break;
	case '9':
		break;
	case '2':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择执行内容</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_scname as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		break;
	case '24':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择活动</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_actname as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>

		break;
	case '16':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择城镇</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_townname as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("proc_value2").innerHTML = "<font color='red'>状态</font>";
		document.getElementById("pv2").innerHTML = "<select id='proc_value2' name='proc_value2'><option value='1'>[1]保护状态</option><option value='2'>[2]破防状态</option><option value='3'>[3]临时占领状态</option><option value='4'>[4]占领状态</option></select>";
		break;
	case '45':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择城镇</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_townname as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("proc_value2").innerHTML = "<font color='red'>逻辑次数</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='1'>";
		break;
		case '46':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择城镇</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_townname as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("proc_value2").innerHTML = "<font color='red'>保护时间分钟(>0增加 <0减少)</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='1'>";
		break;
		case '47':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择城镇</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_townname as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		break;
	case '27':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='-1'>[-1]清空队列</option><option value='0'>[0]关闭排队</option><option value='1'>[1]开启排队</option></select>";
		break;
	case '29':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>数量(1-20000) -3所有AI去掉保护罩 -2主动攻击 -1删除所有ai</font>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>AI编号(1-N)</font>";
		document.getElementById("proc_value3").innerHTML = "<font color='red'>国家ID</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value=''>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value=''>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value=''>";
		document.getElementById("proc_string").innerHTML = "<font color='red'>名字开头(英文字母每批次要不同|0)</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff '></textarea>";
		break;
	case '30':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>标题ID</font>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>正文ID</font>";
		document.getElementById("pv1").innerHTML = "<a href='notice.php' target='_blank'>浏览</a><input type='text' name='proc_value1' size='30' value=''>";
		document.getElementById("pv2").innerHTML = "<a href='notice.php' target='_blank'>浏览</a><input type='text' name='proc_value2' size='30' value=''>";
		document.getElementById("proc_value3").innerHTML = "<font color='red'>只针对语言</font>";
		document.getElementById("pv3").innerHTML = 
<?php
	echo "\"<select id='proc_value3' name='proc_value3'>";
foreach($g_language as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value3 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("proc_string").innerHTML = "<font color='red'>附件内容：（例子：100005,200@1,2@102,3@)</font>";
		document.getElementById("pvs").innerHTML = "<textarea name='pvstring' cols='60' rows='4' style='background-color:#ffffff '></textarea>";
		break;
	case '31':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择排行榜</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_topname as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>

		break;
	case '32':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>选择城市Buff</font>";
		document.getElementById("pv1").innerHTML = 
<?php
	echo "\"<select id='proc_value1' name='proc_value1'>";
foreach($g_buffname as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value1 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
echo "</select>\";";
?>
		document.getElementById("proc_value2").innerHTML = "<font color='red'>时间(秒)</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='1800'>";
		break;
	case '65':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>操作</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='0'>[0]暴动</option></select>";
		break;
	case '66':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>goodsid</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='0'>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>持续时长(秒)</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='60'>";
		document.getElementById("proc_value3").innerHTML = "<font color='red'>限购次数</font>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value='1'>";
		break;
	case '67':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>goodsid</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='0'>";
		break;
	case '69':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>cityid</font>";
		document.getElementById("pv1").innerHTML = "<input type='text' name='proc_value1' size='30' value='0'>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>posx</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='-1'>";
		document.getElementById("proc_value3").innerHTML = "<font color='red'>posy</font>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value='-1'>";
		break;
	case '63':
		document.getElementById("proc_value1").innerHTML = "<font color='red'>系统</font>";
		document.getElementById("pv1").innerHTML = "<select id='proc_value1' name='proc_value1'><option value='1'>[1]位面</option></select>";
		document.getElementById("proc_value2").innerHTML = "<font color='red'>开启关闭</font>";
		document.getElementById("pv2").innerHTML = "<select id='proc_value2' name='proc_value2'><option value='1'>[1]开启</option><option value='0'>[0]关闭</option></select>";
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