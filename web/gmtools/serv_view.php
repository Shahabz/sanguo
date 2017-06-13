<?php
//error_reporting(0);
$USERCMDC_CFGINFO = 14;

$remote_port="33900";
header("Content-type:text/txt; charset=utf-8");
if (!get_magic_quotes_gpc())
{
	$host=addslashes(@$_GET['h']);
	$serverdir=addslashes(@$_GET['dir']);
}
else
{
	$host=@$_GET['h'];
	$serverdir=@$_GET['dir'];
}

$sock = fsockopen ($host, $remote_port, $errno,$errstr,3 );
if( !$sock )
{
	echo "无法连接Agent";
}
else
{
	$buffer = pack( "l6", $USERCMDC_CFGINFO, 0, 0, 0, 0, strlen($serverdir) );
	fputs( $sock, $buffer );
	fputs( $sock, $serverdir );

	$result = fread( $sock, 24 );
	$data = unpack( "l6", $result );
	if( $data[6] > 0 )
	{
		$strdata = fread( $sock, $data[6] );
	}
	echo $strdata;
	fclose ($sock);
}

?>
