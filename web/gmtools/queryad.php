<?php 
require_once 'db.inc.php';

/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
date_default_timezone_set("Etc/GMT");
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
		.RedText {color:#ff0000 ;text-shadow: 2px 2px 5px #999;}
		.GreenText {color:#008800 ;text-shadow: 2px 2px 5px #999;}
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
if( $_SESSION["adminid"] >= 100 )
{
	echo "<a href='awardad.php' type=_blank>服务器指令</a>	";
	echo "<a href='clock.php' type=_blank>定时器</a>	";
	echo "<a href='notice.php' type=_blank>消息管理</a>	";
	echo "<a href='tracklog.php' type=_blank>tracklog</a>	";
	echo "<a href='cdkey.php' type=_blank>兑换码</a>	";
}
?>

<?php
if( $_SESSION["adminid"] >= 50 )
{
	echo "<a href='tj6.php' type=_blank>留存</a>	";
	echo "<a href='tj1.php' type=_blank>统计</a>	";
}

echo "<a href='r_name.php' type=_blank>举报名称</a>	";
echo "<a href='r_chat.php' type=_blank>举报聊天</a>	";
echo "<a href='r_head.php' type=_blank>举报头像</a>	";
echo "<a href='http://kor.res.gohogame.com/head/review.php' type=_blank>头像审核</a>	";
?>

<script language="javascript">
 function actor_queryad() 
 { 
    document.form1.action="queryad_action.php"; 
    document.form1.submit(); 
 } 
     
 function club_queryad() 
 { 
    document.form1.action="queryad_club.php"; 
    document.form1.submit(); 
 }
 
 function city_queryad() 
 { 
    document.form1.action="queryad_city.php"; 
    document.form1.submit(); 
 }
 
 function pay_queryad() 
 { 
    document.form1.action="queryad_pay.php"; 
    document.form1.submit(); 
 }
 
 function server_queryad() 
 { 
    document.form1.action="queryad_server.php"; 
    document.form1.submit(); 
 }
 
 function chat_queryad()
 { 
    document.form1.action="queryad_chat.php";
    document.form1.submit(); 
 }
</script>
    
<?php
echo "<a href='user_changepw.php'>修改密码<a>　";
echo "<a href='login_ad.php'>重新登录<a>　";
if( $_SESSION["adminid"] >= 120 )
{
	echo "<a href='user_ad.php'>用户管理<a>　";
	echo "<a href='serv.php'>服务器管理<a>　";
	echo "<a href='http://bug.gohogame.com/serv_client_turn.php'>客户端管理<a>　";
}


echo "<form id='form1' name='form1' method='post' action=''>";

function pay_remote( $dbh, $dbu, $dbp, $dbd, $actorid, $amount )
{
	$result = 0;
	#打开数据库
	$db = new dbstuff;
	$pconnect = 0;
	$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

	$token = $amount*10;
	$sql="update actor set token=token+'$token',totle_token=totle_token+'$token' where actorid='$actorid'";
	if(!( $db->query($sql) ))
	{
		$result = -1;
	}
	$db->close();
	return $result;
}

$db = new dbstuff;
$pconnect = 0;
$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="SELECT * FROM server_list where id > 100 order by id";
$query = $db->query($sql);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>选项</th><th class='DataHeader'>服务器ID</th><th class='DataHeader'>服务器名称</th><th class='DataHeader'>服务器IP</th><th class='DataHeader'>状态</th><th class='DataHeader'>在线</th></tr>";
$recno = 2;
$firstchecked=0;
while($row = $db->fetch_row($query))
{

	$sysid=$row[0];
	$sysname=$row[1];
	$sysip=$row[2];
	$sysport=$row[3];
	$sysdbh=$row[4];
	$sysdbu=$row[5];
	$sysdbp=$row[6];
	$sysdbd=$row[7];

	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;

	echo"<tr class='Record_$recno'>";
	if( $firstchecked == 0 )
		echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysid' checked></td>";
	else
		echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysid'></td>";
	echo"<td class='Data DataRightAlign'>$sysid</td>";
	echo"<td class='Data DataRightAlign'>$sysname</td>";
	echo"<td class='Data DataRightAlign'>0.0.0.0</td>";
	
	$sock = fsockopen( $sysip,$sysport,$errno,$errstr,3 );
	if( !$sock )
	{
		echo"<td class='RedText'>断开</td>";
	}
	else
	{
		fputs( $sock, "\0\0" );
		$result = fgets( $sock );
		fclose ($sock);

		$arr = explode("|", $result );		
		$player_count  = $arr[0];

		echo"<td class='GreenText'>连接</td>";
	  echo"<td class='GreenText'>$player_count</td>";

	}
	
	echo"</tr>";
	$firstchecked=1;
}
echo"<tr class='TableHeader'>";
echo"<td class='Data DataRightAlign'><input type=checkbox onclick=sel('favor[]')></td>";
echo"<td class='Data DataRightAlign'>全选</td>";
echo"<td class='Data DataLeftAlign'> </td>";
echo"<td class='Data DataLeftAlign'> </td>";
echo"</table>";
$db->close();
?>
<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >

	<tr class="Record_1" id="proc1">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value1">玩家ID</th>
		<td class="Data DataRightAlign">
		<input type="text" id="actorid" name="actorid" value=""/>
	    </td>
	</tr>

	<tr class="Record_2" id="proc2">
		<th class="DataOfPrimaryKey DataRightAlign" id="proc_value2">角色名字</th>
		<td class="Data DataRightAlign">
		<textarea name="systalk_info" cols="60" rows="4" style="background-color:#ffffff "></textarea>
	    </td>
	</tr>
	
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
		<th class="DataOfPrimaryKey DataRightAlign">指令选择</th>
		<td class="Data DataRightAlign">
	    <select id="type" name="type" onChange="opselect(this)">
	      <option value="-1">查询角色</option>
	      <option value="-2">查询联盟</option>
	      <option value="-14">查询城池</option>
	      <option value="-15">角色充值</option>
	      <option value="-3">角色等级分布（最晚延迟1天）</option>
	      <option value="-4">主城等级分布（最晚延迟1天）</option>
	      <option value="-5">充值记录（即时）</option>
	      <option value="-6">消费记录（即时）</option>
	      <option value="-7">消费详情（即时）</option>
	      <option value="-8">使命统计（最晚延迟1天）</option>
	      <option value="-9">任务统计（最晚延迟1天）</option>
	      <option value="-10">查询角色(根据创建时间)</option>
	      <option value="-11">查询角色(根据角色等级)</option>
	      <option value="-13">查询角色(根据充值排序)</option>
	      <option value="-12">监控聊天</option>
	    </select>

	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"> </th>
		<td class="Data DataRightAlign" id="ok">
		<input type="submit" id="submit" name="submit" value="提交" onclick="" />
	    </td>
	</tr>
</table>
</form>

<script language="javascript">
function opselect(obj)
{
	document.getElementById("proc1").style.visibility="hidden";
	document.getElementById("proc2").style.visibility="hidden";
	document.getElementById("proc3").style.visibility="hidden";
	document.getElementById("proc_value1").innerHTML = "";
	document.getElementById("proc_value2").innerHTML = "";
	switch( obj.value )
	{
	case '-1':
		document.getElementById("proc1").style.visibility="visible";
		document.getElementById("proc2").style.visibility="visible";
		document.getElementById("proc_value1").innerHTML = "玩家ID";
		document.getElementById("proc_value2").innerHTML = "角色名字";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='提交' onclick='actor_queryad()' />";
		break;
	case '-2':
		document.getElementById("proc1").style.visibility="visible";
		document.getElementById("proc2").style.visibility="visible";
		document.getElementById("proc_value1").innerHTML = "联盟ID";
		document.getElementById("proc_value2").innerHTML = "联盟名称";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='提交' onclick='club_queryad()' />";
		break;
	case '-14':
		document.getElementById("proc1").style.visibility="visible";
		document.getElementById("proc2").style.visibility="visible";
		document.getElementById("proc_value1").innerHTML = "城池ID";
		document.getElementById("proc_value2").innerHTML = "城池名字";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='提交' onclick='city_queryad()' />";
		break;
	case '-15':
		document.getElementById("proc1").style.visibility="visible";
		document.getElementById("proc2").style.visibility="visible";
		document.getElementById("proc_value1").innerHTML = "角色ID";
		document.getElementById("proc_value2").innerHTML = "角色名称";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='提交' onclick='pay_queryad()' />";
		break;
	case '-3':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='查询' onclick='server_queryad()' />";
		break;
	case '-4':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='查询' onclick='server_queryad()' />";
		break;
	case '-5':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='查询' onclick='server_queryad()' />";
		break;
	case '-6':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='查询' onclick='server_queryad()' />";
		break;
	case '-7':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='查询' onclick='server_queryad()' />";
		break;
	case '-8':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='查询' onclick='server_queryad()' />";
		break;
	case '-9':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='查询' onclick='server_queryad()' />";
		break;
	case '-10':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='提交' onclick='actor_queryad()' />";
		break;
	case '-11':
		document.getElementById("proc2").style.visibility="visible";
		document.getElementById("proc_value2").innerHTML = "角色等级";
		break;
	case '-12':
		document.getElementById("proc3").style.visibility="visible";
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='查看' onclick='chat_queryad()' />";
		break;
	case '-13':
		document.getElementById("ok").innerHTML = "<input type='submit' id='submit' name='submit' value='提交' onclick='actor_queryad()' />";
		break;
	default:
		break;
	}
}
opselect(document.getElementById('type'));
</script>

</div>
</body>
</html>