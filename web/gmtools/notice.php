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
 function sel(a){
  o=document.getElementsByName(a)
  for(i=0;i<o.length;i++)
  o[i].checked=event.srcElement.checked
 }
</script>
</head>

<body>
<?php
$db = new dbstuff;
$pconnect = 0;

$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

$sql="desc notice";
$query = $db->query($sql);
$field_arr = array();
$field_count = 0;
while($row = $db->fetch_row($query))
{
	$field_arr[]=$row[0];
	$field_count++;
}

$sql="SELECT * FROM notice order by id";
$query = $db->query($sql);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";

echo "<tr class='TableHeader'>";
echo "<th class='DataHeader'> </th>";
for( $tmpi = 0; $tmpi < $field_count; $tmpi++ )
{
	echo "<th class='DataHeader'>$field_arr[$tmpi]</th>";
}
echo "</tr>";
//echo "<tr class='TableHeader'><th class='DataHeader'>选项</th><th class='DataHeader'>服务器ID</th><th class='DataHeader'>服务器名称</th><th class='DataHeader'>服务器IP</th></tr>";

$recno = 1;
while($row = $db->fetch_row($query))
{
	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;
	echo"<tr class='Record_$recno'>";

	echo"<td class='Data DataRightAlign'><a href='notice_mod.php?p=1&id=$row[0]'>修改</a></td>";
	for( $tmpi = 0; $tmpi < $field_count; $tmpi++ )
	{
		if( $tmpi >= 2 )
			echo"<td class='Data DataLeftAlign'><div style='width:300px;overflow: hidden;height:14px;' title='$row[$tmpi]' >$row[$tmpi]</div></td>";
		else if( $tmpi == 1 )
			echo"<td class='Data DataLeftAlign'><div style='width:100px;overflow: hidden;height:14px;' title='$row[$tmpi]' >$row[$tmpi]</div></td>";
		else
			echo"<td class='Data DataRightAlign'>$row[$tmpi]</td>";
	}
	echo"</tr>";
}
echo"</table>";
echo "<a href='notice_mod.php?p=0'>添加</a>";

$db->close();
?>

</form>
</body>
</html>
