<?php 
session_start(); 
?>
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
</head>

<?php
require_once 'db.inc.php';
if(!session_is_registered("adminid") || $_SESSION["adminid"] < 120 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

if (!get_magic_quotes_gpc())
{
	$button=addslashes(@$_GET['button']);
	$name=addslashes(@$_GET['name']);
	$passwd=addslashes(@$_GET['passwd']);
	$level=addslashes(@$_GET['level']);
}
else
{
	$button=@$_GET['button'];
	$name=@$_GET['name'];
	$passwd=@$_GET['passwd'];
	$level=@$_GET['level'];
}

#打开数据库
$db = new dbstuff;
$pconnect = 0;

$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

if( $button == '添加' )
{
	$sql="insert into gmuser (name,passwd,level) value('$name',MD5('$passwd'),'$level')";
	$query = $db->query($sql);
	$db->free_result($query);
	$db->close();
	exit("<script language='javascript'>window.location.href='user_ad.php';</script>");
}
elseif( $button == 'delete' )
{
	$sql="delete from gmuser where name='$name'";
	$query = $db->query($sql);
	$db->free_result($query);
	$db->close();
	exit("<script language='javascript'>window.location.href='user_ad.php';</script>");
}
?>


<body>
    <form name="form1" method="get" action="user_ad.php">
	<table border='0' cellspacing='0' cellpadding='0' class='TableData' align="center" width="320" >

	<tr class='TableHeader'>
		<th class='DataHeader'></th>
		<th class='DataHeader'>管理用户</th>
		<th class='DataHeader'></th>
	</tr>

<?php

$sql="select name,level from gmuser";
$query = $db->query($sql);

while($row = $db->fetch_row($query))
{
	$username=$row[0];
	$userlevel=$row[1];
	echo "<tr class=\"Record_1\"><th class=\"Data DataRightAlign\">$username</th>";
	echo "<td class=\"Data DataRightAlign\">$userlevel</td>";
	echo "<td class=\"Data DataRightAlign\"><a href='user_ad.php?name=$username&button=delete'>删除</a></td></tr>";
}
$db->free_result($query);
$db->close();

?>
	<tr class="Record_1"><th class="Data DataRightAlign">　</th>
	<td class="Data DataRightAlign"> </td>
	<td class="Data DataRightAlign"> </td></tr>

	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">User:</th>
		<td class="Data DataRightAlign">
			<input type="text" name="name" id="name">
	    </td>
	    <td class="Data DataRightAlign"></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">Pass:</th>
		<td class="Data DataRightAlign">
			<input type="password" name="passwd" id="passwd">
	    </td>
	    <td class="Data DataRightAlign"></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">Level:</th>
		<td class="Data DataRightAlign">
			<input type="text" name="level" id="level" value='50'>
	    </td>
	    <td class="Data DataRightAlign"></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"></th>
		<td class="Data DataRightAlign">
			 <input type="submit" name="button" id="button" value="添加"> 
	    </td>
	    <td class="Data DataRightAlign"></td>
	</tr>

	</table>
    </form>
</body>
</html>