<?php 
require_once 'db.inc.php';

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

$p=$_GET["p"];
$id=$_GET["id"];

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

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>字段</th><th class='DataHeader'>数据</th></tr>";

$value_arr = array();
if( $p == 1 )
{
	$sql="SELECT * FROM notice where id='$id'";
	$query = $db->query($sql);
	if($row = $db->fetch_row($query))
	{
		for( $tmpi = 0; $tmpi < $field_count; $tmpi++ )
		{
			$value_arr[] = $row[$tmpi];
		}
	}
}
$db->close();

echo "<form id='form1' name='form1' method='post' action='notice_action.php'>";
echo "<input type='hidden' name='p' value='$p'>";
$recno = 1;
for( $tmpi = 0; $tmpi < $field_count; $tmpi++ )
{
	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;
	echo"<tr class='Record_$recno'>";
	echo "<th class='Data DataLefttAlign'>$field_arr[$tmpi]</th>";

	if( $tmpi == 0 )
	{
		if( $p == 1 )
			echo "<td class='Data DataLefttAlign'><input type='text' name='$field_arr[$tmpi]' id='$field_arr[$tmpi]' value='$value_arr[$tmpi]' style='width:800px;' readonly='true'>";
		else
			echo "<td class='Data DataLefttAlign'><input type='text' name='$field_arr[$tmpi]' id='$field_arr[$tmpi]' value=''>";
	}	
	else if( $tmpi < 2 )
	{
		if( $p == 1 )
			echo "<td class='Data DataLefttAlign'><input type='text' name='$field_arr[$tmpi]' id='$field_arr[$tmpi]' value='$value_arr[$tmpi]' style='width:800px;'>";
		else
			echo "<td class='Data DataLefttAlign'><input type='text' name='$field_arr[$tmpi]' id='$field_arr[$tmpi]' value=''>";
	}
	else
	{
		if( $p == 1 )
			echo "<td class='Data DataLefttAlign'><textarea name='$field_arr[$tmpi]' cols='110' rows='4' style='background-color:#ffffff '>$value_arr[$tmpi]</textarea>";
		else
			echo "<td class='Data DataLefttAlign'><textarea name='$field_arr[$tmpi]' cols='110' rows='4' style='background-color:#ffffff '></textarea>";
	}
	echo "</td></tr>";
}

echo"<tr class='Record_1'>";
echo "<th class='Data DataLefttAlign'>";
if( $p == 1 )
	echo "<input type='submit' id='submit' name='submit' value='修改' />";
else
	echo "<input type='submit' id='submit' name='submit' value='添加' />";
echo "</th>";
echo"</form>";

echo "<td class='Data DataRightAlign'>";
if( $p == 1 )
{
	echo "<form id='form2' name='form2' method='post' action='notice_action.php'>";
	echo "<input type='hidden' name='p' value='2'>";
	echo "<input type='hidden' name='id' value='$id'>";
	echo "<input type='submit' id='submit' name='submit' value='删除' />";
	echo"</form>";
}
echo "</td></tr>";

echo"</table>";

?>

</form>
</body>
</html>
