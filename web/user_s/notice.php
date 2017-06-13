<?php
error_reporting(0);
require_once '../db.inc.php';
header("content-type:text/html; charset=utf-8");

if (!get_magic_quotes_gpc())
{
	$command=addslashes(@$_GET['c']);
	$id=addslashes(@$_GET['id']);
	$lang=addslashes(@$_GET['lang']);
	$cn=addslashes(@$_GET['cn']);
	$ext=addslashes(@$_GET['ext']);
}
else
{
	$command=@$_GET['c'];
	$id=@$_GET['id'];
	$lang=@$_GET['lang'];
	$cn=@$_GET['cn'];
	$ext=@$_GET['ext'];
}

$language = $lang."-".$cn;
$array = array(
		"zh" => "zh-cn",
    "zh-cn" => "zh-cn",
    "zh-tw" => "zh-tw",
    "tw-cn" => "zh-tw",
    
    "en" => "en-us",
    "en-us" => "en-us",
    
    "de" => "de-de",
    "de-de" => "de-de",
    
    "ar" => "ar-ar",
    "ar-ar" => "ar-ar",
    
    "fr" => "fr-fr",
    "fr-fr" => "fr-fr",
    
    "pt" => "pt-pt",
    "pt-pt" => "pt-pt",
    
    "ru" => "ru-ru",
    "ru-ru" => "ru-ru",
    
    "es" => "es-es",
    "es-es" => "es-es",
    
    "tr" => "tr-tr",
    "tr-tr" => "tr-tr",
    
    "pl" => "pl-pl",
    "pl-pl" => "pl-pl",
    
    "kr" => "kr-kr",
    "kr-kr" => "kr-kr",
    "ko" => "kr-kr",
    "ko-kr" => "kr-kr",
);

$langName = "en-us";
if ( $array[$language] == null )
{	
	$langName = $array[$lang];
	if ( $langName == null )
			$langName = "en-us";
}
else
{
	$langName = $array[$language];
}

#打开数据库
$db = new dbstuff;
$pconnect = 0;
$db->connect($dbhost, $dbuser, $dbpw, $dbname, $pconnect);
$dbhost = $dbuser = $dbpw = $dbname = $pconnect = NULL;
$db->query("SET NAMES UTF8");

$notice = "";
$sql="select `".$langName."` from notice where id='$id'";
$query = $db->query($sql);
if($sth = $db->fetch_row($query))
{
	$notice = $sth[0];
	$db->free_result($query);
}
$db->close();	
echo "{ \"cmd\":\"$command\", \"notice\":\"$notice\", \"ext\":\"$ext\" }";
?>