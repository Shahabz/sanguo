<?php 
require_once 'db.inc.php';
require_once 'define.php';
function wlogop2( $db, $op )
{
	$name = $_SESSION["adminname"];
	$myip = $_SERVER["REMOTE_ADDR"];
	$sql="INSERT INTO oplog (name,op,ip,optime) values('$name','$op','$myip',now())";
	$db->query($sql);
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


$systalk_type=$_POST["systalk_type"];
$systalk_info=$_POST["systalk_info"];

$actorid=$_POST["actorid"];
$proc_type=$_POST["type"];

if( isset($_POST["serverlist"]) )
	$server_array = $_POST["serverlist"];       //将所选的值组成的数组放入$array数组
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

$sqllist = array();
$loglist = array();
for ( $i = 0; $i < $num; $i++ )
{
	if(isset($array[$i]))
	{
		$sql="SELECT * FROM command_set WHERE id='$array[$i]'";
		$query = $db->query($sql);
		if($row = $db->fetch_row($query))
		{
			$sqllist[]="INSERT INTO command_data (id,cond,cmd,value1,value2,value3,value4,strvalue,timestr,addtime) values('$row[0]','$row[1]','$row[2]','$row[3]','$row[4]','$row[5]','$row[6]','$row[7]','$row[8]',now())";
			$loglist[]="0|$row[1]|$row[2]|$row[3]|$row[4]|$row[5]|$row[6]|$row[7]|$row[8]\0";
		}
	}
}
for ( $i = 0; $i < $num; $i++ )
{
	if(isset($loglist[$i]))
	{
		wlogop2( $db, $loglist[$i] );
	}
}

$db->close();

$servernum = count($server_array);           //计算一共有多少项被选择

for ( $i = 0; $i < $servernum; $i++ )
{
	if(isset($server_array[$i]))
	{
		$db = new dbstuff;
		$pconnect = 0;

		$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

		$sql="SELECT * FROM server_list where id='$server_array[$i]'";
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
			
			echo "$sysname<br>";

			$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );

			if( $proc_type == 0 )
			{
				$db->query( "DELETE FROM command_data;" );
			}
			for( $tmpi = 0; $tmpi < $num; $tmpi++ )
			{
				$db->query($sqllist[$tmpi]);
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
	}
}

// 记录log

?>