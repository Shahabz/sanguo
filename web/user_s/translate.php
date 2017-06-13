<?php
require_once '../db.inc.php';
require_once 'HttpClient.php';
error_reporting(0);
header("content-type:text/html; charset=utf-8");

function wlogtranslate( $db, $msg )
{
	$sql="INSERT INTO errorlog (`msg`,`optime`) values('$msg',now())";
	$db->query($sql);
}

// 百度翻译
function baidu( $db, $text, $to, $serverid, $cityid, $timestamp )
{
		// 百度翻译对应目标语言
	$baidu_tolang = array( 
	"ar"=>"ara", "de"=>"de", "en"=>"en", "es"=>"spa", "fr"=>"fra",
	"it"=>"it", "nl"=>"nl", "pl"=>"pl", "pt"=>"pt", "ru"=>"ru", "th"=>"th", "zh"=>"zh" );
	
	$from = "auto";
	$text = rawurldecode($text);
	$translateString = $text;
			
	// 先把游戏内部定义的目标语言转换到百度定义的目标语言
	$tolang = "en";
	if ( in_array( $to, $baidu_tolang ) )
	{ // 匹配到了目的语言
		$tolang = $baidu_tolang[$to];
	}
	
	// 该翻译信息的唯一标示
	$key = md5( $to . $serverid . $cityid . $timestamp );
	
	// 先检查数据库里是否存在，存在就直接拿
	$sql="select str from baidu where baidu.key='$key'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$translateString = $sth[0];
		$db->free_result($query);
	}
	else
	{
		// 数据不存在，去百度拿翻译
		$url = "http://api.fanyi.baidu.com/api/trans/vip/translate";
		
		// 龙翔
		//$translateAppid = "20161115000031885";
		//$translateKey = "6qFbK6QnQPQGN2RciFwN";	
		
		// 史磊
		//$translateAppid = "20161117000032080";
		//$translateKey = "jDZTFHqPoCcDvAB4h9vj";
		
		// 程新生
		//$translateAppid = "20161220000034318";
		//$translateKey = "YqicwAqUw045XcaTovMI";
			
		// 朱峰
		//$translateAppid = "20170114000035844";
		//$translateKey = "lsxfZu1WVRWq_CiguZU1";
		
		// 翁美佳
		$translateAppid = "20170114000035845";
		$translateKey = "RRzfwYHTJEd2lffzs5i1";
		
		// 李如峰
		//$translateAppid = "20170114000035846";
		//$translateKey = "lrpFju8YAT8ZE3A_0lDG";
		
		// 李晨阳
		//$translateAppid = "20170114000035847";
		//$translateKey = "2AQB1_mCTk_3o3fH35p2";
		
		// 孟飞
		//$translateAppid = "20170114000035848";
		//$translateKey = "qTVWEtxcCe2KwdNoHPqd";
		
		// 郑洁
		//$translateAppid = "20170114000035849";
		//$translateKey = "Ym8WsEsqpBmpVjxkU0lk";
		
		// 于涛
		//$translateAppid = "20170114000035850";
		//$translateKey = "ATmMCXwMLW37ge7MTfV_";
		
		// 贾智
		//$translateAppid = "20170114000035851";
		//$translateKey = "Zu1pj6fVAwg8FaDZkuHj";
		
		
		$salt = rand( 10000, 2147483647 );
		$sign = md5( $translateAppid . $text . $salt . $translateKey );
		$get = $url."?q=" . rawurlencode($text) . "&from=auto" ."&to=" . $tolang . "&appid=" . $translateAppid . "&salt=" . $salt ."&sign=" . $sign;
		$ret = HttpClient::quickGet( $get );
		$ret = json_decode($ret, true);
		
		// 有错误
		$error = $ret["error_code"];
		if ( $error != null )
		{ // 有错误
			
		}
		else
		{
			$error = "";
			$translateString = $ret["trans_result"][0]["dst"];  // 翻译结果
			$from = $ret["from"];
		}
		
		// 插入到数据库里备份
		$sql = "REPLACE INTO `baidu` SET `key`='$key',`src`='$text',`str`='$translateString',`from`='$from',`to`='$tolang',`serverid`=$serverid,`cityid`=$cityid,`timestamp`=$timestamp,`error`='$error';";
		$query = $db->query($sql);
	}
	
	return "\"str\":\"$translateString\"";
}



// google翻译
function google( $db, $text, $to, $serverid, $cityid, $timestamp )
{
		// google翻译对应目标语言
		$google_tolang = array(
		"af"=>"af",
		"am"=>"am",
		"ar"=>"ar",
		"az"=>"az",
		"be"=>"be",
		"bg"=>"bg",
		"bn"=>"bn",
		"bs"=>"bs",
		"ca"=>"ca",
		"ceb"=>"ceb",
		"co"=>"co",
		"cs"=>"cs",
		"cy"=>"cy",
		"da"=>"da",
		"de"=>"de",
		"el"=>"el",
		"en"=>"en",
		"eo"=>"eo",
		"es"=>"es",
		"et"=>"et",
		"eu"=>"eu",
		"fa"=>"fa",
		"fi"=>"fi",
		"fr"=>"fr",
		"fy"=>"fy",
		"ga"=>"ga",
		"gd"=>"gd",
		"gl"=>"gl",
		"gu"=>"gu",
		"ha"=>"ha",
		"haw"=>"haw",
		"hi"=>"hi",
		"hmn"=>"hmn",
		"hr"=>"hr",
		"ht"=>"ht",
		"hu"=>"hu",
		"hy"=>"hy",
		"id"=>"id",
		"ig"=>"ig",
		"is"=>"is",
		"it"=>"it",
		"iw"=>"iw",
		"ja"=>"ja",
		"jw"=>"jw",
		"ka"=>"ka",
		"kk"=>"kk",
		"km"=>"km",
		"kn"=>"kn",
		"ko"=>"ko",
		"ku"=>"ku",
		"ky"=>"ky",
		"la"=>"la",
		"lb"=>"lb",
		"lo"=>"lo",
		"lt"=>"lt",
		"lv"=>"lv",
		"mg"=>"mg",
		"mi"=>"mi",
		"mk"=>"mk",
		"ml"=>"ml",
		"mn"=>"mn",
		"mr"=>"mr",
		"ms"=>"ms",
		"mt"=>"mt",
		"my"=>"my",
		"ne"=>"ne",
		"nl"=>"nl",
		"no"=>"no",
		"ny"=>"ny",
		"pa"=>"pa",
		"pl"=>"pl",
		"ps"=>"ps",
		"pt"=>"pt",
		"ro"=>"ro",
		"ru"=>"ru",
		"sd"=>"sd",
		"si"=>"si",
		"sk"=>"sk",
		"sl"=>"sl",
		"sm"=>"sm",
		"sn"=>"sn",
		"so"=>"so",
		"sq"=>"sq",
		"sr"=>"sr",
		"st"=>"st",
		"su"=>"su",
		"sv"=>"sv",
		"sw"=>"sw",
		"ta"=>"ta",
		"te"=>"te",
		"tg"=>"tg",
		"th"=>"th",
		"tl"=>"tl",
		"tr"=>"tr",
		"uk"=>"uk",
		"ur"=>"ur",
		"uz"=>"uz",
		"vi"=>"vi",
		"xh"=>"xh",
		"yi"=>"yi",
		"yo"=>"yo",
		"zh"=>"zh",
		"zh-TW"=>"zh-TW",
		"zu"=>"zu" );
	
	
	$from = "auto";
	$text = rawurldecode($text);
	$translateString = $text;
			
	// 先把游戏内部定义的目标语言转换到google定义的目标语言
	$tolang = "en";
	if ( in_array( $to, $google_tolang ) )
	{ // 匹配到了目的语言
		$tolang = $google_tolang[$to];
	}
	
	// 该翻译信息的唯一标示
	$key = md5( $to . $serverid . $cityid . $timestamp );
	
	// 先检查数据库里是否存在，存在就直接拿
	$sql="select str from google where google.key='$key'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$translateString = $sth[0];
		$db->free_result($query);
	}
	else
	{
		// 数据不存在，去google拿翻译
		$url = "https://translation.googleapis.com/language/translate/v2?";
		$key = "AIzaSyCIp7tgORQbDN0J2ebuMPigR_9C6loL3C8";
		$get = $url."key=".$key."&q=" . rawurlencode($text) . "&target=" . $tolang;
		$ret = HttpClient::quickGet( $get );
		$ret = json_decode($ret, true);
		
		// 有错误
		$error = $ret["error"];
		$errorcode = "";
		if ( $error != null )
		{ // 有错误
			$errorcode = $ret["error"]["code"];
		}
		else
		{
			$error = "";
			$translateString = $ret["data"]["translations"][0]["translatedText"];  // 翻译结果
			$from = $ret["data"]["translations"][0]["detectedSourceLanguage"];
		}
		
		// 插入到数据库里备份
		$sql = "REPLACE INTO `google` SET `key`='$key',`src`='$text',`str`='$translateString',`from`='$from',`to`='$tolang',`serverid`=$serverid,`cityid`=$cityid,`timestamp`=$timestamp,`error`='$errorcode';";
		$query = $db->query($sql);
	}
	
	return "\"str\":\"$translateString\"";
}

// 微软bing翻译
function getToken($azure_key)
{
    $url = 'https://api.cognitive.microsoft.com/sts/v1.0/issueToken';
    $ch = curl_init();
    $data_string = json_encode('{body}');
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
            'Content-Type: application/json',
            'Content-Length: ' . strlen($data_string),
            'Ocp-Apim-Subscription-Key: ' . $azure_key
        )
    );
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HEADER, false);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    $strResponse = curl_exec($ch);
    curl_close($ch);
    return $strResponse;
}
function curlRequest($url, $authHeader)
{
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array($authHeader, "Content-Type: text/xml"));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, False);
    $curlResponse = curl_exec($ch);
    curl_close($ch);
    return $curlResponse;
}
function bing( $db, $text, $to, $serverid, $cityid, $timestamp )
{
	$bing_tolang = array(
				"af"=>"af",	// 南非荷兰语
				"ar"=>"ar",	// 阿拉伯语
				"bn"=>"bn",	// 孟加拉语
				"bs"=>"bs-Latn",	// 波斯尼亚语
				"bg"=>"bg",	// 保加利亚语
				"ca"=>"ca",	// 加泰隆语
				"cn"=>"zh-CHS",	// 简体中文
				"zh-cn"=>"zh-CHS",	// 简体中文
				"zh-CN"=>"zh-CHS",	// 简体中文
				"zh"=>"zh-CHS",	// 简体中文
				"zh-TW"=>"zh-CHT",	// 繁体中文
				"zh-TW"=>"yue",	// 粤语(繁体)
				"hr"=>"hr",	// 克罗地亚语
				"cs"=>"cs",	// 捷克语
				"da"=>"da",	// 丹麦语
				"nl"=>"nl",	// 荷兰语
				"en"=>"en",	// 英语
				"et"=>"et",	// 爱沙尼亚语
				"fj"=>"fj",	// 斐济
				"fil"=>"fil",	// 菲律宾语
				"fi"=>"fi",	// 芬兰语
				"fr"=>"fr",	// 法语
				"de"=>"de",	// 德语
				"el"=>"el",	// 希腊语
				"ht"=>"ht",	// 海地克里奥尔语
				"he"=>"he",	// 希伯来语
				"hi"=>"hi",	// 印地语
				"mww"=>"mww",	// 白苗文
				"hu"=>"hu",	// 匈牙利语
				"id"=>"id",	// 印度尼西亚语
				"it"=>"it",	// 意大利语
				"ja"=>"ja",	// 日语
				"sw"=>"sw",	// 斯瓦希里语
				"tlh"=>"tlh",	// 克林贡语
				"tlh-Qaak"=>"tlh-Qaak",	// 克林贡语 (pIqaD)
				"ko"=>"ko",	// 朝鲜语
				"kr"=>"ko",	// 韩语
				"lv"=>"lv",	// 拉脱维亚语
				"lt"=>"lt",	// 立陶宛语
				"mg"=>"mg",	// 马尔加什语
				"ms"=>"ms",	// 马来语
				"mt"=>"mt",	// 马耳他语
				"yua"=>"yua",	// 尤卡坦玛雅语
				"no"=>"no",	// 挪威语
				"otq"=>"otq",	// 克雷塔罗奥托米语
				"fa"=>"fa",	// 波斯语
				"pl"=>"pl",	// 波兰语
				"pt"=>"pt",	// 葡萄牙语
				"ro"=>"ro",	// 罗马尼亚语
				"ru"=>"ru",	// 俄语
				"sm"=>"sm",	// 萨摩亚语
				"sr-Cyrl"=>"sr-Cyrl",	// 塞尔维亚语 (西里尔文)
				"sr-Latn"=>"sr-Latn",	// 塞尔维亚语 (拉丁文)
				"sk"=>"sk",	// 斯洛伐克语
				"sl"=>"sl",	// 斯洛文尼亚语
				"es"=>"es",	// 西班牙语
				"sv"=>"sv",	// 瑞典语
				"ty"=>"ty",	// 塔希提语
				"th"=>"th",	// 泰语
				"to"=>"to",	// 汤加语
				"tr"=>"tr",	// 土耳其语
				"uk"=>"uk",	// 乌克兰语
				"ur"=>"ur",	// 乌尔都语
				"vi"=>"vi",	// 越南语
				"cy"=>"cy"	// 威尔士语 
				);
	
	$text = rawurldecode($text);
	$translateString = $text;
			
	// 先把游戏内部定义的目标语言转换到bing定义的目标语言
	$tolang = "en";
	if ( $to == "zh" )
	{
		$tolang = "zh-CHS";
	}
	else if ( $to == "zh-TW" )
	{
		$tolang = "zh-CHT";
	}
	else
	{
			if ( in_array( $to, $bing_tolang ) )
			{ // 匹配到了目的语言
				$tolang = $bing_tolang[$to];
			}
	}
	
	// 该翻译信息的唯一标示
	$key = md5( $to . $serverid . $cityid . $timestamp );
	
	// 先检查数据库里是否存在，存在就直接拿
	$sql="select str from bing where bing.key='$key'";
	$query = $db->query($sql);
	if($sth = $db->fetch_row($query))
	{
		$translateString = $sth[0];
		$db->free_result($query);
	}
	else
	{
		// 数据不存在，去bing拿翻译
		$azure_key = "2322b9d2e82d4de7b9b6adbda30a325b";
		$accessToken = getToken($azure_key);
		$params = "text=" . urlencode($text) . "&to=" . $tolang . "&appId=Bearer+" . $accessToken;
		$translateUrl = "http://api.microsofttranslator.com/v2/Http.svc/Translate?$params";
		$curlResponse = curlRequest($translateUrl, $authHeader);
		$xmlObj = simplexml_load_string($curlResponse);
		foreach ((array)$xmlObj[0] as $val) {
		    $translateString = $val;
		}
		
		// 插入到数据库里备份
		$sql = "REPLACE INTO `bing` SET `key`='$key',`src`='$text',`str`='$translateString',`from`='auto',`to`='$tolang',`serverid`=$serverid,`cityid`=$cityid,`timestamp`=$timestamp,`error`='-1';";
		$query = $db->query($sql);
	}
	
	return "\"str\":\"$translateString\"";
}

if (!get_magic_quotes_gpc())
{
	$command=addslashes(@$_GET['c']);
	$text=addslashes(@$_GET['text']);
	$to=addslashes(@$_GET['to']);
	$serverid=addslashes(@$_GET['sid']);
	$cityid=addslashes(@$_GET['cid']);
	$timestamp=addslashes(@$_GET['stamp']);
	$ext=addslashes(@$_GET['ext']);
}
else
{
	$command=@$_GET['c'];
	$text=@$_GET['text'];
	$to=@$_GET['to'];
	$serverid=@$_GET['sid'];
	$cityid=@$_GET['cid'];
	$timestamp=@$_GET['stamp'];
	$ext=@$_GET['ext'];
}

$db = new dbstuff;
$pconnect = 0;
$db->connect( $translate_dbhost, $translate_dbuser, $translate_dbpw, $translate_dbname, 0 );
$db->query("SET NAMES UTF8");

$msg = baidu( $db, $text, $to, $serverid, $cityid, $timestamp );
//$msg = google( $db, $text, $to, $serverid, $cityid, $timestamp );
//$msg = bing( $db, $text, $to, $serverid, $cityid, $timestamp );
echo "{ \"cmd\":\"$command\", $msg, \"ext\":\"$ext\" }";
?>
