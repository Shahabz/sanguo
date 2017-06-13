<?php 
require_once 'db.inc.php';
require_once 'define.php';
function wlogop2( $db, $op )
{
	$name = $_SESSION["adminname"];
	$myip = $_SERVER["REMOTE_ADDR"];
}

/*session方式*/
session_start();
header("content-Type: text/html; charset=utf8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}
if( $_SESSION["adminid"] < 100 )
{
	echo"权限不足";
	exit;
}

if( isset($_POST["id"]) )
	$server_id=$_POST["id"];
else
{
	echo"没有选择服务器";
	exit;
}

$num = 0;
if( isset($_POST["favor"]) )
{
	$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
	$num = count($array);           //计算一共有多少项被选择
}
else
{
	$num = 0;
}

$db = new dbstuff;
$pconnect = 0;

$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="SELECT * FROM server_list where id='$server_id'";
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
	
	$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );

	for ( $i = 0; $i < $num; $i++ )
	{
		if(isset($array[$i]))
		{
			$sql="DELETE FROM command_data where id='$array[$i]'";
			$query = $db->query($sql);
		}
	}
	$db->close();

	$cmd = 21;
	$sock = fsockopen ("$sysip:$sysport" );
	if( !$sock )
	{
		echo "$sysip:$sysport not connect!";
	}
	else
	{
		wlogop( "$cmd|0|0|0|0|\0" );
		fputs( $sock, "$cmd|0|0|0|0|\0" );
		$result = fgets( $sock );
		if ( $result != 0 )
			echo "$sysip:$sysport ok!<br>";
		else
			echo "$sysip:$sysport fail![$result]<br>";
	}
	fclose ($sock);

}

exit("<script language='javascript'>window.location.href='clock_one.php?id=$server_id';</script>");

?>
