<?php 
require_once 'db.inc.php';
require_once 'define.php';
error_reporting(0);
date_default_timezone_set("Etc/GMT");
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

if( $_SESSION["adminid"] < 100 )
{
	echo"权限不足";
	exit;
}

$host = 0;
$cityid = 0;
$corps = 0;
$corps = 0;
$level = 0;
$num = 0;

$cmd = $_GET['cmd'];

if ( $cmd == 34 )
{
	$host = $_GET['host'];
	$cityid = $_GET['cityid'];
	$corps = $_GET['corps'];
	$corps = $corps+1;
	$level = $_GET['level'];
	$num = $_GET['num'];	
}
else if ( $cmd == 64 )
{
	$host = $_GET['host'];
	$cityid = $_GET['cityid'];
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
</script>
</head>

<body align="center">
<div width="980" align="center">

<?php

	$string = "";
	$sock = fsockopen ("$host" );
	if( !$sock )
	{
		echo "$server not connect!";
	}
	else
	{
		if ( $cmd == 34 )
		{
				wlogop( "34|$cityid|$corps|$level|$num|$string\0" );
				fputs( $sock, "34|$cityid|$corps|$level|$num|$string\0" );
		}
		else if ( $cmd == 64 )
		{
				wlogop( "64|-1|0|0|$cityid|$string\0" );
				fputs( $sock, "64|-1|0|0|$cityid|$string\0" );
		}
		else
		{
			fclose ($sock);
			return "没有这个指令";
		}
		
		$recvbuff = "";
	  while(!feof($sock)) 
	  {     
      $data = fread($sock,81920);
      $recvbuff .= $data;        
    } 
		$recvarray = explode('|',$recvbuff); 
		
		$result = 100 - $recvarray[0];
		if ( $result != 0 )
			echo "$g_result[$result] 指令执行完毕(服务器:$server 角色ID:$actorid) 指令:$cmd|$arg1|$arg2|$arg3|$arg4|$string<br>";
		else
			echo "$g_result[$result] 指令执行失败(服务器:$server 角色ID:$actorid)<br>";
	}
	fclose ($sock);

?>

</div>
</body>
</html>
