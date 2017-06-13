<?php
require_once("./phpChart/conf.php");
require_once 'db.inc.php';

$sysdbh="kor-s1.c6ekaseib22e.us-east-1.rds.amazonaws.com";
$sysdbu = "gamewrite";
$sysdbp = "9dceA698";
$logdb = "slg_yq_en_1001_log";

$d=strtotime("-12 Hours");
$str = date("Y-m-d H:i:s", $d);
echo $str;

$arr1 = array();
$arr2 = array();

$db = new dbstuff;
$db->connect( $sysdbh, $sysdbu, $sysdbp, $logdb, 0 );

$sql="select con,game from gamecount where optime >= '$str'";
$query = $db->query($sql);
while($thisrow = $db->fetch_row($query))
{
    $arr1[]=$thisrow[0];
    $arr2[]=$thisrow[1];
}
$db->close();
echo "  num:".$arr1[1];
$pc=new C_PhpChartX(array($arr1), 'simplest_graph');
//$pc->set_series_default(array('renderer'=>'plugin::PieRenderer'));
$pc->set_title(array('text'=>'My Simplest Graph'));
$pc->set_animate(true);
$pc->draw();



?>
