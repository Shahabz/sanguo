<?php
require_once 'config.inc.php';
//http://192.168.197.129/u_cli/api.php?c=serverinfo&m=getlist&sign=ede4ab62b9f02d3e668beba3c12c817b
$c = isset($_GET['c'])?$_GET['c']:NULL;
$m = isset($_GET['m'])?$_GET['m']:NULL;
if( $c != NULL && $m != NULL )
{  
		$pramers = getparams();
		if ( $pramers == false )
		{
			echo "{ \"error\":\"invalid signature\" }";
			exit;
		}
		else
		{
			include( ROOT_PATH . "/$c.php" );
			if( class_exists($c) && method_exists($c, $m) )
	    { // 类方法均为静态方法
	    	$return = $c::$m( $pramers );
	    	echo $return;
	  	}
	  	else
	  	{
	  		echo "{ \"error\":\"-1001\" }";
	    	exit;  
	  	}
		}
}
else
{  
    echo "{ \"error\":\"-1002\" }";
    exit;
}

/**
 * 获取GET参数
 *
 */
function getparams()
{
	$query = $_SERVER["QUERY_STRING"];
  $queryParts = explode('&', $query);
  $getArray = array();
  $paramArray = array();
  foreach ($queryParts as $param) 
  {
    $item = explode('=', $param);
    $getArray[$item[0]] = $item[1];
    if ( $item[0] != "sign" )
    {
    	$paramArray[$item[0]] = $item[1];
    }
  }
  
  // 是否校验参数 规则md5( 排除sign的其它数据 )
  $query = http_build_query( $paramArray );
  $sign = $getArray["sign"];
  $params_sign = md5( $query.SECRET_KEY );
  if ( $params_sign != $sign )
  {
  	return false;
  }
  return $paramArray;
}
?>
