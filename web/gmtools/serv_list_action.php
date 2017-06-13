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

$db = new dbstuff;

$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

$sql="INSERT INTO gate_list (id,name,ip,port,new,usernum,close) values('$id','$name','$ip','$port','0','0','1')";
$query = $db->query($sql);
$db->close();

exit("<script language='javascript'>window.location.href='serv_list.php';</script>");

?>
