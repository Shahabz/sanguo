<?php
$dbhost="kor-s1.c6ekaseib22e.us-east-1.rds.amazonaws.com";
$dbname="kor_user";
$dbuser="gamewrite";
$dbpw="9dceA698";
$pconnect="";

$translate_dbhost="kor-s1.c6ekaseib22e.us-east-1.rds.amazonaws.com";
$translate_dbname="kor_translate";
$translate_dbuser="gamewrite";
$translate_dbpw="9dceA698";

class dbstuff {
	var $querynum = 0;
	var $link;
	var $last_dbname = '';
	function connect($dbhost, $dbuser, $dbpw, $dbname = '', $pconnect = 0) {
		if($pconnect) {
			if(!$this->link = @mysql_pconnect($dbhost, $dbuser, $dbpw)) {
				$this->halt('Can not connect to MySQL server');
			}
		} else {
			if(!$this->link = @mysql_connect($dbhost, $dbuser, $dbpw)) {
				$this->halt('Can not connect to MySQL server');
			}
		}

		if($this->version() > '4.1') {
			global $charset, $dbcharset;
			if(!$dbcharset && in_array(strtolower($charset), array('gbk', 'big5', 'utf-8'))) {
				$dbcharset = str_replace('-', '', $charset);
			}

			if($dbcharset) {
				mysql_query("SET character_set_connection=$dbcharset, character_set_results=$dbcharset, character_set_client=binary", $this->link);
			}

			if($this->version() > '5.0.1') {
				mysql_query("SET sql_mode=''", $this->link);
			}
		}

		if($dbname) {
			mysql_select_db($dbname, $this->link);
			$this->last_dbname = $dbname;
		}

	}

	function select_db($dbname) {
		return mysql_select_db($dbname, $this->link);
	}

	function reselect_db() {
		if($this->last_dbname) {
			return mysql_select_db($this->last_dbname, $this->link);
		}
	}

	function fetch_array($query, $result_type = MYSQL_ASSOC) {
		return mysql_fetch_array($query, $result_type);
	}

	function query($sql, $type = '') {
		global $debug, $discuz_starttime, $sqldebug, $sqlspenttimes;

		$func = $type == 'UNBUFFERED' && @function_exists('mysql_unbuffered_query') ?
			'mysql_unbuffered_query' : 'mysql_query';
		if(!($query = $func($sql, $this->link)) && $type != 'SILENT') {
			$this->halt('MySQL Query Error', $sql);
		}


		$this->querynum++;
		return $query;
	}

	function affected_rows() {
		return mysql_affected_rows($this->link);
	}

	function error() {
		return (($this->link) ? mysql_error($this->link) : mysql_error());
	}

	function errno() {
		return intval(($this->link) ? mysql_errno($this->link) : mysql_errno());
	}

	function result($query, $row) {
		$query = @mysql_result($query, $row);
		return $query;
	}

	function num_rows($query) {
		$query = mysql_num_rows($query);
		return $query;
	}

	function num_fields($query) {
		return mysql_num_fields($query);
	}

	function free_result($query) {
		return mysql_free_result($query);
	}

	function insert_id() {
		return ($id = mysql_insert_id($this->link)) >= 0 ? $id : $this->result($this->query("SELECT last_insert_id()"), 0);
	}

	function fetch_row($query) {
		$query = mysql_fetch_row($query);
		return $query;
	}

	function fetch_fields($query) {
		return mysql_fetch_field($query);
	}

	function version() {
		return mysql_get_server_info($this->link);
	}

	function close() {
		return mysql_close($this->link);
	}

	function halt($message = '', $sql = '') {
		print "message:".$message;
		print "sql:".$sql;
	}
}

function checkorderid( $db,$orderid )
{
	$count = 0;
	$sql="SELECT count(*) from paylog where orderid='$orderid'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$count = $sth[0];
	}
	$db->free_result($query);

	return $count;
}

function wlogpay( $db, $orderid, $platid, $serverid, $userid, $actorid, $country, $channel_orderid, $channel_uid, $productid, $goodsid, $currency, $money, $ip )
{
	$sql="INSERT INTO paylog (orderid, platid, serverid, userid, actorid, country, channel_orderid, channel_uid, productid, goodsid, currency, money, ip, optime) values( '$orderid', '$platid', '$serverid', '$userid', '$actorid', '$country', '$channel_orderid', '$channel_uid', '$productid', '$goodsid', '$currency', '$money', '$ip', now())";
	$db->query($sql);
	if( $db->affected_rows() <= 0 )
	{
		return -1;
	}
	return 0;
}

function wlogpayerror( $db, $orderid, $channel_orderid, $channel_uid, $msg )
{
	$myip = $_SERVER["REMOTE_ADDR"];
	$sql="INSERT INTO payerror (orderid, channel_orderid, channel_uid, msg, ip, optime) values('$orderid','$channel_orderid','$channel_uid','$msg','$myip',now())";

	$db->query($sql);
}

function wloglogin( $db, $username, $msg )
{
	$myip = $_SERVER["REMOTE_ADDR"];
	$sql="INSERT INTO loginlog (username,ip,optime,msg) values('$username','$myip',now(),'$msg')";

	$db->query($sql);
}

function wlogaccess( $db, $username, $msg )
{
	$myip = $_SERVER["REMOTE_ADDR"];
	$sql="INSERT INTO accesslog (username,ip,optime,msg) values('$username','$myip',now(),'$msg')";

	$db->query($sql);
}

function wlogpush( $db, $key, $action, $lang )
{
	$sql="INSERT INTO pushlog (`key`,`action`,`lang`,`optime`) values('$key','$action','$lang',now())";

	$db->query($sql);
}

function admincheck( $uid )
{
	// °×Ãûµ¥
	if ( $uid == "311448572" ||  
	$uid == "311460844" || 
	$uid == "310824056" || 
	$uid == "315457392" ||  
	$uid == "313411070" ||  
	$uid == "311676614" ||
	$uid == "312856036" ||
	$uid == "311459358" ||
	$uid == "311483104" ||
	$uid == "315585128" ||
	$uid == "313667380" ||
	$uid == "311508084" )
	{
		return 1;
	}
	return 0;
}

?>
