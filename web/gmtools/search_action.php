<?php 
require_once 'db.inc.php';
require_once 'define.php';
/*session方式*/
session_start();
header("content-Type: text/html; charset=gbk");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

$actorid=$_POST["actorid"];
$indata=$_POST["indata"];
$oppath=$_POST["oppath"];

$intime=$indata." ".$_POST["intime"];
$intime2=$indata." ".$_POST["intime2"];

$value_min1=$_POST["value_min1"];
$value_max1=$_POST["value_max1"];
$value_min2=$_POST["value_min2"];
$value_max2=$_POST["value_max2"];
$value_min3=$_POST["value_min3"];
$value_max3=$_POST["value_max3"];

$page=$_POST["page"];
if( $page <= 0 )
	$page = 1;
$page = $page-1;
$beginno = $page*2000;
$datestr=explode( "-", $indata );
$tabname = "glog_".$datestr[0].$datestr[1].$datestr[2];

if ( isset($_POST["op"]) )
		$array_op = $_POST["op"];       //将所选的值组成的数组放入$array数组
else
{
	echo"没有选择操作";
	exit;
}
$opnum = count($array_op);

if ( isset($_POST["favor"]) )
		$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
else
{
	echo"没有选择服务器";
	exit;
}
$num = count($array);           //计算一共有多少项被选择

$hasactorid = 0;
if(isset($actorid) && $actorid > 0 )
{
	$hasactorid = 1;
}
$op_text=array(1=>'得失金钱',2=>'得到物品',3=>'失去物品',4=>'经验变化',5=>'技能变化',6=>'伙伴',9=>'升级',15=>'装备升级',37=>'活动参与',39=>'得失钻石',40=>'得失奥义',47=>'体力');
$path_text=array(0=>'普通',2=>'伙伴抽卡',3=>'角色升级',4=>'神秘商店',5=>'副本',6=>'死亡',7=>'每日任务',8=>'脚本(世界boss)',9=>'刷怪奖励',10=>'修炼',11=>'宠物通灵',12=>'购买金钱体力钻石',13=>'GM',
								14=>'系统礼物',15=>'掠夺，开格子',16=>'竞技场',17=>'契约石合成',18=>'装备制作',19=>'售卖',20=>'宝玉合成',22=>'契约石献祭',23=>'装备强化',24=>'野外战斗',25=>'充值',26=>'悬赏任务',
								27=>'切换显示宠物',29=>'副本宝箱',30=>'宠物强化',31=>'使用物品',32=>'战斗加速',33=>'新手卡',34=>'签到',38=>'附魔刷新',39=>'首充',40=>'伙伴碎片召唤',41=>'伙伴升级技能',42=>'伙伴进阶',
								43=>'伙伴守卫战斗',44=>'邮件',45=>'伙伴分解',46=>'连续登陆',49=>'充值赠送',50=>'主线任务',68=>'升级能力',84=>'活动',85=>'膜拜',86=>'温泉');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
	<style type="text/css"><!--
		body {font-family: Arial,Helvetica,sans-serif; font-size: 12px;}
		h1 {font-size: 18px; text-decoration: bold;}
		h2 {font-size: 16px; text-decoration: bold;}
		h3 {font-size: 14px; text-decoration: bold;}
		th {font-size: 12px; border-style: solid; border-width: 1px; text-decoration: bold; padding: 1px;}
		td {font-size: 12px; border-style: solid; border-width: 1px; padding: 1px;}
		code {font-size: 12px; white-space: pre;}
		.TableObject {border-collapse: collapse; border-color: #000000; font-family: 宋体}
		.TableData {border-collapse: collapse; border-color: #000000; font-family: 宋体}
		.TableHeader {border-color: #000000; text-decoration: bold; background-color: #e0e0e0;}
		.ObjectHeader {padding-left: 5px; text-align: left; border-color: #000000; text-decoration: bold;}
		.ObjectOfPrimaryKey {text-align: left; border-color: #aaaaaa; text-decoration: bold; background-color: #e0e0e0;}
		.ObjectOfUniqueKey {text-align: left; border-color: #aaaaaa; background-color: #e0e0e0;}
		.Object {text-align: left; border-color: #aaaaaa;}
		.Record_1 {}
		.Record_2 {background-color: #f0f0f0;}
		.DataHeader {padding-left: 5px; text-align: left; text-decoration: bold; border-color: #000000; background-color: #e0e0e0;}
		.DataOfPrimaryKey {text-align: left; text-decoration: bold; border-color: #aaaaaa; background-color: #e0e0e0;}
		.DataOfUniqueKey {border-color: #aaaaaa;}
		.Data {border-color: #aaaaaa;}
		.DataRightAlign {text-align: right;}
		.DataNull {color: #999999; border-color: #aaaaaa;}
	--></style>
</head>

<body>
<?php
echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo"<tr class='TableHeader'>";

echo"<td class='Data DataRightAlign'>角色ID</td>";
echo"<td class='Data DataRightAlign'>操作</td>";
echo"<td class='Data DataRightAlign'>途径</td>";
echo"<td class='Data DataRightAlign'>参数1(对象)</td>";
echo"<td class='Data DataRightAlign'>参数2(数量)</td>";
echo"<td class='Data DataRightAlign'>参数3(标识)</td>";
echo"<td class='Data DataRightAlign'>关联角色</td>";
echo"<td class='Data DataRightAlign'>操作时间</td>";

echo"</tr>";

$recno = 2;

for ( $i = 0; $i < $num; $i++ )
{
	if(isset($array[$i]))
	{
		$db = new dbstuff;
		$pconnect = 0;
		$dbh=$dbhost;
		$dbu = $dbuser;
		$dbp = $dbpw;
		$dbd = $dbname;
		$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

		$sql="SELECT * FROM server_list where id='$array[$i]'";
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
			$sysdbd=$row[11];
			
			echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>";

			$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );

			$wherestr = "(";
			for( $tmpi = 0; $tmpi < $opnum; $tmpi++ )
			{
				if( $tmpi == 0 )
					$wherestr = $wherestr."op='".$array_op[$tmpi]."' ";
				else
					$wherestr = $wherestr." or op='".$array_op[$tmpi]."' ";
			}
			$wherestr = $wherestr.")";
			if( $oppath >= 0 )
				$wherestr = $wherestr." and path='".$oppath."'";
			if( isset($_POST["actorid"]) && $_POST["actorid"] > 0 )
				$wherestr = $wherestr." and actor1='".$actorid."'";

			if( $value_min1 != 0 || $value_max1 != 0 )
				$wherestr = $wherestr." and object>='".$value_min1."' and object<='".$value_max1."'";
			if( $value_min2 != 0 || $value_max2 != 0 )
				$wherestr = $wherestr." and objectnum>='".$value_min2."' and objectnum<='".$value_max2."'";
			if( $value_min3 != 0 || $value_max3 != 0 )
				$wherestr = $wherestr." and objectid>='".$value_min3."' and objectid<='".$value_max3."'";

			if ( isset($_POST["object"]) && $_POST["object"] != 0)
				$sql="select op,path,object,objectnum,objectid,actor1,actor2,optime from ".$tabname." where ".$wherestr."  and object='".$_POST["object"]."' and optime>='".$intime."' and optime<='".$intime2."' limit $beginno,2000";
			else
				$sql="select op,path,object,objectnum,objectid,actor1,actor2,optime from ".$tabname." where ".$wherestr." and optime>='".$intime."' and optime<='".$intime2."' limit $beginno,2000";
			$query = $db->query($sql);
			while($row = $db->fetch_row($query))
			{
				$op=$row[0];
				$path=$row[1];
				$object=$row[2];
				$objectnum=$row[3];
				$objectid=$row[4];
				$actor1=$row[5];
				$actor2=$row[6];
				$optime=$row[7];

				if( $recno == 1 )
					$recno = 2;
				else
					$recno = 1;
				echo"<tr class='Record_$recno'>";
			
				echo"<td class='Data DataRightAlign'>$actor1</td>";
				echo"<td class='Data DataRightAlign'>$op_text[$op]</td>";
				if ( $path_text[$path] == "" )
					echo"<td class='Data DataRightAlign'>$path</td>";
				else
					echo"<td class='Data DataRightAlign'>$path_text[$path]</td>";
				echo"<td class='Data DataRightAlign'>$object</td>";
				echo"<td class='Data DataRightAlign'>$objectnum</td>";
				echo"<td class='Data DataRightAlign'>$objectid</td>";
				echo"<td class='Data DataRightAlign'>$actor2</td>";
				echo"<td class='Data DataRightAlign'>$optime</td>";
			
				echo"</tr>";

			}
			$db->close();
		}
		else
		{
			$db->close();
		}
	}
}
// 记录log
echo"</table>";

?>

</body>
</html>
