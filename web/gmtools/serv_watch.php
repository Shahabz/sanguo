<?php 
require_once 'db.inc.php';
error_reporting(0);
/*session方式*/
session_start();
//header("content-Type: text/html; charset=gbk");

if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}
if( $_SESSION["adminid"] < 100 )
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
		.btn{position: relative;overflow: hidden;margin-right: 1px;display:inline-block;*display:inline;padding:1px 16px 1px;line-height:13px;*line-height:16px;color:#fff;text-align:center;vertical-align:middle;cursor:pointer;background-color:#808080;border:1px solid #cccccc;border-color:#e6e6e6 #e6e6e6 #bfbfbf;border-bottom-color:#b3b3b3;-webkit-border-radius:1px;-moz-border-radius:1px;border-radius:1px;}
		.btn input {position: absolute;top: 0; right: 0;margin: 0;border: solid transparent;opacity: 0;filter:alpha(opacity=0); cursor: pointer;}
		.progress { position:relative; margin-left:1px; margin-top:1px; width:200px;padding: 1px; border-radius:1px; display:none}
		.bar {background-color:#FFCC66; display:block; width:0%; height:16px; border-radius: 1px; }
		.percent { position:absolute; height:10px; display:inline-block; top:1px; left:2%; color:#FF6633 }
		.files{height:12px; line-height:12px; margin:1px 0}
		.RedText {color:#ff0000 ;text-shadow: 2px 2px 5px #999;}
		.GreenText {color:#008800 ;text-shadow: 2px 2px 5px #999;}
		.BlueText {color:#0000ee ;text-shadow: 2px 2px 5px #999;}
		.PurpleText {color:#ee00ee ;text-shadow: 2px 2px 5px #999;}
	--></style>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<!-- <meta http-equiv="refresh" content="30"; charset=gbk"> -->
<script language="javascript">

function sel( a, tmpi )
{
	obj=document.getElementsByName(a)
	if( tmpi<obj.length )
	{
		if( event.srcElement.checked )
			obj[tmpi].value=1;
		else
			obj[tmpi].value=0;
	}
}

</script>
</head>

<body>
<form id='addform' name='addform' method='post' action='serv_watch_action.php'>
<table border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'>
	<th class='DataHeader'>服务器名称</th>
	<th class='DataHeader'>IP地址</th>
	<th class='DataHeader'>端口</th>
	<th class='DataHeader'></th></tr>
	<tr class='TableHeader'>
	<td class='Data DataRightAlign'><input name='name' size='12' type='text'></td>
	<td class='Data DataRightAlign'><input name='ip' size='12' type='text' value='0.0.0.0'></td>
	<td class='Data DataRightAlign'><input name='port' size='6' type='text' value='10001'></td>
	<td class='Data DataRightAlign'><input type="submit" id="submit" name="submit" value="添加到监控列表" /></td>
	</tr>
</table>
</form>
<br>

<br>
<form id='form1' name='form1' method='post' action='serv_watch.php'>

<?php

if ( isset($_POST["favor"]) )
{
	$lines = file( $hostlist_file );

	$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
	$servidarr = $_POST["serverid"];


	$fp= fopen( $hostlist_file, "w" );

	$i = 0;
	foreach ($lines as $line)
	{
		if(isset($array[$i]) && isset($servidarr[$i]))
		{
			if( $array[$i] == 0 )
			{
				fwrite($fp, $line);
			}
		}
		$i++;
		echo "<br>";
	}
	fclose($fp);
	exit("<script language='javascript'>window.location.href='serv_watch.php';</script>");
}

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>删除</th><th class='DataHeader'>行号</th><th class='DataHeader'>名称</th><th class='DataHeader'>IP</th><th class='DataHeader'>端口</th></tr>";

$fp= fopen( $hostlist_file, "r" );

$recno = 2;
$ip_arr = array();
$id_arr = array();
$index = 0;

while (!feof($fp))
{
	if( $recno == 2 )
		$recno = 1;
	else
		$recno = 2;

	$line = fgets($fp);
	$arr = explode( "|", $line );
	$strarr = explode(" ",$arr[0]);
	if ( !isset($arr[1]) )
		break;
	$sysname=$arr[1];
	$sysip=$strarr[0];
	$sysport=$strarr[1];
	$sysid = $index;

	echo"<tr class='Record_$recno'>";

	echo"<td class='Data DataRightAlign'><input name='checkctl[]' type='checkbox' onclick=sel('favor[]','$index')>";
	echo"<input name='serverid[]' type='hidden' value='$sysid' ></td>";
	echo"<input name='favor[]' type='hidden' value='0' ></td>";


	echo"<td class='Data DataRightAlign'>$sysid</td>";
	echo"<td class='Data DataRightAlign'>$sysname</td>";
	echo"<td class='Data DataRightAlign'>$sysip</td>";
	echo"<td class='Data DataRightAlign'>$sysport</td>";
	
	echo"<input name='ip_arr[]' type='hidden' value='$sysip' >";
	echo"<input name='id_arr[]' type='hidden' value='$sysid' >";
	echo"<input name='sysname_arr[]' type='hidden' value='$sysname' >";

	$ip_arr[] = $sysip;
	$id_arr[] = $sysid;
	
	echo"</tr>";
	$index++;
}
fclose($fp);

echo"<tr class='TableHeader'>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'><input type='submit' id='submit' name='submit' value='提交' /></td>";
echo"</table>";
$db->close();
?>
<br>

</form>
<br>

<br><br><br><br><br><br>

</body>
</html>
