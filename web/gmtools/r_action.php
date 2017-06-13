<?php 
require_once 'db.inc.php';
require_once 'define.php';

// 过滤函数
function htmlEnc( $str )
{
  return $str;
}

/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

//if( $_SESSION["adminid"] < 50 )
//{
	//echo"权限不足";
	//exit;
//}


$command=0;
$pv1=0;
$pv2=0;
$pv3=0;
$pvstring = "";

if(!empty($_POST['command'])){
$command = $_POST['command'];
}

if(!empty($_POST['proc_value1'])){
$pv1 = $_POST['proc_value1'];
}

if(!empty($_POST['proc_value2'])){
$pv2 = $_POST['proc_value2'];
}
if(!empty($_POST['proc_value3'])){
$pv3 = $_POST['proc_value3'];
}
if(!empty($_POST['pvstring'])){
$pvstring = $_POST['pvstring'];
}

if ( isset($_POST["favor"]) )
	$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
else
{
	echo"没有选择举报信息";
	exit;
}
$num = count($array);           //计算一共有多少项被选择

$db = new dbstuff;
$pconnect = 0;
$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

$sql="SELECT serverid,ip,port FROM server_list";
$query = $db->query($sql);
while($row = $db->fetch_row($query))
{
	$id = $row[0];
	$sysid[$id]=$id;
	$sysip[$id]=$row[1];
	$sysport[$id]=$row[2];
}


for ( $i = 0; $i < $num; $i++ )
{
	if(isset($array[$i]))
	{
		// $sign|$serverid|$cityid|$actorid
		$var=explode("|",$array[$i]);
		
		$sign=$var[0];
		$serverid=$var[1];
		$cityid=$var[2];
		$actorid=$var[3];
		
		$cmd=$command;
		$arg1=$pv1;
		$arg2=$pv2;
		$arg3=$pv3;
		$arg4=$actorid;
		$string=$pvstring;
		
		if ( $cmd == 64 )
		{
			$arg4=$cityid;
		}
		
		
		if ( $cmd == -1 )
		{
			$sql="DELETE FROM `report_chat` WHERE `sign`='$sign';";
			$query = $db->query($sql);
			echo "删除完毕<br>";
		}
		else if ( $cmd == -2 )
		{
			$sql="DELETE FROM `report_head` WHERE `sign`='$sign';";
			$query = $db->query($sql);
			echo "删除完毕<br>";
		}
		else if ( $cmd == -3 )
		{
			$sql="DELETE FROM `report_name` WHERE `sign`='$sign';";
			$query = $db->query($sql);
			echo "删除完毕<br>";
		}
		else
		{
			$server = $sysip[$serverid].":".$sysport[$serverid];
			$sock = fsockopen ("$server" );
			if( !$sock )
			{
				echo "$server not connect!";
			}
			else
			{
				wlogop( "$cmd|$arg1|$arg2|$arg3|$arg4|$string\0" );
				fputs( $sock, "$cmd|$arg1|$arg2|$arg3|$arg4|$string\0" );
				$recvbuf = "";
			  while(!feof($sock)) 
			  {     
	        $data = fread($sock,81920);
	        $recvjson .= $data;        
	      }
	      
				$recvarray = explode('|',$recvjson); 
				
				$result = 100 - $recvarray[0];
				if ( $result != 0 )
				{
						echo "$g_result[$result] 指令执行完毕(服务器:$server 角色ID:$actorid) 指令:$cmd|$arg1|$arg2|$arg3|$arg4|$string<br>";
						//$query = $db->query("DELETE FROM `report_chat` WHERE `sign`='$sign';");	
						//$query = $db->query("DELETE FROM `report_head` WHERE `sign`='$sign';");
				}
				else
					echo "$g_result[$result] 指令执行失败(服务器:$server 角色ID:$actorid)<br>";
				
				
				
				echo $recvarray[1]."<br>";
					
			}
			fclose ($sock);
		}
	}
}

// 记录log

?>