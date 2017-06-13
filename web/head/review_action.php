<?php
require_once 'FileUtil.php';
require_once 'db.inc.php';
error_reporting(E_ALL);

$db = new dbstuff;
$pconnect = 0;
$db->connect( $dbhost, $dbuser, $dbpw, $dbname, 0 );

$sql="SELECT serverid,ip,port FROM server_list";
$query = $db->query($sql);
while($row = $db->fetch_row($query))
{
	$id = $row[0];
	$sysid[$id]=$id;
	$sysip[$id]=$row[1];
	$sysport[$id]=$row[2];
}

if ( isset($_POST["checkitem"]) )
{
	$checkitem = $_POST["checkitem"];       //将所选的值组成的数组放入$array数组
	$checkfile = $_POST["checkfile"];
	$num = count($checkitem);           //计算一共有多少项被选择
	
	for ( $i = 0; $i < $num; $i++ )
	{
		if(isset($checkitem[$i]) && isset($checkfile[$i]))
		{
			$ischeck=$checkitem[$i];
			$filename=$checkfile[$i];
			
			$file = explode(".",$filename);
			$file = $file[0];
			$arr = explode("_",$file);
			$serverid = $arr[0];
			$cityid = $arr[1];
			
			$time = time();
			$fdate = floor( $time/86400 );
			$outfilename = $file."_".$time.".jpg";
			$outdir = "./verify/".$serverid."/".$fdate;
			
			// 成功就用时间戳当headid
			$arg1 = 0;
			if ( $ischeck == 1 )
			{
				$arg1 = $time;	
			}
			
			// 通知到该服务器
			$server = $sysip[$serverid].":".$sysport[$serverid];
			$sock = fsockopen ( $server );
			if( !$sock )
			{
				echo "$server not connect!";
			}
			else
			{
				fputs( $sock, "64|$arg1|0|0|$cityid|\0" );
				$recvbuf = "";
			  while(!feof($sock)) 
			  {     
	        $data = fread($sock,81920);
	        $recvjson .= $data;        
	      }
	      
				$recvarray = explode('|',$recvjson);
				$result = 100 - $recvarray[0];
				if ( $result != 0 )
				{
						echo "指令执行完毕："."服务器ID：".$serverid."(".$sysip[$serverid].":".$sysport[$id].")"." 城池ID：".$cityid." 是否通过审核：".$ischeck."<br>";
						// 移动到已经审核通过的目录
						FileUtil::createDir($outdir);
						FileUtil::moveFile("./upload/".$filename, $outdir."/".$outfilename);
				}
				else
				{
						echo "指令执行失败："."服务器ID：".$serverid."(".$sysip[$serverid].":".$sysport[$id].")"." 城池ID：".$cityid." 是否通过审核：".$ischeck."<br>";
				}
					
			}
			fclose ($sock);
		}
	}

	exit("<script language='javascript'>window.location.href='review.php';</script>");
}
