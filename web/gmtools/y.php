<?php
require_once 'db.inc.php';
error_reporting(0);
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); 
header("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT"); 
header("Cache-Control: no-cache, must-revalidate"); 
header("Pramga: no-cache"); 
echo "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />";

$USERCMDC_UPLOAD_BEGIN = 2;
$USERCMDC_UPLOAD_FILE = 3;

$USERCMDC_SHELL = 99;

$LONG_CONNECT = 9999;

$remote_port="33900";

$uploadpath=$http_path."/undeadres";

function upload_file( $sock, $sysname, $upfilename, $sysid )
{
	global $uploadpath;
	global $USERCMDC_UPLOAD_BEGIN;
	global $USERCMDC_UPLOAD_FILE;
	global $LONG_CONNECT;
	
	$return_value = 0;
	
	set_time_limit(0);

	$step = 0;
	// 不是数字16，是字母l和数字6
	$buffer = pack( "l6", $USERCMDC_UPLOAD_BEGIN, 0, 0, 0, $LONG_CONNECT, strlen($upfilename) );
	fputs( $sock, $buffer );
	fputs( $sock, $upfilename );
	$result = fread( $sock, 24 );
	$data = unpack( "l6", $result );
		
	$size = filesize( "$uploadpath/$sysname/$upfilename" );
	$myfile = fopen( "$uploadpath/$sysname/$upfilename", "r");
	if( !$myfile )
	{
		echo "无法打开$uploadpath/$sysname/$upfilename(或为web权限不足访问该文件).";
		return;
	}

	$totlelen = 0;
	$last_step = 0;
	while(!feof($myfile))
	{
		$filedata = fread( $myfile, 2000 );
		$datalen = strlen($filedata);

		$buffer = pack( "l6", $USERCMDC_UPLOAD_FILE, 1, 0, 0, $LONG_CONNECT, $datalen );
		fputs( $sock, $buffer );
		fputs( $sock, $filedata );

		$totlelen += $datalen;

		$step = floor($totlelen*150/$size);
		if( $step > $last_step )
		{
			// ■
			$outtext = "$upfilename";
			$outtext .= "<div style=\"padding:0;background-color:white;border:1px solid navy;width:150px\">";
			$outtext .= "<div style=\"padding:0;background-color:#FFCC66;border:0;width:".$step."px;text-align:center;height:8px\"></div>";
			$outtext .= "</div>";
			echo "<script>set_innerHTML('progress".$sysid."','".$outtext."')</script>";
			ob_flush();
			flush();
			$last_step = $step;
		}

	}
	fclose($myfile);

	$buffer = pack( "l6", $USERCMDC_UPLOAD_FILE, 0, 0, 0, $LONG_CONNECT, 0 );
	fputs( $sock, $buffer );
	$result = fread( $sock, 24 );
	$data = unpack( "l6", $result );

	if( $data[6] > 0 )
	{
		$md5string = fread( $sock, $data[6] );
	}

	$localmd5 = md5_file("$uploadpath/$sysname/$upfilename");
	if( strcmp( $md5string, $localmd5 ) == 0 )
	{
		$outtext = "<span class=GreenText>".$upfilename."上传完毕</span>";
		echo "<script>set_innerHTML('progress".$sysid."','".$outtext."')</script>";
	}
	else
	{
		$outtext = "<span class=RedText>".$upfilename."md5值不匹配</span>";
		echo "<script>set_innerHTML('progress".$sysid."','".$outtext."')</script>";
		$return_value = 1;
	}
	ob_flush();
	flush();
	return $return_value;
}

function remote_run( $sock, $cmdstr )
{
	global $USERCMDC_SHELL;
	global $LONG_CONNECT;

	$buffer = pack( "l6", $USERCMDC_SHELL, 0, 0, 0, $LONG_CONNECT, strlen($cmdstr) );
	fputs( $sock, $buffer );
	fputs( $sock, $cmdstr );

	$result = fread( $sock, 24 );
	$data = unpack( "l6", $result );
	$strdata = "";
	if( $data[6] > 0 )
	{
		$strdata = fread( $sock, $data[6] );
	}

	if( $data[1] < 0 )
	{
		echo "<span class=RedText>执行失败 $strdata</span>";
	}
	else
	{
		echo "<span class=GreenText>执行成功 $strdata</span>";
	}
}

if (!get_magic_quotes_gpc())
{
	$command=addslashes(@$_GET['c']);
	$host=addslashes(@$_GET['h']);
	$sysid=addslashes(@$_GET['id']);
	$ver_ios=addslashes(@$_GET['v1']);
	$ver_android=addslashes(@$_GET['v2']);
}
else
{
	$command=@$_GET['c'];
	$host=@$_GET['h'];
	$sysid=@$_GET['id'];
	$ver_ios=@$_GET['v1'];
	$ver_android=@$_GET['v2'];
}


$len = strlen( $command );

$sock = fsockopen ($host, $remote_port, $errno,$errstr,3 );
if( !$sock )
{
	echo "<a href='setupagent.php?h=$host' target=_blank>安装Agent</a>";
}
else
{
	$arr=explode("|", $command );
	if( count($arr) < 3 )
	{
		echo "Error";
	}
	
	// $arr[0] 服务器安装目录
	// $arr[1] 状态 0:查询 1:上传 2:设定版本
	// $arr[2] android/ios
	// 查询
	if( $arr[1] == 0 )
	{
		echo "<span class=GreenText>准备就绪</span>";
		ob_flush();
		flush();
	}
	else if( $arr[1] == 1 )
	{
		$myver = $ver_android;
		if( $arr[2] == "ios" )
			$myver = $ver_ios;
		if( upload_file( $sock, $arr[2], "ver_".$arr[2], $sysid ) == 1 )
			exit(0);
		if( upload_file( $sock, $arr[2], $myver.".tar.gz", $sysid ) == 1 )
			exit(0);
		remote_run( $sock, "tar -zxf /usr/local/iagent/upload/".$myver.".tar.gz -C ".$arr[0] );
	}
	else if( $arr[1] == 2 )
	{
		remote_run( $sock, "cp -rf /usr/local/iagent/upload/ver_".$arr[2]." ".$arr[0] );
	}
	else if( $arr[1] == 3 )
	{
		$myver = $ver_android;
		if( $arr[2] == "ios" )
			$myver = $ver_ios;
		if( upload_file( $sock, $arr[2], "ver_".$arr[2], $sysid ) == 1 )
			exit(0);
		if( upload_file( $sock, $arr[2], $myver.".tar.gz", $sysid ) == 1 )
			exit(0);
		remote_run( $sock, "cp -rf /usr/local/iagent/upload/".$myver.".tar.gz ".$arr[0] );
		remote_run( $sock, "cp -rf /usr/local/iagent/upload/ver_".$arr[2]." ".$arr[0] );
	}

	fclose ($sock);
}


?>
