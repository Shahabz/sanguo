<?php 
require_once 'db.inc.php';
require_once 'define.php';
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

function getRandChar($length)
{
   $str = null;
   $strPol = "abcdefghijklmnopqrstuvwxyz";
   $max = strlen($strPol)-1;
   for($i=0;$i<$length;$i++)
   {
    $str.=$strPol[rand(0,$max)];
   }
   return $str;
}

// 操作
$op=$_POST["op"];

$cdkey_index=$_POST["cdkey_index"];
$cdkey_id=$_POST["cdkey_id"];
$limit_sys=$_POST["limit_sys"];
$limit_channel=$_POST["limit_channel"];
$awardgroup=$_POST["awardgroup"];
$overduetime=$_POST["overduetime"];
$totalcount=$_POST["totalcount"];

$beginid=$_POST["beginid"];
$endid=$_POST["endid"];

$db = new dbstuff;
$pconnect = 0;
$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

if ( $op == 1 )
{ // 创建
	if ( $overduetime > 0 )
		$overduetime = time() + $overduetime*24*3600;
		
	for ( $tmpi=0; $tmpi < $totalcount; $tmpi++ )
	{
		$pwd = getRandChar(3);
		$sql="INSERT INTO `card` SET `id`=$cdkey_id+$tmpi,`passwd`='$pwd',`cdkey_index`='$cdkey_index',`limit_sys`='$limit_sys',`limit_channel`='$limit_channel',`awardgroup`='$awardgroup',`overduetime`='$overduetime';";
		$query = $db->query($sql);
	}
	echo "生成完毕";
}
else if ( $op == 2 )
{ // 删除
	$sql="DELETE FROM card where id >= '$beginid' and id <= '$endid';";
	$query = $db->query($sql);
	echo "删除成功";
}
else if ( $op == 3 )
{ // 下载
	
	header('Content-Type: application/force-download');
	header('Content-Disposition: attachment; filename=兑换码导出('.$beginid.'-'.$endid.').txt');

	$sql="SELECT id, passwd, cdkey_index, limit_sys, limit_channel, awardgroup, overduetime FROM card WHERE id >='$beginid' and id <= $endid;";
	$query = $db->query($sql);

 		echo "cdkey\t";
	  echo "number\t";
	  echo "sys\t";
	  echo "channel\t";
	  echo "award\t";
	  echo "overduetime";
	  echo "\n";
	  
	while($row = $db->fetch_row($query))
	{
		$id=$row[0];
		$passwd=$row[1];
		$cdkey_index=$row[2];
		$limit_sys=$row[3];
		$limit_channel=$row[4];
		$awardgroup=$row[5];
		$overduetime=$row[6];
		
	  echo "$id$passwd\t";
	  echo "$cdkey_index\t";
	  echo "$limit_sys\t";
	  echo "$limit_channel\t";
	  echo "$awardgroup\t";
	  echo "$overduetime";
	  echo "\n";
	
	}
}
?>
