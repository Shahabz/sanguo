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

//<!--JS 页面自动刷新 -->
echo ("<script type=\"text/javascript\">");
echo ("function fresh_page()");    
echo ("{");
echo ("window.location.reload();");
echo ("}"); 
echo ("setTimeout('fresh_page()',500000);");
echo ("</script>");



if(!empty($_POST['radio'])){
$ds = $_POST['radio'];
}
if(!empty($_POST['systalk_type'])){
$systalk_type = $_POST['systalk_type'];
}
if(!empty($_POST['systalk_info'])){
$systalk_info = $_POST['systalk_info'];
}


$proc_type=$_POST["type"];
$begindate=$_POST["begindate"];
$enddate=$_POST["enddate"];
$begintime=$_POST["begintime"];
$endtime=$_POST["endtime"];


// 时间戳范围
$begintimestamp = strtotime($begindate." 00:00:00");
$endtimestamp = strtotime($enddate." 23:59:59");
if ( $begindate == $enddate )
{
	$begintimestamp = strtotime($begindate." ".$begintime);
	$endtimestamp = strtotime($enddate." ".$endtime);
}

// 时间字符串范围
$bdate = $begindate." 00:00:00";
$edate = $enddate." 23:59:59";
if ( $begindate == $enddate )
{
	$bdate = $begindate." ".$begintime;
	$edate = $enddate." ".$endtime;
}

$datestr = explode( "-", $begindate );
$tabname = "glog_".$datestr[0].$datestr[1].$datestr[2];

if ( isset($_POST["favor"]) )
		$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
else
{
	echo"没有选择服务器";
	exit;
}
$num = count($array);           //计算一共有多少项被选择
$interest = "";                 //设置一个空变量interest

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

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
if ( $proc_type == -12 )
{ // 聊天
	echo"<th class='DataHeader'>频道</th>";
	echo"<th class='DataHeader'>名称</th>";
	echo"<th class='DataHeader'>VIP</th>";
	echo"<th class='DataHeader'>消息</th>";
	echo"<th class='DataHeader'>时间</th>";
}


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
			$db->connect( $sysdbh, $sysdbu, $sysdbp, $sysdbd, 0 );
			
			if ( $proc_type == -12 )
			{ // 聊天
				echo "<tr class='Record_2'><th class='DataHeader'>$sysname</th><td></td><td></td><td></td><td></td></tr>";
				
	
				$sql="select type, prefix, name, viplevel, msg, sendtime from world_talkcache where sendtime >= '$begintimestamp' and sendtime <= '$endtimestamp' order by sendtime desc limit 1000;";
				$query = $db->query($sql);
				$totalcount = 0;
				while($row = $db->fetch_row($query))
				{
					$type=$row[0];
					$prefix=$row[1];
					$name=$row[2];
					$viplevel=$row[3];
					$msg=$row[4];
					$sendtime=date("Y-m-d H:i:s", $row[5]);
					$totalcount += 1;
								
					if ( $type == 0 )
					{
						echo"<tr class='Record_2'>";
						echo"<td class='Data DataRightAlign'>世界</td>";
					}
					else
					{
						echo"<tr class='Record_1'>";
						echo"<td class='Data DataRightAlign'>联盟</td>";
					}
					
					if ( $prefix == "" )
					{
						echo"<td class='Data DataRightAlign'>$name</td>";
					}
					else
					{
						echo"<td class='Data DataRightAlign'>[$prefix]$name</td>";
					}
					
					echo"<td class='Data DataRightAlign'>$viplevel</td>";
					
					$arr = explode( '|', $msg );
					if ( $arr[0]  == "$#" )
					{
						$i=$arr[1];
						echo"<td class='Data DataRightAlign'>【自动发言】$g_talk_auto_name[$i]</td>";
					}
					else
					{
						echo"<td class='Data DataRightAlign'>$msg</td>";
					}
					
					echo"<td class='Data DataRightAlign'>$sendtime</td>";
					echo"</tr>";
				}
				echo"总条数：$totalcount";
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
</table>



</div>
</body>
</html>
