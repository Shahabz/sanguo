<?php 
require_once 'db.inc.php';
require_once 'define.php';
date_default_timezone_set("Etc/GMT");
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

if(!empty($_POST['radio'])){
$ds = $_POST['radio'];
}
if(!empty($_POST['systalk_type'])){
$systalk_type = $_POST['systalk_type'];
}
if(!empty($_POST['systalk_info'])){
$systalk_info = $_POST['systalk_info'];
}

$actorid=$_POST["actorid"];
$proc_type=$_POST["type"];
$proc_num=$_POST["num"];
$proc_num2=$_POST["num2"];

if ( isset($_POST["favor"]) )
		$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
else
{
	echo"没有选择服务器";
	exit;
}
$num = count($array);           //计算一共有多少项被选择
$interest = "";                 //设置一个空变量interest

$hasactorid = 0;
if(isset($actorid) && $actorid > 0 )
{
	$hasactorid = 1;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
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

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script language="javascript">
 function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
 }
 
  function post(url, params) {
		var temp = document.createElement("form");
		temp.action = url;
		temp.method = "post";
		temp.target = '_blank';
		temp.style.display = "none";
		for (var x in params) {
			var opt = document.createElement("textarea");
			opt.name = x;
			opt.value = params[x];
			temp.appendChild(opt);
		}
		document.body.appendChild(temp);
		temp.submit();
		return temp;
}

</script>
</head>

<body align="center">
<div width="980" align="center">

<?php

$g_paygoods = array(0=>'无');
function getdbdatalist( $db2 )
{
	Global $g_paygoods;
	$sql="select goodsid, name from paygoods;";
	$query = $db2->query($sql);
	while($row = $db2->fetch_row($query))
	{
		$g_paygoods[$row[0]] = $row[1];
	}
	return 0;
}
$db2 = new dbstuff;
$db2->connect( $data_dbh, $data_dbu, $data_dbp, $data_dbd, 0 );
getdbdatalist( $db2 );

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo"<th class='DataHeader'>游戏订单</th>";
echo"<th class='DataHeader'>游戏用户ID</th>";
echo"<th class='DataHeader'>游戏角色ID</th>";
echo"<th class='DataHeader'>创角时间</th>";
echo"<th class='DataHeader'>角色等级</th>";
echo"<th class='DataHeader'>城池等级</th>";
echo"<th class='DataHeader'>游戏商品ID</th>";
echo"<th class='DataHeader'>游戏商品名称</th>";
echo"<th class='DataHeader'>awardgroup</th>";
echo"<th class='DataHeader'>IP</th>";
echo"<th class='DataHeader'>是否支付成功</th>";
echo"<th class='DataHeader'>时间</th>";

$recno = 2;
for ( $i = 0; $i < $num; $i++ )
{
	if(isset($array[$i]))
	{
		$db = new dbstuff;
		$pconnect = 0;
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
			$sysdbd=$row[7];
			
			$syslogdb=$row[11];
			
			echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>";
			$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );


			if( $hasactorid == 1 )
			{
				$sql="select p.orderid,p.userid,p.actorid,p.actorlevel,p.citylevel,p.productid,p.goodsid,p.awardgroup,p.ip,p.status,p.optime,a.createtime,p.status from pay_order p,actor a where a.actorid=p.actorid and p.actorid = $actorid";
			}
			else
			{
				$sql="select p.orderid,p.userid,p.actorid,p.actorlevel,p.citylevel,p.productid,p.goodsid,p.awardgroup,p.ip,p.status,p.optime,a.createtime,p.status from pay_order p,actor a where a.actorid=p.actorid and a.laird_name like '%$systalk_info%' limit 100;";
			}
			$query = $db->query($sql);
			while($row = $db->fetch_row($query))
			{
				$orderid=$row[0];
					$userid=$row[1];
					$actorid=$row[2];
					$actorlevel=$row[3];
					$citylevel=$row[4];
					$productid=$row[5];
					$goodsid=$row[6];
					$awardgroup=$row[7];
					$ip=$row[8];
					$status=$row[9];
					$optime=date("Y-m-d H:i:s", $row[10]) ;
					$createtime=date("Y-m-d H:i:s",$row[11]);
					$status=$row[12];
					$totalrecharge += $g_productid[$productid];
					echo"<tr class='Record_2'>";
					echo"<td class='Data DataRightAlign'>$orderid</td>";
					echo"<td class='Data DataRightAlign'>$userid</td>";		
					echo"<td class='Data DataRightAlign'>$actorid</td>";	
					echo"<td class='Data DataRightAlign'>$createtime</td>";
					echo"<td class='Data DataRightAlign'>$actorlevel</td>";
					echo"<td class='Data DataRightAlign'>$citylevel</td>";
					echo"<td class='Data DataRightAlign'>$goodsid</td>";
					echo"<td class='Data DataRightAlign'>$g_paygoods[$goodsid]</td>";	
					echo"<td class='Data DataRightAlign'>$awardgroup</td>";
					echo"<td class='Data DataRightAlign'>$ip</td>";
					echo"<td class='Data DataRightAlign'>$status</td>";
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
?>

</div>
</body>
</html>
