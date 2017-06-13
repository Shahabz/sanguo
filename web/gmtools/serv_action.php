<?php 
require_once 'db.inc.php';

session_start();
header("content-Type: text/html; charset=gbk");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}
$op=$_POST["op"];

$id=$_POST["id"];
$name=$_POST["name"];
$ip=$_POST["ip"];
$port=$_POST["port"];
$out_ip=$_POST["out_ip"];
$out_port=$_POST["out_port"];
$servpath=$_POST["servpath"];

$db = new dbstuff;

$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="INSERT INTO server_list (id,name,ip,port,out_ip,out_port,servpath) values('$id','$name','$ip','$port','$out_ip','$out_port','$servpath')";
$query = $db->query($sql);
$db->close();

exit("<script language='javascript'>window.location.href='serv.php';</script>");

?>
