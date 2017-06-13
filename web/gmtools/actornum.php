<?php
require_once 'db.inc.php';

session_start();
header("content-Type: text/html; charset=utf8");


$str = "&nbsp&nbsp";
$total = 0;
$contotle = 0;
$db = new dbstuff;
$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );
$sql = "SELECT * FROM server_list order by id";
$query = $db->query($sql);
while($row = $db->fetch_row($query))
{
		$sysid=$row[0];
		$sysname=$row[1];
		$sysip=$row[2];
		$sysport=$row[3];
		$sysdbh=$row[4];
		$sysdbu=$row[5];
		$sysdbp=$row[6];
		$sysdbd=$row[7];

		$logdb=$row[11];

        $cmd=0;
        $arg1=0;
        $arg2=0;
        $arg3=0;
        $arg4=0;
        $string=0;
        $erron = 0;
        $errstr = "";

        $connum = 0;

				$sock = fsockopen ("$sysip:$sysport" );
        if( !$sock )
        {
           $str .= "[$sysname ×]"."&nbsp";
        }
        else
        {
                fputs( $sock, "$cmd|$arg1|$arg2|$arg3|$arg4|$string\0" );
                $result = fgets( $sock );
                if ( $result >= 0 )
                {
                          $str .= "[$sysname:$result";
                          $total += $result;
                }
                else
                {
                         $str .= "[$sysname:$result";
                }


							$db = new dbstuff;
							$db->connect( $sysdbh, $sysdbu, $sysdbp, $logdb, 0 );
					
							$sql="select con from gamecount order by id desc limit 1";
							$query = $db->query($sql);
							if($thisrow = $db->fetch_row($query))
							{
							
								$db->close();
			          $connum=$thisrow[0];
			        }
			        $str .= "/$connum]</a>"."&nbsp";
        }
        if( $sock )
        {
           fclose ($sock);
        }

        $contotle += $connum;

}
echo "<font size=2>&nbsp&nbsp总人数:".$total."/".$contotle."&nbsp".$str."</font>";

$db->close();

?>
