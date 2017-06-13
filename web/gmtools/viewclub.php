<?php 
require_once 'db.inc.php';
require_once 'define.php';
error_reporting(0);
date_default_timezone_set("Etc/GMT");
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}


$host = $_POST['host'];
$clubid = $_POST['clubid'];
$sysdbh = $_POST['sysdbh'];
$sysdbu = $_POST['sysdbu'];
$sysdbp = $_POST['sysdbp'];
$sysdbd = $_POST['sysdbd'];

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
</script>
</head>

<body align="center">
<div width="980" align="center">

<?php

	$string = "";
	$sock = fsockopen ("$host" );
	if( !$sock )
	{
		echo "$server not connect!";
	}
	else
	{
		wlogop( "7|4|0|0|$clubid|$string\0" );
		fputs( $sock, "7|4|0|0|$clubid|$string\0" );
		$recvbuff = "";
	  while(!feof($sock)) 
	  {     
      $data = fread($sock,81920);
      $recvbuff .= $data;        
    }
    
		$recvarray = explode('|',$recvbuff); 
		
		$result = 100 - $recvarray[0];
		if ( $result != 0 )
			echo "$g_result[$result] 指令执行完毕(服务器:$server 联盟ID:$clubid) 指令:$cmd|$arg1|$arg2|$arg3|$arg4|$string<br>";
		else
			echo "$g_result[$result] 指令执行失败(服务器:$server 联盟ID:$clubid)<br>";
		
		$result_string = $recvarray[1];
		//$result_string = iconv("GB2312", "UTF-8//IGNORE", $recvarray[1] );
		
		// 根据utf8的编码范围，是可以剔除掉非法utf8字符的
		$result_string = preg_replace('/[\x00-\x08\x10\x0B\x0C\x0E-\x19\x7F]'.
 '|[\x00-\x7F][\x80-\xBF]+'.
 '|([\xC0\xC1]|[\xF0-\xFF])[\x80-\xBF]*'.
 '|[\xC2-\xDF]((?![\x80-\xBF])|[\x80-\xBF]{2,})'.
 '|[\xE0-\xEF](([\x80-\xBF](?![\x80-\xBF]))|(?![\x80-\xBF]{2})|[\x80-\xBF]{3,})/S', '', $result_string );
 
		str_replace("'", '"', $result_string);//将json数据中的单引替换成双引
		preg_replace('/,\s*([\]}])/m', '$1', $result_string); // 不能有多余的逗号 如：[1,2,]
echo $result_string;
		$jsonArray = json_decode( $result_string, true );
		$clubid = $jsonArray['clubid'];
		$clubname = $jsonArray['clubname'];
		$prefix = $jsonArray['prefix'];
		$description = $jsonArray['description'];
		$leadername = $jsonArray['leadername'];
		$leaderid = $jsonArray['leaderid'];
		$language = $jsonArray['language'];
		$createtime = $jsonArray['createtime'];
		$battlepower = $jsonArray['battlepower'];
		$questid = $jsonArray['questid'];
		$questvalue = $jsonArray['questvalue'];	
		$member_count = $jsonArray['member_count'];
		$member_limitcount = $jsonArray['member_limitcount'];
		$member_maxcount = $jsonArray['member_maxcount'];
		$killnum = $jsonArray['killnum'];
		$clubpoint = $jsonArray['clubpoint'];
		$tecpoint = $jsonArray['tecpoint'];
		$activity_dragonstate = $jsonArray['activity_dragonstate'];
		$activity_dragontime = $jsonArray['activity_dragontime'];
		$activity_dragonshelter = $jsonArray['activity_dragonshelter'];
		$activity_dragonactornum = $jsonArray['activity_dragonactornum'];
		$activity_dragonpoint = $jsonArray['activity_dragonpoint'];
		$technology_upgradekind = $jsonArray['technology_upgradekind'];
		$technology_begintime = $jsonArray['technology_begintime'];
		
		// 基本信息
		echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>联盟ID</td><td class='Data DataRightAlign'>$clubid</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>名称</td><td class='Data DataRightAlign'>$clubname</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>简称</td><td class='Data DataRightAlign'>$prefix</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>公告</td><td class='Data DataRightAlign'>$description</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>领主名称</td><td class='Data DataRightAlign'>$leadername</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>领主ID</td><td class='Data DataRightAlign'>$leaderid</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>语言</td><td class='Data DataRightAlign'>$language</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>创建时间</td><td class='Data DataRightAlign'>$createtime</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>战力</td><td class='Data DataRightAlign'>$battlepower</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>联盟任务</td><td class='Data DataRightAlign'>$questid</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>联盟任务数值</td><td class='Data DataRightAlign'>$questvalue</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>当前成员</td><td class='Data DataRightAlign'>$member_count</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>最大成员</td><td class='Data DataRightAlign'>$member_limitcount</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>历史最大成员</td><td class='Data DataRightAlign'>$member_maxcount</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>击杀人数</td><td class='Data DataRightAlign'>$killnum</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>联盟积分</td><td class='Data DataRightAlign'>$clubpoint</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>联盟科技点</td><td class='Data DataRightAlign'>$tecpoint</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>之怒状态</td><td class='Data DataRightAlign'>$activity_dragonstate</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>之怒时间</td><td class='Data DataRightAlign'>$activity_dragontime</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>之怒人数</td><td class='Data DataRightAlign'>$activity_dragonactornum</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>之怒点数</td><td class='Data DataRightAlign'>$activity_dragonpoint</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>当前科技升级</td><td class='Data DataRightAlign'>$technology_upgradekind</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>当前科技时间</td><td class='Data DataRightAlign'>$technology_begintime</td></tr>";
		echo"</table>";
		
		
		// 科技列表
		echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
		echo"<th class='DataHeader'>科技名称</th>";
		echo"<th class='DataHeader'>等级</th>";
		echo"<th class='DataHeader'>星级</th>";
		echo"<th class='DataHeader'>经验</th>";
		$technologycount = count($jsonArray['technology']);
		for ( $tmpi = 0; $tmpi < $technologycount; $tmpi++ )
		{
			$name = $jsonArray['technology'][$tmpi]['name'];
			$level = $jsonArray['technology'][$tmpi]['level'];
			$star = $jsonArray['technology'][$tmpi]['star'];
			$exp = $jsonArray['technology'][$tmpi]['exp'];
			echo "<tr class='Record_2'><td class='Data DataRightAlign'>$name</td><td class='Data DataRightAlign'>$level</td><td class='Data DataRightAlign'>$star</td><td class='Data DataRightAlign'>$exp</td></tr>";
		}
		echo"</table>";
	

	}
	fclose ($sock);

?>

</div>
</body>
</html>
