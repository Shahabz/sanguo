<?php 
require_once 'db.inc.php';
require_once 'define.php';
error_reporting(0);
/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}


$host = $_GET['host'];
$armyindex = $_GET['armyindex'];

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
		fputs( $sock, "7|2|$armyindex|0|0|$string\0" );
		$recvbuff = "";
	  while(!feof($sock)) 
	  {     
      $data = fread($sock,81920);
      $recvbuff .= $data;        
    }
    
		$recvarray = explode('|',$recvbuff); 
		
		$result = 100 - $recvarray[0];
		if ( $result != 0 )
			echo "$g_result[$result] 指令执行完毕(服务器:$server 角色ID:$actorid) 指令:$cmd|$arg1|$arg2|$arg3|$arg4|$string<br>";
		else
			echo "$g_result[$result] 指令执行失败(服务器:$server 角色ID:$actorid)<br>";
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
//echo $result_string;
		$jsonArray = json_decode( $result_string, true );
		$from_type = $jsonArray['from_type'];
		$from_id = $jsonArray['from_id'];
		$from_posx = $jsonArray['from_posx'];
		$from_posy = $jsonArray['from_posy'];
		$target_type = $jsonArray['target_type'];
		$target_id = $jsonArray['target_id'];
		$target_posx = $jsonArray['target_posx'];
		$target_posy = $jsonArray['target_posy'];
		$state = $jsonArray['state'];
		$statetime = $jsonArray['statetime'];
		$stateduration = $jsonArray['stateduration'];
		$action = $jsonArray['action'];	
		$appdata = $jsonArray['appdata'];
		$posx = $jsonArray['posx'];
		$posy = $jsonArray['posy'];
		$move_total_distance = $jsonArray['move_total_distance'];
		$battlepower =  number_format($jsonArray['battlepower']);
		$carry_wood = number_format($jsonArray['carry_wood']);
		$carry_food = number_format($jsonArray['carry_food']);
		$carry_iron = number_format($jsonArray['carry_iron']);
		$carry_mithril = number_format($jsonArray['carry_mithril']);
		$carry_max = number_format($jsonArray['carry_max']);
		$troops_totalnum = number_format($jsonArray['troops_totalnum']);
		
		// 基本信息
		echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>出发类型</td><td class='Data DataRightAlign'>$from_type</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>出发ID</td><td class='Data DataRightAlign'>$from_id</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>出发坐标</td><td class='Data DataRightAlign'>X:$from_posx,Y:$from_posy</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>目标类型</td><td class='Data DataRightAlign'>$target_type</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>目标ID</td><td class='Data DataRightAlign'>$target_id</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>目标坐标</td><td class='Data DataRightAlign'>X:$target_posx,Y:$target_posy</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>状态</td><td class='Data DataRightAlign'>$state</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>状态时间</td><td class='Data DataRightAlign'>$statetime</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>状态持续时间</td><td class='Data DataRightAlign'>$stateduration</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>动作</td><td class='Data DataRightAlign'>$action</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>附加值</td><td class='Data DataRightAlign'>$appdata</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>当前坐标</td><td class='Data DataRightAlign'>X:$posx,Y:$posy</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>总距离</td><td class='Data DataRightAlign'>$move_total_distance</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>携带魂力</td><td class='Data DataRightAlign'>$carry_wood</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>携带食物</td><td class='Data DataRightAlign'>$carry_food</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>携带铁矿</td><td class='Data DataRightAlign'>$carry_iron</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>携带水银</td><td class='Data DataRightAlign'>$carry_mithril</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>最大携带量</td><td class='Data DataRightAlign'>$carry_max</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>总战力</td><td class='Data DataRightAlign'>$battlepower</td></tr>";
		echo "<tr class='Record_2'><td class='Data DataRightAlign'>总兵力</td><td class='Data DataRightAlign'>$troops_totalnum</td></tr>";
		echo"</table>";
		
		
		echo "<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";	
		echo "<th class='DataHeader'>兵种</th>";
		echo "<th class='DataHeader'>等级</th>";
		echo "<th class='DataHeader'>数量</th>";
		$troopscount = count($jsonArray['troops']);
		for ( $tmpi = 0; $tmpi < $troopscount; $tmpi++ )
		{
			$corps = $jsonArray['troops'][$tmpi]['corps'];
			$level = $jsonArray['troops'][$tmpi]['level'];
			$num = $jsonArray['troops'][$tmpi]['num'];
			$name = $g_CorpsName[$corps];
			echo "<tr class='Record_2'><td class='Data DataRightAlign'>$name</td><td class='Data DataRightAlign'>Lv.$level</td><td class='Data DataRightAlign'>$num</td></tr>";
		}
		echo "</table>";		

	}
	fclose ($sock);

?>

</div>
</body>
</html>
