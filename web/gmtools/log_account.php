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

$sysid=$_POST["sysid"];
$actorid=$_POST["actorid"];
$date=$_POST["date"];
$path=$_POST["path"];

$begintime=$date." ".$_POST["begintime"];
$endtime=$date." ".$_POST["endtime"];

$page = $_POST["page"];
if( $page <= 0 )
	$page = 1;
$page = $page-1;
$beginno = $page*2000;
$datestr = explode( "-", $date );
$tabname = "glog_".$datestr[0].$datestr[1].$datestr[2];


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
<?php

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo"<tr class='TableHeader'>";
echo"<td class='Data DataRightAlign'>操作</td>";
echo"<td class='Data DataRightAlign'>登入时间</td>";
echo"<td class='Data DataRightAlign'>登出时间</td>";
echo"<td class='Data DataRightAlign'>登入等级</td>";
echo"<td class='Data DataRightAlign'>登出等级</td>";
echo"<td class='Data DataRightAlign'>IP</td>";
echo"</tr>";

$recno = 2;

$db = new dbstuff;
$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="SELECT * FROM server_list where id='$sysid'";
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
	
	echo "<tr class='Record_2'><th class='DataHeader'>$sysname [$actorid]</th><td></td><td></td><td></td><td></td><td></td></tr>";

	$db->connect( $sysdbh, $sysdbu, $sysdbp, $syslogdb, 0 );
	
	$logcount=0;
	$sql="select op_type,in_time,out_time,in_level,out_level,ip from account where actorid='$actorid' order by in_time desc limit 200";
	$query = $db->query($sql);
	while($row = $db->fetch_row($query))
	{
		$op_type=$row[0];
		$in_time=$row[1];
		$out_time=$row[2];
		$in_level=$row[3];
		$out_level=$row[4];
		$ip=$row[5];

		if( $recno == 1 )
			$recno = 2;
		else
			$recno = 1;
		echo"<tr class='Record_$recno'>";
		if ( $op_type == 1 )
			echo"<td class='Data DataRightAlign'>正常</td>";
		else if ( $op_type == 2 )
			echo"<td class='Data DataRightAlign'>顶替</td>";
		else
			echo"<td class='Data DataRightAlign'>非正常下线</td>";
			
		echo"<td class='Data DataRightAlign'>&nbsp$in_time&nbsp</td>";
		echo"<td class='Data DataRightAlign'>&nbsp$out_time&nbsp</td>";
		echo"<td class='Data DataRightAlign'>&nbsp$in_level&nbsp</td>";
		echo"<td class='Data DataRightAlign'>&nbsp$out_level&nbsp</td>";
		echo"<td class='Data DataRightAlign'>&nbsp$ip&nbsp</td>";
		echo"</tr>";
		$logcount++;
	}
	$db->close();
		
}
echo"</table>";
echo "总计：$logcount/200";
?>

</body>
</html>
