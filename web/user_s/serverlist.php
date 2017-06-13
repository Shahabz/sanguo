<?php
require_once '../db.inc.php';
//error_reporting(E_ALL); 

function getlist( $db, $uid )
{
	$count = 0;
	$list = "\"list\": [";
	$sql="select id, ip, port, new, close, king_sevname, king_name, king_country from `gate_list` order by id asc;";
	$query = $db->query($sql);
	while ( $sth = $db->fetch_row($query) )
	{
		$serverid = $sth[0];
		$host = $sth[1];
		$port = $sth[2];
		$new = $sth[3];
		$close = $sth[4];
		$king_sevname = $sth[5];
		$king_name = $sth[6];
		$king_country = $sth[7];
		
		// 白名单
		if ( admincheck( $uid ) == 1 )
		{
			$close = 0;
		}
		
		if ( $count > 0 )
		{
				$list .= ",";
		}
		$count += 1;
		$list .= "{\"s\":$serverid, \"h\":\"$host\" , \"p\":$port , \"n\":$new, \"cl\":$close, \"ks\":\"$king_sevname\", \"kn\":\"$king_name\", \"kc\":$king_country}";
	}
	$db->free_result($query);
	$list .= "]";
	return $list;
}


// 入口
$command = 0;
$uid = 0;
if (!get_magic_quotes_gpc())
{
	$command=addslashes(@$_GET['c']);
	$uid=addslashes(@$_GET['uid']);
}
else
{
	$command=@$_GET['c'];
	$uid=@$_GET['uid'];
}

$dbisopen = 0;
if( $result == 0 )
{
	#打开数据库
	$db = new dbstuff;
	$pconnect = 1;
	$db->connect($dbhost, $dbuser, $dbpw, $dbname, $pconnect);
	$dbhost = $dbuser = $dbpw = $dbname = $pconnect = NULL;
	$dbisopen = 1;
}

$msg = getlist( $db, $uid );

if( $dbisopen )
{
	$db->close();
}


echo "{ \"cmd\":\"$command\", $msg }";

?>
