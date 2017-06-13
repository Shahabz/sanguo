<?php 
require_once 'db.inc.php';

session_start();
header("content-Type: text/html; charset=gbk");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}
$op=$_POST["op"];

$name=$_POST["name"];
$ip=$_POST["ip"];
$port=$_POST["port"];

//$size = filesize ( $hostlist_file );
//if ( $size == false )
//	$size = 0;

$fp= fopen( $hostlist_file, "a" );
//if( $size > 0 )
//	fwrite($fp, "\n");
fwrite($fp, "$ip $port|$name\n");
fclose($fp);

exit("<script language='javascript'>window.location.href='serv_watch.php';</script>");

?>
