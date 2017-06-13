<?php 
require_once 'db.inc.php';

/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

$p=$_POST["p"];

$db = new dbstuff;
$pconnect = 0;

$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

$sql="desc notice";
$query = $db->query($sql);
$field_arr = array();
$value_arr = array();
$field_count = 0;
while($row = $db->fetch_row($query))
{
	$field_arr[]=$row[0];
	$item = $_POST[$row[0]];
	$value_arr[] = $item;
	$field_count++;
}

if( $p == 1 )
{
	$sql="UPDATE notice set ";
	for( $tmpi = 1; $tmpi < $field_count; $tmpi++ )
	{
		if( $tmpi > 1 )
			$sql .= ",";
		$sql .= "`$field_arr[$tmpi]`='$value_arr[$tmpi]'";
	}
	$sql .= " where id='$value_arr[0]'";
	$query = $db->query($sql);
}
else if( $p == 2 )
{
	$id=$_POST["id"];
	$sql="DELETE from notice where id='$id'";
	$query = $db->query($sql);
}
else
{
	$sql="INSERT into notice values( ";
	for( $tmpi = 0; $tmpi < $field_count; $tmpi++ )
	{
		if( $tmpi > 0 )
			$sql .= ",";
		$sql .= "'$value_arr[$tmpi]'";
	}
	$sql .= ")";
	$query = $db->query($sql);
}

$db->close();

exit("<script language='javascript'>window.location.href='notice.php';</script>");
?>
