<?php
require_once 'db.inc.php';
error_reporting(0);
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); 
header("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT"); 
header("Cache-Control: no-cache, must-revalidate"); 
header("Pramga: no-cache"); 
echo "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />";

$USERCMDC_GETINFO = 1;
$USERCMDC_UPLOAD_BEGIN = 2;
$USERCMDC_UPLOAD_FILE = 3;
$USERCMDC_START = 4;
$USERCMDC_STOP = 5;
$USERCMDC_INSTALL = 6;
$USERCMDC_CFGCREATE = 7;
$USERCMDC_CFGMODIFY = 8;
$USERCMDC_MOVECORE = 9;
$USERCMDC_UPDATE = 10;
$USERCMDC_UPDATE_SCRIPT = 11;
$USERCMDC_STAT = 12;
$USERCMDC_UPDATE_DATA = 13;
$USERCMDC_CFGINFO = 14;
$USERCMDC_UPDATESQL = 15;
$USERCMDC_UPDATELUA = 16;
$USERCMDC_BACKUPGAME = 17;
$USERCMDC_DOWNLOAD_BEGIN = 18;
$USERCMDC_DOWNLOAD_FILE = 19;
$USERCMDC_CRONTAB = 20;
$USERCMDC_CLEAR_GAME = 21;

$USERCMDC_SHELL = 99;

$LONG_CONNECT = 9999;

$uploadfile = "undeadServer.tar.gz";
$uploadsql = "undeadSql.tar.gz";
$uploadpatchsql = "f_game_patch.sql";
$uploadpath = $http_path."/undead_upload";
$downloadpath = $http_path."/undead_upload";
$downloadurl = "/undead_upload";
$scriptdir = "./script/";
$servername = "undeadServer";
$statfile = ".stat";

$remote_port="33900";

function runsql( $sql )
{
	if( $sql == "" )
		return;
	$sql=str_replace('\\','',$sql); 

	$db = new dbstuff;
	global $dbh;
	global $dbu;
	global $dbp;
	global $dbd;
	$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

	$query = $db->query($sql);
	$db->free_result($query);

	$db->close();

	return;
}

function set_server_list( $serverid )
{
	global $user_dbh;
	global $user_dbu;
	global $user_dbp;
	global $user_dbd;

	global $dbh;
	global $dbu;
	global $dbp;
	global $dbd;

	$db = new dbstuff;
	$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );
	$sql="SELECT name,ip,port,dbh,dbu,dbp,dbd FROM server_list where id='$serverid'";
	$query = $db->query($sql);

	$servername = "";
	$ip = "";
	$port = "";
	$dbh_game = "";
	$dbu_game = "";
	$dbp_game = "";
	$dbd_game = "";

	if($row = $db->fetch_row($query))
	{
		$servername=$row[0];
		$ip=$row[1];
		$port=$row[2];
		$dbh_game=$row[3];
		$dbu_game=$row[4];
		$dbp_game=$row[5];
		$dbd_game=$row[6];
	}
	else
	{
		echo "set server_list table error(serverid:$serverid)";
		$db->free_result($query);
		$db->close();
		return;
	}


	$db->free_result($query);
	$db->close();

	$db = new dbstuff;
	$db->connect( $user_dbh, $user_dbu, $user_dbp, $user_dbd, 0 );

	$sql = "replace into server_list(id,serverid,name,ip,port,dbh,dbu,dbp,dbd) values('$serverid','$serverid','$servername','$ip','$port','$dbh_game','$dbu_game','$dbp_game','$dbd_game')";
	$query = $db->query($sql);
	$db->free_result($query);

	$db->close();
	return;
}

function upload_file( $sock, $upfilename, $sysid )
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
		
	$size = filesize( "$uploadpath/$upfilename" );
	$myfile = fopen( "$uploadpath/$upfilename", "r");
	if( !$myfile )
	{
		echo "无法打开$uploadpath/$upfilename(或为web权限不足访问该文件).";
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
//			echo "<span style='color:green;'>.</span>";
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

	$localmd5 = md5_file("$uploadpath/$upfilename");
	if( strcmp( $md5string, $localmd5 ) == 0 )
	{
		$outtext = "<span class=GreenText>".$upfilename."上传完毕</span>";
		echo "<script>set_innerHTML('progress".$sysid."','".$outtext."')</script>";
	}
	else
	{
		$outtext = "<span class=RedText>".$upfilename."md5值不匹配{$localmd5}[$md5string]</span>";
		echo "<script>set_innerHTML('progress".$sysid."','".$outtext."')</script>";
		$return_value = 1;
	}
	ob_flush();
	flush();
	return $return_value;
}

function download_file( $sock, $downloadfile, $sysid )
{
	global $downloadurl;
	global $downloadpath;
	global $USERCMDC_DOWNLOAD_BEGIN;
	global $USERCMDC_DOWNLOAD_FILE;
	global $LONG_CONNECT;
	
	$return_value = 0;
	
	set_time_limit(0);

	$step = 0;
	// 不是数字16，是字母l和数字6
	$buffer = pack( "l6", $USERCMDC_DOWNLOAD_BEGIN, 0, 0, 0, $LONG_CONNECT, strlen($downloadfile) );
	fputs( $sock, $buffer );
	fputs( $sock, $downloadfile );
	$result = fread( $sock, 24 );
	$data = unpack( "l6", $result );

	if( $data[1] < 0 )
	{
		return $return_value;
	}

	if( $data[6] > 0 )
	{
		$md5string = fread( $sock, $data[6] );
	}
		
	$size = $data[2];
	$left_size = $size;
	$myfile = fopen( "$downloadpath/$downloadfile", "w");
	if( !$myfile )
	{
		echo "无法打开$downloadpath/$downloadfile(或为web权限不足访问该文件).";
		return;
	}

	$totlelen = 0;
	$last_step = 0;

	$buffer = pack( "l6", $USERCMDC_DOWNLOAD_FILE, 1, 0, 0, $LONG_CONNECT, 0 );
	fputs( $sock, $buffer );

	while( $left_size > 0 )
	{
		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );

		if( $data[1] != 0 )
		{
			ob_flush();
			flush();
			return $return_value;
		}

		$datalen = $data[6];
		if( $datalen > 0 )
		{
			$left = $datalen;
			while( $left > 0 )
			{
				$filedata = fread( $sock, $left );
				fwrite( $myfile, $filedata );
				$left -= strlen($filedata);
			}
		}

		$totlelen += $datalen;
		$left_size -= $datalen;

		$step = floor($totlelen*150/$size);
		if( $step > $last_step )
		{
			// ■
			$outtext = "$downloadfile";
			$outtext .= "<div style=\"padding:0;background-color:white;border:1px solid navy;width:150px\">";
			$outtext .= "<div style=\"padding:0;background-color:#FFCC66;border:0;width:".$step."px;text-align:center;height:8px\"></div>";
			$outtext .= "</div>";
			echo "<script>set_innerHTML('progress".$sysid."','".$outtext."')</script>";
			ob_flush();
			flush();
			$last_step = $step;
		}

		if( $data[2] == 0 )
		{
			break;
		}

	}
	fclose($myfile);


	$localmd5 = md5_file("$downloadpath/$downloadfile");
	if( strcmp( $md5string, $localmd5 ) == 0 )
	{
		$outtext = "<span class=GreenText>".$downloadfile."<a href=\"$downloadurl/$downloadfile\" target=_blank>点击下载</a></span>";
		echo "<script>set_innerHTML('progress".$sysid."','".$outtext."')</script>";
	}
	else
	{
		$outtext = "<span class=RedText>".$downloadfile."md5值不匹配{$localmd5}[$md5string]</span>";
		echo "<script>set_innerHTML('progress".$sysid."','".$outtext."')</script>";
		$return_value = 1;
	}
	ob_flush();
	flush();
	return $return_value;
}

if (!get_magic_quotes_gpc())
{
	$command=addslashes(@$_GET['c']);
	$host=addslashes(@$_GET['h']);
	$sysid=addslashes(@$_GET['id']);
	$strinput=addslashes(@$_GET['str']);
	$sqlinput=addslashes(@$_GET['sql']);
}
else
{
	$command=@$_GET['c'];
	$host=@$_GET['h'];
	$sysid=@$_GET['id'];
	$strinput=@$_GET['str'];
	$sqlinput=@$_GET['sql'];
}
//$host='10.132.36.1';
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
	// $arr[1] 状态 0:查询 1:上传 2:解压
	// 查询
	if( $arr[1] == 0 )
	{
		$buffer = pack( "l6", $USERCMDC_GETINFO, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($strinput)+1 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$strinput );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}
	// cmd, value1~4, len, str
//    $str .= $data[1]."<br>"."&nbsp";
//    $str .= $data[2]."<br>"."&nbsp";
//    $str .= $data[3]."<br>"."&nbsp";
//    $str .= $data[4]."<br>"."&nbsp";
//    $str .= $data[5]."<br>"."&nbsp";
//    $str .= $data[6]."<br>"."&nbsp";
		$rtn = "";
		if( $data[5] == 0 )
		{
			echo "<script>set_innerHTML('time".$sysid."','<span class=RedText>无</span>')</script>";
		}
		else
		{
			$time = date('Y-m-d H:i:s',$data[5]);
			$spacetime = abs(time()-$data[5]);
			$outtext = "";
			if( $spacetime < 60 )
				$outtext = "<span>$time</span>";
			else if( $spacetime < 300 )
				$outtext = "<span class=BlueText>$time</span>";
			else
				$outtext = "<span class=RedText>$time</span>";
			echo "<script>set_innerHTML('time".$sysid."','".$outtext."')</script>";
		}
		if( $data[1] < 0 )
		{
			$rtn .= "<span class=RedText>未安装</span>";
		}
		else
		{
			if( $data[2] )
				$rtn = "<span class=RedText>savesqlfile.sql</span>";
			if( $data[3] )
				$rtn .= "<span class=PurpleText>[core文件:$data[3]个]</span>";
		}

		if( $data[4] == 0 )
		{
			echo "<script>set_innerHTML('tarfile".$sysid."','<span class=GreenText>分发包就绪</span>')</script>";
		}
		else
		{
			echo "<script>set_innerHTML('tarfile".$sysid."','<span class=RedText>分发包不同</span>')</script>";
		}
		
//		echo "<span style='color:green;' id='jindu".$sysid."'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>";
		echo $rtn;
		ob_flush();
		flush();
		
		$time = 0;
		$runvalue = 1;
		while( $runvalue == 1 )
		{
			$buffer = pack( "l6", $USERCMDC_STAT, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($statfile)+1 );
			fputs( $sock, $buffer );
			fputs( $sock, $arr[0]."|".$statfile );
	
			$result = fread( $sock, 24 );
			$data = unpack( "l6", $result );
			$strdata = "";
			if( $data[6] > 0 )
			{
				$strdata = fread( $sock, $data[6] );
			}
	
			if( $data[1] < 0 )
			{
				echo "<script>set_innerHTML('jindu".$sysid."','<div class=PurpleText>停止状态</div>')</script>";
				$runvalue = 0;
			}
			else
			{
				if( $data[2] > 0 )
				{
					echo "<script>set_innerHTML('jindu".$sysid."','<div class=BlueText>启动中(".$data[2].")</div>')</script>";
					ob_flush();
					flush();
				}
				else if( $data[2] == 0 )
				{
					echo "<script>set_innerHTML('jindu".$sysid."','<div class=GreenText>运行中</div>')</script>";
					$runvalue = 0;
				}
				else if( $data[2] == -1 )
				{
					echo "<script>set_innerHTML('jindu".$sysid."','<div class=RedText>启动失败</div>')</script>";
					$runvalue = 0;
				}
				else if( $data[2] == -11 )
				{
					echo "<script>set_innerHTML('jindu".$sysid."','<div class=RedText>停止失败</div>')</script>";
					$runvalue = 0;
				}
				else
				{
					echo "<script>set_innerHTML('jindu".$sysid."','<div class=BlueText>停止中</div>')</script>";
					ob_flush();
					flush();
				}
				sleep(3);
			}
		}

	}
	else if( $arr[1] == 1 )
	{
		// 1.上传undeadServer.tar.gz
		if( upload_file( $sock, $uploadfile, $sysid ) == 1 )
			exit(0);
		if( upload_file( $sock, $uploadsql, $sysid ) == 1 )
			exit(0);
		if( upload_file( $sock, $uploadpatchsql, $sysid ) == 1 )
			exit(0);
//		upload_file( $sock, $uploadsql, $sysid );
//		upload_file( $sock, $uploadpatchsql, $sysid );
	}
	else if( $arr[1] == 2 )
	{
		// 启动服务器
		$buffer = pack( "l6", $USERCMDC_START, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($servername)+strlen($statfile)+2 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$servername."|".$statfile );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			echo "<span class=RedText>启动失败 $strdata</span>";
		}
		else
		{
			echo "<span style='color:green;' id='jindu".$sysid."'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>";
			ob_flush();
			flush();
			
			$time = 0;
			$runvalue = 1;
			while( $runvalue == 1 )
			{
				$buffer = pack( "l6", $USERCMDC_STAT, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($statfile)+1 );
				fputs( $sock, $buffer );
				fputs( $sock, $arr[0]."|".$statfile );
		
				$result = fread( $sock, 24 );
				$data = unpack( "l6", $result );
				$strdata = "";
				if( $data[6] > 0 )
				{
					$strdata = fread( $sock, $data[6] );
				}
		
				if( $data[1] < 0 )
				{
					echo "<span class=RedText>启动异常 $strdata</span>";
					$runvalue = 0;
				}
				else
				{
					$time++;
					echo "<script>set_innerHTML('jindu".$sysid."','<div class=BlueText>启动中(".$data[2].")</div>')</script>";
//					echo "$time";
					ob_flush();
					flush();
					if( $data[2] == 0 )
					{
						echo "<script>set_innerHTML('jindu".$sysid."','<div class=GreenText>运行中</div>')</script>";
						$runvalue = 0;
					}
					else if( $data[2] == -1 )
					{
						echo "<script>set_innerHTML('jindu".$sysid."','<div class=RedText>启动失败</div>')</script>";
						$runvalue = 0;
					}
					else
						sleep(3);
				}
			}
		}
	}
	else if( $arr[1] == 3 )
	{
		// 停止服务器
		$buffer = pack( "l6", $USERCMDC_STOP, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($servername)+strlen($statfile)+2 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$servername."|".$statfile );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			echo "<span class=RedText>停止失败 $strdata</span>";
		}
		else
		{
			echo "<span style='color:green;' id='jindu".$sysid."'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>";
			ob_flush();
			flush();
			
			$time = 0;
			$runvalue = 1;
			while( $runvalue == 1 )
			{
				$buffer = pack( "l6", $USERCMDC_STAT, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($statfile)+1 );
				fputs( $sock, $buffer );
				fputs( $sock, $arr[0]."|".$statfile );
		
				$result = fread( $sock, 24 );
				$data = unpack( "l6", $result );
				$strdata = "";
				if( $data[6] > 0 )
				{
					$strdata = fread( $sock, $data[6] );
				}
		
				if( $data[1] < 0 )
				{
					echo "<script>set_innerHTML('jindu".$sysid."','<div class=PurpleText>停止状态</div>')</script>";
					$runvalue = 0;
				}
				else
				{
					$time++;
					echo "<script>set_innerHTML('jindu".$sysid."','<div class=BlueText>停止中(".$data[2].")</div>')</script>";
					ob_flush();
					flush();
					if( $data[2] == -11 )
					{
						echo "<script>set_innerHTML('jindu".$sysid."','<div class=RedText>停止失败</div>')</script>";
						$runvalue = 0;
					}
					else
						sleep(3);
				}
			}
		}
	}
	else if( $arr[1] == 4 )
	{
		$rtn = "";
		$buffer = pack( "l6", $USERCMDC_UPDATE, 0, 0, 0, 0, strlen($arr[0])+strlen($uploadfile)+strlen($uploadpatchsql)+2 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$uploadfile."|".$uploadpatchsql );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			$rtn .= "<span class=RedText>更新失败[解压数据包失败] $strdata</span>";
		}
		else
		{
			// 生成配置文件
			$rtn .= "<span class=GreenText>解压完成 $strdata 更新完成</span>";
		}
		echo $rtn;
	}
	else if( $arr[1] == 5 )
	{
		$rtn = "";
		$buffer = pack( "l6", $USERCMDC_UPDATE_SCRIPT, 0, 0, 0, 0, strlen($arr[0])+strlen($uploadfile)+strlen($scriptdir)+2 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$uploadfile."|".$scriptdir );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			$rtn .= "<span class=RedText>更新失败[解压数据包失败] $strdata</span>";
		}
		else
		{
			// 生成配置文件
			$rtn .= "<span class=GreenText>解压完成 $strdata 更新完成</span>";
		}
		echo $rtn;
	}
	else if( $arr[1] == 6 )
	{
		$rtn = "";

		// 先做配置
		$buffer = pack( "l6", $USERCMDC_CFGCREATE, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+1+strlen($strinput) );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$strinput );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}
		if( $data[1] < 0 )
		{
			$rtn .= "<span class=RedText>安装失败[创建配置文件失败] $strdata</span>";
		}
		else
		{
			$rtn .= " <span class=GreenText>配置完成 $strdata</span>";
			// 解包安装
			$buffer = pack( "l6", $USERCMDC_INSTALL, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($uploadfile)+strlen($uploadsql)+2 );
			fputs( $sock, $buffer );
			fputs( $sock, $arr[0]."|".$uploadfile."|".$uploadsql );
	
			$result = fread( $sock, 24 );
			$data = unpack( "l6", $result );
			$strdata = "";
			if( $data[6] > 0 )
			{
				$strdata = fread( $sock, $data[6] );
			}
	
			if( $data[1] < 0 )
			{
				$rtn .= "<span class=RedText>安装失败[解压数据包失败] $strdata</span>";
			}
			else
			{
				// 生成配置文件
				$rtn .= "<span class=GreenText>解压完成 $strdata 安装完成</span>";
				runsql( $sqlinput );
			}

			// 生成自动备份
			$timeset = "5 4 * * *";
			$scriptcode = "#!/bin/bash\nserverpath=".$arr[0]."\nsource \${serverpath}/server.cfg\nGAMEDB_HOST=`echo \$GAMEDB_HOST | sed 's/\\r//g'`\nGAMEDB_USER=`echo \$GAMEDB_USER | sed 's/\\r//g'`\nGAMEDB_PASS=`echo \$GAMEDB_PASS | sed 's/\\r//g'`\nGAMEDB_DATABASE=`echo \$GAMEDB_DATABASE | sed 's/\\r//g'`\nbackuppath=\${serverpath}/backup\nmkdir -p \$backuppath\ncd \$backuppath/\nsqlfile=`date +%Y_%m_%d`-`date +%H_%M_%S`-\${GAMEDB_DATABASE}.sql\ntarfile=\${sqlfile}.tar.gz\nmysqldump -h\$GAMEDB_HOST -u\$GAMEDB_USER -p\$GAMEDB_PASS --default-character-set=utf8 --single-transaction -R \$GAMEDB_DATABASE > \$sqlfile\ntar -vzcf \${tarfile} \${sqlfile}\nrm -rf \${sqlfile}\n";
			$strval = $timeset."|".$scriptcode;
			$buffer = pack( "l6", $USERCMDC_CRONTAB, 1, 0, 0, $LONG_CONNECT, strlen($strval) );
			fputs( $sock, $buffer );
			fputs( $sock, $strval );
	
			$result = fread( $sock, 24 );
			$data = unpack( "l6", $result );
	
			if( $data[1] < 0 )
			{
				$rtn .= "<span class=RedText>crontab失败 $strdata</span>";
			}
			set_server_list( $sysid );
		}

		echo $rtn;

	}
	else if( $arr[1] == 7 )
	{
		// 修改配置文件
		$buffer = pack( "l6", $USERCMDC_CFGMODIFY, 0, 0, 0, 0, strlen($arr[0])+1+strlen($strinput) );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$strinput );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		$rtn = "";
		if( $data[1] < 0 )
		{
			$rtn .= "<span class=RedText>配置文件修改失败 $strdata</span>";
		}
		else
		{
			$rtn .= "<span class=GreenText>配置文件修改完成 $strdata</span>";
			runsql( $sqlinput );
		}
		echo $rtn;
	}
	else if( $arr[1] == 8 )
	{
		// 转存core文件
		$buffer = pack( "l6", $USERCMDC_MOVECORE, 0, 0, 0, 0, strlen($arr[0]) );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0] );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		$rtn = "";
		if( $data[1] < 0 )
		{
			$rtn .= "<span class=RedText>崩溃信息转存失败 $strdata</span>";
		}
		else
		{
			$rtn .= "<span class=GreenText>崩溃信息转存完成 $strdata</span>";
		}
		echo $rtn;
		
	}
	else if( $arr[1] == 9 )
	{
		// 转存core文件
		$buffer = pack( "l6", $USERCMDC_MOVECORE, 1, 0, 0, 0, strlen($arr[0]) );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0] );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		$rtn = "";
		if( $data[1] < 0 )
		{
			$rtn .= "<span class=RedText>崩溃信息转存失败 $strdata</span>";
		}
		else
		{
			$rtn .= "<span class=GreenText>崩溃信息转存完成 $strdata</span>";
		}
		echo $rtn;
		
	}
	else if( $arr[1] == 10 )
	{
		// 更新f_data.sql
		$buffer = pack( "l6", $USERCMDC_UPDATE_DATA, 0, 0, 0, 0, strlen($arr[0])+strlen($uploadsql)+1 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$uploadsql );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		$rtn = "";
		if( $data[1] < 0 )
		{
			$rtn .= "<span class=RedText>更新f_data失败 $strdata</span>";
		}
		else
		{
			$rtn .= "<span class=GreenText>更新f_data完成 $strdata</span>";
		}
		echo $rtn;
		
	}
	else if( $arr[1] == 11 )
	{
		// undeadServer sql
		$buffer = pack( "l6", $USERCMDC_UPDATESQL, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($servername)+1 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$servername );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			echo "<span class=RedText>更新sql失败 $strdata</span>";
		}
		else
		{
			echo "<span class=GreenText>更新sql成功 $strdata</span>";
		}
	}
	else if( $arr[1] == 12 )
	{
		// undeadServer lua
		$buffer = pack( "l6", $USERCMDC_UPDATESQL, 0, 0, 0, $LONG_CONNECT, strlen($arr[0])+strlen($servername)+1 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$servername );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			echo "<span class=RedText>更新lua失败 $strdata</span>";
		}
		else
		{
			echo "<span class=GreenText>更新lua成功 $strdata</span>";
		}
	}
	else if( $arr[1] == 13 )
	{
		$rtn = "";
		$buffer = pack( "l6", $USERCMDC_BACKUPGAME, 0, 0, 0, $LONG_CONNECT, strlen($arr[0]) );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0] );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			echo "<span class=RedText>备份失败 $strdata</span>";
		}
		else
		{
			if( download_file( $sock, "f_game.sql.tar.gz", $sysid ) == 1 )
				exit(0);
		}
	}
	else if( $arr[1] == 14 )
	{
		$buffer = pack( "l6", $USERCMDC_CLEAR_GAME, 1, 0, 0, 0, strlen($arr[0])+strlen($statfile)+1 );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0]."|".$statfile );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			if( $data[2] == 1 )
				echo "<span class=RedText>清除失败 $strdata(服务器可能在运行)</span>";
			else if( $data[2] == 2 )
				echo "<span class=RedText>清除失败 $strdata(超过开服时间)</span>";
		}
		else
		{
			echo "<span class=RedGreen>清除完成 $strdata(备份数据在/tmp目录)</span>";
		}
	}
	else if( $arr[1] == 15 )
	{
		$rtn = "";
		$buffer = pack( "l6", $USERCMDC_BACKUPGAME, 0, 0, 0, $LONG_CONNECT, strlen($arr[0]) );
		fputs( $sock, $buffer );
		fputs( $sock, $arr[0] );

		$result = fread( $sock, 24 );
		$data = unpack( "l6", $result );
		$strdata = "";
		if( $data[6] > 0 )
		{
			$strdata = fread( $sock, $data[6] );
		}

		if( $data[1] < 0 )
		{
			echo "<span class=RedText>备份失败 $strdata</span>";
		}
		else
		{
			echo "<span class=RedText>备份完毕 [$arr[0]/f_game.sql.tar.gz]</span>";
		}
	}
	fclose ($sock);
}


?>
