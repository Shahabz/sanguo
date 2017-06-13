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

<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<script language="javascript">
 function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
 }
</script>
</head>

<body>
<a href="awardad.php" type=_blank>操作</a>
<?php
echo "<form id='form1' name='form1' method='post' action='search_action.php'>";

$db = new dbstuff;
$pconnect = 0;
$remote_ip=$dbhost;
$dbu = $dbuser;
$dbp = $dbpw;
$dbd = $dbname;
$db->connect( $remote_ip, $dbu, $dbp, $dbd, 0 );

$sql="SELECT * FROM server_list where id > 100 order by id";
$query = $db->query($sql);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>选项</th><th class='DataHeader'>服务器ID</th><th class='DataHeader'>服务器名称</th><th class='DataHeader'>服务器IP</th></tr>";
$recno = 2;

while($row = $db->fetch_row($query))
{

	$sysid=$row[0];
	$sysname=$row[1];
	$sysip=$row[2];
	$sysport=$row[3];

	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;

	echo"<tr class='Record_$recno'>";
	echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysid' ></td>";
	echo"<td class='Data DataRightAlign'>$sysid</td>";
	echo"<td class='Data DataRightAlign'>$sysname</td>";
	echo"<td class='Data DataRightAlign'>$sysip:$sysport</td>";
	echo"</tr>";

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
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">玩家ID</th>
		<td class="Data DataRightAlign">
		<input type="text" id="actorid" name="actorid" size="8" value=""/>
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign">时间</th>
		<td class="Data DataRightAlign">
		<input type="text" id="indata" name="indata" size="20" value="<?php echo date("Y-m-d"); ?>"/>
		<input type="text" id="intime" name="intime" size="20" value="00:00:00"/>-
		<input type="text" id="intime2" name="intime2" size="20" value="23:59:59"/>
	    </td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">指定对象</th>
		<td class="Data DataRightAlign">
		<input type="text" id="object" name="object" size="8" value=""/>
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign">途径</th>
		<td class="Data DataRightAlign">
	    <select id="oppath" name="oppath" onChange="opselect(this)">
	      <option value="-1">任何途径</option>
	      <option value="0">普通(系统)</option>
	      <option value="2">抽卡</option>
	      <option value="4">商店</option>
	      <option value="7">每日任务</option>
	      <option value="8">脚本奖励</option>
	      <option value="9">刷怪奖励</option>
	      <option value="10">修炼</option>
	      <option value="11">通灵石</option>
	      <option value="12">每日购买</option>
	      <option value="14">排名奖</option>
	      <option value="15">商城</option>
	      <option value="16">竞技场</option>
	      <option value="17">装备合成</option>
	      <option value="18">装备强化</option>
	      <option value="20">宝玉合成</option>
	      <option value="22">装备制造</option>
	      <option value="23">装备打造</option>
	      <option value="25">充值</option>
	      <option value="26">悬赏</option>
	      <option value="28">请神活动</option>
	      <option value="30">宠物强化</option>
	      <option value="31">使用物品</option>
	      <option value="32">战斗加速</option>
	      <option value="33">新手卡</option>
	      <option value="34">签到</option>
	      <option value="35">猫神</option>
	      <option value="37">附魔</option>
	      <option value="38">精炼</option>
	      <option value="39">首充</option>
	      <option value="40">伙伴召唤</option>
	      <option value="41">伙伴技能</option>
	      <option value="42">伙伴进阶</option>
	      <option value="43">伙伴守卫</option>
	      <option value="44">邮件</option>
	      <option value="45">伙伴分解</option>
	      <option value="46">连续登陆</option>
	      <option value="49">充值赠送</option>
	      <option value="50">主线</option>
	      <option value="84">活动</option>
	      <option value="85">膜拜</option>
	      <option value="86">温泉</option>
	    </select>
	    </td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">选项</th>
		<td class="Data DataRightAlign">
		<input type=checkbox onclick=sel('op[]')>全选
		<input name='op[]' type='checkbox' value='4' > 经验
		<input name='op[]' type='checkbox' value='1' > 金钱
		<input name='op[]' type='checkbox' value='39' > 钻石
		<input name='op[]' type='checkbox' value='47' > 体力
		<input name='op[]' type='checkbox' value='40' > 奥义
		<input name='op[]' type='checkbox' value='6' > 伙伴
		<input name='op[]' type='checkbox' value='2' > 得物品
		<input name='op[]' type='checkbox' value='3' > 失物品
		<input name='op[]' type='checkbox' value='5' > 技能升级
		<input name='op[]' type='checkbox' value='9' > 角色升级
		<input name='op[]' type='checkbox' value='15' > 装备升级
		<input name='op[]' type='checkbox' value='37' > 活动参与
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign">参数区间</th>
		<td class="Data DataRightAlign">
		参数1：<input type="text" id="value_min1" name="value_min1" size="8" value="0"/>-
		<input type="text" id="value_max1" name="value_max1" size="8" value="0"/>
		参数2：<input type="text" id="value_min2" name="value_min2" size="8" value="0"/>-
		<input type="text" id="value_max2" name="value_max2" size="8" value="0"/>
		参数3：<input type="text" id="value_min3" name="value_min3" size="8" value="0"/>-
		<input type="text" id="value_max3" name="value_max3" size="8" value="0"/>
	    </td>
	</tr>
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign">页号</th>
		<td class="Data DataRightAlign">
		<input type="text" id="page" name="page" size="8" value="1"/>
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