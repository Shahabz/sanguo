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
		.Record_2 {background-color: #f0b0b0;}
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
<form id='addform' name='addform' method='post' action='serv_list_action.php'>
<table border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'>
	<th class='DataHeader'>ID</th>
	<th class='DataHeader'>服务器名称</th>
	<th class='DataHeader'>IP地址</th>
	<th class='DataHeader'>端口</th>
	<th class='DataHeader'></th></tr>
	<tr class='TableHeader'>
	<td class='Data DataRightAlign'><input name='id' id="id" size='6' type='text'></td>
	<td class='Data DataRightAlign'><input name='name' size='12' type='text'></td>
	<td class='Data DataRightAlign'><input name='ip' size='12' type='text' value='0.0.0.0'></td>
	<td class='Data DataRightAlign'><input name='port' size='6' type='text' value='10001'></td>
	<td class='Data DataRightAlign'><input type="submit" id="submit" name="submit" value="添加到gate_list" /></td>
	</tr>
</table>
</form>
<br>

<br>
<form id='form1' name='form1' method='post' action='serv_list.php'>

<?php

$db = new dbstuff;
$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

if ( isset($_POST["favor"]) )
{
	$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
	$servidarr = $_POST["serverid"];
	$newarr = $_POST["isnew"];
	$weightarr = $_POST["weight"];
	
	$num = count($array);           //计算一共有多少项被选择
	$interest = "";                 //设置一个空变量interest
	for ( $i = 0; $i < $num; $i++ )
	{
		if(isset($array[$i]) && isset($servidarr[$i]) && isset($newarr[$i]) && isset($weightarr[$i]) )
		{
			$server=$servidarr[$i];
			$closed=$array[$i];
			$new=$newarr[$i];
			$weight=$weightarr[$i];
			$sql="update gate_list set close='$closed',`new`='$new',`weight`='$weight' where id='$server'";
			$query = $db->query($sql);
		}
	}
	$db->close();
	exit("<script language='javascript'>window.location.href='serv_list.php';</script>");
}




$sql="SELECT id,name,ip,port,new,usernum,close,weight,tempnum FROM gate_list order by id";
$query = $db->query($sql);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>关闭</th><th class='DataHeader'>ID</th><th class='DataHeader'>名称</th><th class='DataHeader'>地址</th><th class='DataHeader'>是否导入服</th><th class='DataHeader'>导入服倒量权重</th><th class='DataHeader'>权重计数</th><th class='DataHeader'>人数</th></tr>";
$recno = 2;

$ip_arr = array();
$id_arr = array();
$index = 0;
$maxsysid = 1;
while($row = $db->fetch_row($query))
{
	$sysid=$row[0];
	$sysname=$row[1];
	$sysip=$row[2];
	$sysport=$row[3];
	$sysnew=$row[4];
	$usernum=$row[5];
	$closed=$row[6];
	$weight=$row[7];
	$tempnum=$row[8];
	
	if( $sysid >= $maxsysid )
		$maxsysid = $sysid+1;

	if( $closed == 0 )
		$recno = 1;
	else
		$recno = 2;

	echo"<tr class='Record_$recno'>";

	if( $closed == 0 )
	{
		echo"<td class='Data DataRightAlign'><input name='checkctl[]' type='checkbox' onclick=sel('favor[]','$index')>";
		echo"<input name='serverid[]' type='hidden' value='$sysid' ></td>";
		echo"<input name='favor[]' type='hidden' value='0' ></td>";
	}
	else
	{
		echo"<td class='Data DataRightAlign'><input name='checkctl[]' type='checkbox' checked onclick=sel('favor[]','$index')>";
		echo"<input name='serverid[]' type='hidden' value='$sysid' ></td>";
		echo"<input name='favor[]' type='hidden' value='1'></td>";
	}
	echo"<td class='Data DataRightAlign'>$sysid</td>";
	echo"<td class='Data DataRightAlign'>$sysname</td>";
	echo"<td class='Data DataRightAlign'>$sysip:$sysport</td>";
	echo"<td class='Data DataRightAlign'><input name='isnew[]' type='text' value='$sysnew' ></td>";
	echo"<td class='Data DataRightAlign'><input name='weight[]' type='text' value='$weight' ></td>";
	echo"<td class='Data DataRightAlign'>$tempnum</td>";
	echo"<td class='Data DataRightAlign'>$usernum</td>";
	
	echo"<input name='ip_arr[]' type='hidden' value='$sysip' >";
	echo"<input name='id_arr[]' type='hidden' value='$sysid' >";
	echo"<input name='sysname_arr[]' type='hidden' value='$sysname' >";

	$ip_arr[] = $sysip;
	$id_arr[] = $sysid;
	
	echo"</tr>";
	$index++;

}
echo"<tr class='TableHeader'>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
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
<script language="javascript">

	obj=document.getElementById("id")
	obj.value = <?php echo $maxsysid; ?>;

</script>

<br><br><br><br><br><br>

</body>
</html>
