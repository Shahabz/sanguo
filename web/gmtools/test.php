<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml">
 <head>
 <title>hello</title>
 </head>
 <body>
 <div style="float:left;background-color:#FFFFFF;width:200px;height:4px;padding:1px;border:#CCCCCC 1px solid;overflow:hidden;">
     <div id="jindu" style="float:left;width:0;height:4px;background-color:#669966;overflow:hidden;">&nbsp;</div>
   </div>

<?php
$remote_ip="192.168.1.133";
$remote_port="33900";

$USERCMDC_UPLOAD_BEGIN = 1;
$USERCMDC_UPLOAD_FILE = 2;
$USERCMDC_START = 3;
$USERCMDC_STOP = 4;
$USERCMDC_SHELL = 99;

$LONG_CONNECT = 9999;

$up = 0;
if(!empty($_GET['up'])){
$up = $_GET['up'];
}

header("content-Type: text/html; charset=utf8");

if( $up == 0 )
{
	echo "<a href=\"test.php?up=1\">开始</a>";
	echo "</body></html>";
	exit(0);
}
set_time_limit(0);
$step = 0;

$str = "&nbsp&nbsp";
$total = 0;
$contotle = 0;

$sock = fsockopen ("$remote_ip:$remote_port" );
if( !$sock )
{
	$str .= "Error";
}
else
{
	$file = "unixODBC-2.3.2.tar";
	// 不是数字16，是字母l和数字6
	$buffer = pack( "l6", $USERCMDC_UPLOAD_BEGIN, 0, 0, 0, $LONG_CONNECT, strlen($file) );
	fputs( $sock, $buffer );
	fputs( $sock, $file );
	$result = fread( $sock, 24 );
	$data = unpack( "l6", $result );

	// cmd, value1~4, len, str
//    $str .= $data[1]."<br>"."&nbsp";
//    $str .= $data[2]."<br>"."&nbsp";
//    $str .= $data[3]."<br>"."&nbsp";
//    $str .= $data[4]."<br>"."&nbsp";
//    $str .= $data[5]."<br>"."&nbsp";
//    $str .= $data[6]."<br>"."&nbsp";

	$size = filesize( "/tmp/unixODBC-2.3.2.tar" );
	$myfile = fopen("/tmp/unixODBC-2.3.2.tar", "r") or die("Unable to open file!");
	$totlelen = 0;
	$last_step = 0;
	while(!feof($myfile))
	{
		$filedata = fread( $myfile, 2000 );
		$datalen = strlen($filedata);

		$buffer = pack( "l6", 2, 1, 0, 0, $LONG_CONNECT, $datalen );
		fputs( $sock, $buffer );
		fputs( $sock, $filedata );
		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );

		$totlelen += $datalen;

		$step = floor($totlelen*200/$size);
		if( $step > $last_step )
		{
			echo '<script> document.getElementById("jindu").style.width = "'.$step.'px"; </script>';
//			ob_flush();
			flush();
			$last_step = $step;
		}

//	    $str .= $data[1]."<br>"."&nbsp";
//	    $str .= $data[2]."<br>"."&nbsp";
//	    $str .= $data[3]."<br>"."&nbsp";
//	    $str .= $data[4]."<br>"."&nbsp";
//	    $str .= $data[5]."<br>"."&nbsp";
//	    $str .= $data[6]."<br>"."&nbsp";
	}
	fclose($myfile);

	$buffer = pack( "l6", 2, 0, 0, 0, 0, 0 );
	fputs( $sock, $buffer );
	$result = fread( $sock, 24 );
	$data = unpack( "l6", $result );
//	$str .= $data[1]."<br>"."&nbsp";
//	$str .= $data[2]."<br>"."&nbsp";
//	$str .= $data[3]."<br>"."&nbsp";
//	$str .= $data[4]."<br>"."&nbsp";
//	$str .= $data[5]."<br>"."&nbsp";
//	$str .= $data[6]."<br>"."&nbsp";
	if( $data[6] > 0 )
	{
		$md5string = fread( $sock, $data[6] );
	}

}
if( $sock )
{
	fclose ($sock);
}

echo $str;
echo $md5string;
echo "|";
$localmd5 = md5_file("/tmp/unixODBC-2.3.2.tar");
if( $md5string == $localmd5 )
	echo "上传完毕";
else 
	echo "上传失败(md5值不匹配)";
?>
</body>
</html>
