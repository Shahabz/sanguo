<?php
error_reporting(0);
require_once '../db.inc.php';

if (!get_magic_quotes_gpc())
{
	$deviceid=addslashes(@$_GET['d']);
	$step=addslashes(@$_GET['step']);
	$country=addslashes(@$_GET['c']);
	$system=addslashes(@$_GET['sys']);
	$clientver=addslashes(@$_GET['cv']);
	$resver=addslashes(@$_GET['rv']);
}
else
{
	$deviceid=@$_GET['d'];
	$step=@$_GET['step'];
	$country=@$_GET['c'];
	$system=@$_GET['sys'];
	$clientver=@$_GET['cv'];
	$resver=@$_GET['rv'];
}



#打开数据库
$db = new dbstuff;
$pconnect = 0;
$db->connect($dbhost, $dbuser, $dbpw, $dbname, $pconnect);
$dbhost = $dbuser = $dbpw = $dbname = $pconnect = NULL;
$thistime = time();
$sql="insert into tracklog ( deviceid, step, step_time, country, system, clientver, resver ) values('$deviceid','$step','$thistime','$country','$system','$clientver','$resver') ";
$db->query($sql);
$db->close();
?>