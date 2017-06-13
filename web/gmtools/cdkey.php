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
function checkconfirm()
{
	var info = document.forms[0].systalk_info.value;
	var r=confirm(info+"\n要立即提交吗？");

	return r;
}

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

function del_action( jsonmsg ) 
 { 
 	 	var pwd = prompt("请输入del方可删除","");
    if ( pwd != null && pwd != "" )
    {
      if (pwd=="del")
      {
        //document.form1.submit();
        post( 'cdkey_action.php', jsonmsg );
        return true;
      }
      else
      {
        alert("错误！");
        return false;
      }
    }
    return false;
 }
 
 function download_action( jsonmsg ) 
 { 
    post( 'cdkey_action.php', jsonmsg );
    return true;
 }
</script>
</head>

<body align="center">
<div width="980" align="center">

<a href="queryad.php" type=_blank>返回</a>
<?php

echo"</br>";
echo"<tr class='Record_$recno'>"."已有兑换码"."</tr>";
echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo"<th class='DataHeader'>批次号</th>";
echo"<th class='DataHeader'>该批次起ID</th>";
echo"<th class='DataHeader'>该批次止ID</th>";
echo"<th class='DataHeader'>总数</th>";
echo"<th class='DataHeader'>限制系统</th>";
echo"<th class='DataHeader'>限制渠道</th>";
echo"<th class='DataHeader'>奖励组</th>";
echo"<th class='DataHeader'>过期时间</th>";

$db = new dbstuff;
$pconnect = 0;
$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

$sql="SELECT MIN(id), MAX(id), COUNT(id), cdkey_index, limit_sys, limit_channel, awardgroup, overduetime FROM card group by cdkey_index, limit_channel";
$query = $db->query($sql);

$recno = 2;
$firstchecked = 0;
while($row = $db->fetch_row($query))
{
	$minid=$row[0];
	$maxid=$row[1];
	$count=$row[2];
	$cdkey_index=$row[3];
	$limit_sys=$row[4];
	$limit_channel=$row[5];
	$awardgroup=$row[6];
	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;
	echo"<tr class='Record_$recno'>";
	echo"<td class='Data DataRightAlign'>$cdkey_index</td>";
	echo"<td class='Data DataRightAlign'>$minid</td>";
	echo"<td class='Data DataRightAlign'>$maxid</td>";
	echo"<td class='Data DataRightAlign'>$count</td>";
	
	if ( $limit_sys == 0 )
		echo"<td class='Data DataRightAlign'>全系统</td>";
	else if ( $limit_sys == 1 )
		echo"<td class='Data DataRightAlign'>ios</td>";
	else
		echo"<td class='Data DataRightAlign'>android</td>";
		
	echo"<td class='Data DataRightAlign'>$g_channeid[$limit_channel]</td>";
	echo"<td class='Data DataRightAlign'>$awardgroup</td>";
	
	if ( $row[7] == 0 )
	{
		echo"<td class='Data DataRightAlign'>永久</td>";
	}
	else
	{
		$overduetime=date("Y-m-d H:i:s", $row[7]);
		echo"<td class='Data DataRightAlign'>$overduetime</td>";
	}
	
	echo"<td class='Data DataRightAlign'>";
	//  \"javascript:post( 'cdkey_action.php', { op:'2', beginid:'$minid', endid:'$maxid' } );\"
	echo"<a href = \"javascript:del_action( { op:'2', beginid:'$minid', endid:'$maxid' } );\" target='_blank'>删除该批次</a>";
	echo"</td>";
		
	echo"<td class='Data DataRightAlign'>";
	echo"<a href = \"javascript:download_action( { op:'3', beginid:'$minid', endid:'$maxid' } );\" target='_blank'>下载</a>";
	echo"</td>";
	
	echo"</tr>";
}
echo"</table>";
echo"</br>";
echo"</br>";
echo"</br>";
echo"<tr class='Record_$recno'>"."创建兑换码，兑换码由4-N个数字（卡的ID号）+3个字符组成，例子1000xyz"."</tr>";
?>             


<form id='form1' name='form1' method='post' action='cdkey_action.php' target='_blank'>
<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >

	<tr class="Record_1" id="proc1">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value1">批次(1开始，最大127个批次，省着点用)</th>
		<td class="Data DataRightAlign">
		<input type="text" id="cdkey_index" name="cdkey_index" size="8" value=""/>
		<input type="hidden" id="op" name="op" size="8" value="1"/>
	  </td>
	</tr>
	
	<tr class="Record_1" id="proc2">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value2">该批次兑换码起始ID（最小四位数字）</th>
		<td class="Data DataRightAlign">
		<input type="text" id="cdkey_id" name="cdkey_id" size="8" value=""/>
	    </td>
	</tr>
	
	<tr class="Record_1" id="proc3">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value3">限制系统</th>
		<td class="Data DataRightAlign">
		 <select id="limit_sys" name="limit_sys" >
		 		<option value="0">无限制</option>
	      <option value="1">ios</option>
	      <option value="2">android</option>
	    </select>
	   </td>
	</tr>
	
	<tr class="Record_1" id="proc4">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value4">限制渠道</th>
		<td class="Data DataRightAlign">
		 <select id="limit_channel" name="limit_channel" >
<?php
foreach($g_channeid as $k=>$v)
{
	echo "<option value='".$k."'";
	if( $proc_value3 == $k ) echo "selected";
	echo ">[".$k."]".$v."</option>";
}
?>
	    </select>
	   </td>
	</tr>
	
	<tr class="Record_1" id="proc5">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value5">奖励</th>
		<td class="Data DataRightAlign">
		<input type="text" id="awardgroup" name="awardgroup" size="8" value=""/>
	   </td>
	</tr>
	
	<tr class="Record_1" id="proc6">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value6">有效期（天，0代表不限时）</th>
		<td class="Data DataRightAlign">
			<input type="text" id="overduetime" name="overduetime" size="8" value=""/>
		</td>
	</tr>
	
	<tr class="Record_1" id="proc7">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value7">数量（实际点，发不了那么多，别生成那么多，浪费）</th>
		<td class="Data DataRightAlign">
			<input type="text" id="totalcount" name="totalcount" size="8" value=""/>
		</td>
	</tr>
	
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"> </th>
		<td class="Data DataRightAlign">
		<input type="submit" id="submit" name="submit" value="提交" />
	    </td>
	</tr>
</table>

</form>

<script language="javascript">
function opselect(obj)
{

}

	opselect(document.getElementById('command'));
</script>
</div>
</body>
</html>