<?php
require_once 'db.inc.php';
header("Content-type:text/txt; charset=utf-8");
$host=$_POST["host"];
$port=$_POST["port"];
$pass=$_POST["pass"];

$cmd = "/usr/local/ant/bin/ant -buildfile ".$http_path."/undead_upload/resetupagent.xml -verbose -Dhost=\"$host\" -Dport=\"$port\" -Dpass=\"$pass\" -Dwebroot=\"$http_path\"";

putenv( "JAVA_HOME=/usr/lib/jvm/jdk1.8.0_111" );
putenv( "ANT_HOME=/usr/local/ant" );
$string = system($cmd);
print $string;
//exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
?>
