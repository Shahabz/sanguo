<?php 
require_once 'db.inc.php';
require_once 'define.php';
error_reporting(1);
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf8");
date_default_timezone_set("Etc/GMT");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}


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

<script language="javascript">
 function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
 }
</script>

<body>
<?php

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
	echo"<tr>";
	echo"<td>";
		echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
		echo"<tr class='TableHeader'>";
		echo"<td class='Data DataRightAlign'>断点</td>";
		echo"<td class='Data DataRightAlign'>人数</td>";
		echo"</tr>";

		$recno = 2;
		
		$db = new dbstuff;
		$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );
		
		$step1_num = 0;
		$step2_num = 0;
		$step3_num = 0;
		$step11_num = 0;
		$step19_num = 0;
		$step20_num = 0;
		$step21_num = 0;
		$step22_num = 0;
		$step23_num = 0;
		$step24_num = 0;
		$step100_num = 0;
		$totalnum = 0;
		$sql="select a.step, count(*) from (select max(step) as step from tracklog where step_time >= '$begintimestamp' and step_time <= '$endtimestamp' and step >= 3 group by deviceid ) a group by a.step;";
		echo "$sql";
		$query = $db->query($sql);
		while($row = $db->fetch_row($query))
		{
			$step=$row[0];
			$count=$row[1];
			
			echo"<tr class='Record_2'>";

				if ( $step == 3 )
				{
						echo"<td class='Data DataRightAlign'>初始化SDK，或者连接服务器处流失(3)</td>";
						$step3_num += $count;
				}
				else if ( $step == 11 )
				{
						echo"<td class='Data DataRightAlign'>玩家loading处流失(11)</td>";
						$step11_num += $count;
				}
				else if ( $step == 19 )
				{
						echo"<td class='Data DataRightAlign'>进入游戏，弹出小妹说话的对话框前(19)</td>";
						$step19_num += $count;
				}
				else if ( $step == 20 )
				{
						echo"<td class='Data DataRightAlign'>关闭对话框后，开始播放人类投石车攻击动画(20)</td>";
						$step20_num += $count;
				}
				else if ( $step == 21 )
				{
						echo"<td class='Data DataRightAlign'>玩家点击乌瑟尔说话，关闭乌瑟尔说话后，播放动画前(21)</td>";
						$step21_num += $count;
				}
				else if ( $step == 22 )
				{
						echo"<td class='Data DataRightAlign'>人类攻城动画播放完毕后，巫妖王出现前打(22)</td>";
						$step22_num += $count;
				}
				else if ( $step == 23 )
				{
						echo"<td class='Data DataRightAlign'>玩家点击巫妖王说话，关闭巫妖王说话后，播放巫妖王离开动画前(23)</td>";
						$step23_num += $count;
				}
				else if ( $step == 24 )
				{
						echo"<td class='Data DataRightAlign'>玩家点击巫妖王后，使命任务界面打开前(24)</td>";
						$step24_num += $count;
				}
				else if ( $step == 100 )	
				{
					echo"<td class='Data DataRightAlign'>正式创建角色完毕，进入游戏(100)</td>";
					$step100_num += $count;
				}
				else
				{
					echo"<td class='Data DataRightAlign'>$step</td>";
				}
				
				echo"<td class='Data DataRightAlign'>$count</td>";
				
				$totalnum += $count;

			echo"</tr>";
		}
		$db->close();
		echo"<td class='Data DataRightAlign'></td>";
		echo"<td class='Data DataRightAlign'>$totalnum</td>";	
		echo"</table>";
	echo"</td>";
	
	echo"<td>";
	echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
	
	echo"<tr class='TableHeader'>";
	echo"<td class='Data DataRightAlign'>占比</td>";
	echo"</tr>";
		
	
	echo"<tr class='Record_2'>";
	$n3 = round( ($step3_num/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>$n3%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	$n11 = round( ($step11_num/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>$n11%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	$n19 = round( ($step19_num/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>$n19%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	$n20 = round( ($step20_num/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>$n20%</td>";
	echo"</tr>";
	
	if ( $step21_num > 0 )
	{
		echo"<tr class='Record_2'>";
		$n21 = round( ($step21_num/$totalnum)*100, 2);
		echo"<td class='Data DataRightAlign'>$n21%</td>";
		echo"</tr>";
	}
	
	echo"<tr class='Record_2'>";
	$n22 = round( ($step22_num/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>$n22%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	$n23 = round( ($step23_num/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>$n23%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	$n24 = round( ($step24_num/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>$n24%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	$n100 = round( ($step100_num/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>$n100%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	echo"<td class='Data DataRightAlign'>无</td>";
	echo"</tr>";
	
	echo"</table>";
echo"</td>";

echo"<td>";
	echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
		
	echo"<tr class='Record_2'>";
	$calc = round( (($step3_num+$step11_num+$step19_num+$step20_num+$step21_num+$step22_num+$step23_num+$step24)/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>角色都没有创建的占:$calc%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	$calc = round( (($step3_num+$step11_num)/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>其中尚未进入引导的占:$calc%</td>";
	echo"</tr>";
	
	echo"<tr class='Record_2'>";
	$calc = round( (($step19_num+$step20_num+$step21_num+$step22_num+$step23_num+$step24)/$totalnum)*100, 2);
	echo"<td class='Data DataRightAlign'>其中引导过程中占:$calc%</td>";
	echo"</tr>";
	
	echo"</table>";
echo"</td>";

echo"</tr>";
echo"</table>";
?>

<form id='form1' name='form1' method='post' action='tracklog.php'>
<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >
	
		<tr class="Record_2" id="proc3">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value3">日期</th>
		<td class="Data DataRightAlign">
		<input type="text" id="begindate" name="begindate" size="20" value="<?php echo date("Y-m-d"); ?>"/>-
		<input type="text" id="enddate" name="enddate" size="20" value="<?php echo date("Y-m-d"); ?>"/>时间
		<input type="text" id="begintime" name="begintime" size="20" value="00:00:00"/>-
		<input type="text" id="endtime" name="endtime" size="20" value="23:59:59"/>
	  </td>
	</tr>
	
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"> </th>
		<td class="Data DataRightAlign">
		<input type="submit" id="submit" name="submit" value="提交" />
	    </td>
	</tr>
</table>
</form>

</body>
</html>
