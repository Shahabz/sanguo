<?php 
require_once 'db.inc.php';
require_once 'define.php';
date_default_timezone_set("Etc/GMT");

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

if( $_SESSION["adminid"] < 100 )
{
	echo"权限不足";
	exit;
}

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

if ( $command == 54 )
{
	$pvstring = $pv1."&".$pv2."&".$pvstring;
	//$pvstring = iconv("utf-8//IGNORE","gbk",$pvstring);
	$pv1 = 0;
	$pv2 = 0;
	$pv3 = 0;
}

if ( isset($_POST["favor"]) )
	$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
else
{
	echo"没有选择服务器";
	exit;
}

if ( $command == -99 || $command == -98 )
{
		$actorid = 0;
		if(isset($array[0]))
		{
			$var=explode("|",$array[0]);
			$server=$var[0];
			$actorid=$var[1];
		}
		$db = new dbstuff;
		$pconnect = 0;
		$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

		$sql="SELECT * FROM server_list where id='$pv3'";
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
		}
		$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );
		if ( $command == -99 )
		{
			$sql = "UPDATE `actor_list` SET `userid`='$pv1',`username`='$pv2' WHERE `actorid`='$actorid';";
			$query = $db->query($sql);
		}
		else if ( $command == -98 )
		{
			$sql = "UPDATE `actor_list` SET `offset`=0 WHERE `actorid`='$actorid';";
			$query = $db->query($sql);
		}
		$db->close();
		echo"设置用户关联成功";
		exit;
}


$num = count($array);           //计算一共有多少项被选择
$interest = "";                 //设置一个空变量interest
for ( $i = 0; $i < $num; $i++ )
{
	if(isset($array[$i]))
	{
		$var=explode("|",$array[$i]);
		$server=$var[0];
		$actorid=$var[1];

		$cmd=$command;
		$arg1=$pv1;
		$arg2=$pv2;
		$arg3=$pv3;
		$arg4=$actorid;
		$string=$pvstring;

if ( $command == 26 )
{// 充值接口特殊转换
	$arg1 = $actorid;
	$arg2 = $pv1;
	$string = $pv2."|".$pv3."|0";
	$arg3 = 0;
	$arg4 = 0;
}


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
				echo "$g_result[$result] 指令执行完毕(服务器:$server 角色ID:$actorid) 指令:$cmd|$arg1|$arg2|$arg3|$arg4|$string<br>";
			else
				echo "$g_result[$result] 指令执行失败(服务器:$server 角色ID:$actorid)<br>";
			
			
			
			echo $recvarray[1]."<br>";
				
		}
		fclose ($sock);
	}
}

// 记录log

?>