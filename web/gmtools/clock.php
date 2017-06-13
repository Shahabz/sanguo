<?php 
require_once 'db.inc.php';
require_once 'define.php';
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


<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<script language="javascript">
function checkdata1()
{
	var testvalue = document.forms[0].indata.value;
	return false;
	if (!testvalue.match(/^(\S)+[(\s)|(\w)|(\W)]+(\S)$/)||!testvalue.match(/^[^$%^&]+$/))
	{
		alert ("false");
		return false;
	}
	return false;
}

function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
}
function checkdata()
{
	var testvalue = document.forms[1].indata.value;
	if (!testvalue.match(/^[0-9\*\/\-\,]+ [0-9\*\/\-\,]+ [0-9\*\/\-\,]+ [0-9\*\/\-\,]+ [0-9\*\/\-\,]+$/))
	{
		alert ("时间格式输入有误，请仔细检查！");
		return false;
	}
	return true;
}

</script>
</head>

<body>
<a href="clock2.php" type=_blank>操作</a><br>
		<font color=red><b>特别注意：星期的范围为0~6之间，星期日的取值不能用7，必需用0</b></font>

<?php
echo "<form id='form2' name='form2' method='post' action='clock.php'>";

$systalk_cond=$_POST["cond"];
$proc_string=$_POST["proc_string"];

$command=$_POST["command"];
$proc_value1=$_POST["proc_value1"];
$proc_value2=$_POST["proc_value2"];
$proc_value3=$_POST["proc_value3"];
$proc_value4=$_POST["proc_value4"];
$timestr=$_POST["indata"];
$submit=$_POST["submit"];


$db = new dbstuff;
$pconnect = 0;

$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

if( $submit == "添加" )
{
	$sql="INSERT INTO command_set (cond,cmd,value1,value2,value3,value4,strvalue,timestr,addtime,deleteflag) values('$systalk_cond','$command','$proc_value1','$proc_value2','$proc_value3','$proc_value4','$proc_string','$timestr',now(),0)";
	$db->query($sql);
	$db->close();
	exit("<script language='javascript'>window.location.href='clock.php';</script>");
}
elseif( $submit == "删除" )
{
	if( isset($_POST["favor"]) )
	{
		$array = $_POST["favor"];
		$num = count($array);           //计算一共有多少项被选择
	
		for ( $i = 0; $i < $num; $i++ )
		{
			if(isset($array[$i]))
			{
				$sql="UPDATE command_set set deleteflag='1' WHERE id='$array[$i]'";
				$db->query($sql);
			}
		}
	}
	$db->close();

	exit("<script language='javascript'>window.location.href='clock.php';</script>");
}

$sql="SELECT * FROM command_set where deleteflag=0";
$query = $db->query($sql);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>选项</th><th class='DataHeader'>内容</th><th class='DataHeader'>时间</th><th class='DataHeader'>条件</th></tr>";

$recno = 1;
while($row = $db->fetch_row($query))
{
	$sysid=$row[0];
	$syscond=$row[1];
	$syscmd=$row[2];
	$sysvalue1=$row[3];
	$sysvalue2=$row[4];
	$sysvalue3=$row[5];
	$sysvalue4=$row[6];
	$sysstrvalue=$row[7];
	$systimestr=$row[8];


	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;

	$title = "";
	if( $syscmd == 18 )
	{
		$title = "滚动公告：$sysstrvalue";
	}
	else if( $syscmd == 20 )
	{
		$title = "活动：".$g_actname[$sysvalue1];
		if( $sysvalue2 > 0 )
		{
			$title .= "&nbsp;持续时间：".$sysvalue2."分钟"."&nbsp参数:".$sysvalue3."&nbsp参数:".$sysvalue4;
		}
	}
	else if( $syscmd == 2 )
	{
		$title = "执行脚本：".$g_scname[$sysvalue1];
	}
	else if( $syscmd == 48 )
	{
		$title = "存档：";
	}
	else
	{
		$title = "$syscmd|$sysvalue1|$sysvalue2|$sysvalue3|$sysvalue4|$sysstrvalue";
	}
	
	echo"<tr class='Record_$recno'>";

	echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysid' ></td>";
//	echo"<td class='Data DataLeftAlign'><div style='width:700px;overflow: hidden;height:14px;' title='$title' onmouseover='this.style.width=\"\"' onmouseout='this.style.width=\"700px\"'>$title</div></td>";
	echo"<td class='Data DataLeftAlign'><div style='width:700px;overflow: hidden;height:14px;' title='$title' >$title</div></td>";
	echo"<td class='Data DataLeftAlign'>$systimestr</td>";
	echo"<td class='Data DataLeftAlign'>  </td>";

	echo"</tr>";
}
echo"<tr class='TableHeader'>";
echo"<td class='Data DataRightAlign'><input type=checkbox onclick=sel('favor[]')></td>";
echo"<td class='Data DataLeftAlign'>全选</td>";
echo"<td class='Data DataLeftAlign'> </td>";
echo"<td class='Data DataLeftAlign'><input type=\"submit\" id=\"submit\" name=\"submit\" value=\"删除\" /></td>";
echo"</table>";

$db->close();

echo"  	</form>";
?>

<form id='form1' name='form1' method='post' action='clock.php' onsubmit="return checkdata()" >

<p></p>
<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >

	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">指令选择</th>
		<td class="Data DataRightAlign">
	    <select id='command' name='command' onChange='opselect(this)'>
		<option value='18'>滚动公告</option>
		<option value='20' selected>活动启动</option>
		<option value='2'>脚本执行</option>
		<option value='48'>存档</option>
		</select>
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"><div id="proc_value1"></div></th>
		<td class="Data DataRightAlign">
			<div id="pv1"></div>
	    </td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><div id="proc_value2"></div></th>
		<td class="Data DataRightAlign">
			<div id="pv2"></div>
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"><div id="proc_value3"></div></th>
		<td class="Data DataRightAlign">
			<div id="pv3"></div>
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"><div id="proc_value4"></div></th>
		<td class="Data DataRightAlign">
			<div id="pv4"></div>
	    </td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><div id="proc_string"></div></th>
		<td class="Data DataRightAlign">
		<div id="pvstr"></div>
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign">执行条件</th>
		<td class="Data DataRightAlign">
	    <select id="cond" name="cond">
		<option value="0">无</option>
	    </select>
	    </td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">定时时间</th>
		<td class="Data DataRightAlign">
		<input type="text" id="indata" name="indata" size="20" value="* * * * *" style="text-align: right" />
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"> </th>
		<td class="Data DataRightAlign">
		<input type="submit" id="submit" name="submit" value="添加" />
	    </td>
	</tr>
</table>
		(分) (时) (日) (月) (星期) 如：1 19-7/2,8 * * * 表示每天的晚19点到第二天7点间隔2小时的每个01分的时间加上在8点01分执行<br>

  	
  </p>
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
	document.getElementById("pv4").innerHTML = "";
	document.getElementById("pvstr").innerHTML = "";
	switch( obj.value )
	{
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
		document.getElementById("pvstr").innerHTML = "<textarea name='proc_string' cols='60' rows='4' style='background-color:#ffffff '></textarea>";
		break;
	case '20':
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
		document.getElementById("proc_value2").innerHTML = "<font color='red'>持续时间</font>";
		document.getElementById("pv2").innerHTML = "<input type='text' name='proc_value2' size='30' value='0' style='text-align: right'>(分钟)";

		document.getElementById("proc_value3").innerHTML = "<font color='red'>额外参数</font>";
		document.getElementById("pv3").innerHTML = "<input type='text' name='proc_value3' size='30' value='0' style='text-align: right'>";
		document.getElementById("proc_value4").innerHTML = "<font color='red'>额外参数</font>";
		document.getElementById("pv4").innerHTML = "<input type='text' name='proc_value4' size='30' value='0' style='text-align: right'>";
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
	default:
		break;
	}
}

	opselect(document.getElementById('command'));
</script>


</body>
</html>