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
 function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
 }
</script>
</head>

<body>
<?php
$server_id=$_GET["id"];

echo "<form id='form1' name='form1' method='post' action='clock_one_action.php'>";

$db = new dbstuff;
$pconnect = 0;

$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="SELECT * FROM server_list where id='$server_id'";
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
	
	echo "$sysname<br>";

	$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );

	$sql="SELECT * FROM command_data";
	$query = $db->query($sql);

	echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
	echo "<tr class='TableHeader'><th class='DataHeader'>选项</th><th class='DataHeader'>内容</th><th class='DataHeader'>时间</th><th class='DataHeader'>条件</th></tr>";
	
	$recno = 1;
	while($row = $db->fetch_row($query))
	{
		$id=$row[0];
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
			$title = "存档(必须添加!!!!必须添加!!!!!必须添加!!!!!)：";
		}
		else
		{
			$title = "$syscmd|$sysvalue1|$sysvalue2|$sysvalue3|$sysvalue4|$sysstrvalue";
		}
		
	
		
		echo"<tr class='Record_$recno'>";
	
		echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$id' ></td>";
		echo"<td class='Data DataLeftAlign'>$title</td>";
		echo"<td class='Data DataLeftAlign'>$systimestr</td>";
		echo"<td class='Data DataLeftAlign'>  </td>";
	
		echo"</tr>";
		echo "<input type='hidden' name='id' value='$server_id' />";
	}
	echo"<tr class='TableHeader'>";
	echo"<td class='Data DataRightAlign'><input type=checkbox onclick=sel('favor[]')></td>";
	echo"<td class='Data DataLeftAlign'>全选</td>";
	echo"<td class='Data DataLeftAlign'> </td>";
	echo"<td class='Data DataLeftAlign'> </td>";
	
	echo"<tr class='TableHeader'>";
	echo"<td class='Data DataRightAlign'>　</td>";
	echo"<td class='Data DataLeftAlign'> </td>";
	echo"<td class='Data DataLeftAlign'> </td>";
	echo"<td class='Data DataLeftAlign'> </td>";
	
	echo"<tr class='TableHeader'>";
	echo"<td class='Data DataRightAlign'> </td>";
	echo"<td class='Data DataLeftAlign'><input type=\"submit\" id=\"submit\" name=\"submit\" value=\"删除\" /></td>";
	echo"<td class='Data DataLeftAlign'> </td>";
	echo"<td class='Data DataLeftAlign'> </td>";
	echo"</table>";
	
	$db->close();

}

?>

</form>
</body>
</html>