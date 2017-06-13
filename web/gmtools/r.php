<?php 
require_once 'db.inc.php';

/*session方式*/
session_start();
header("content-Type: text/html; charset=utf-8");
date_default_timezone_set("Etc/GMT");
if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

echo "<a href='r_name.php' type=_blank>举报名称</a>	";
echo "<a href='r_chat.php' type=_blank>举报聊天</a>	";
echo "<a href='r_head.php' type=_blank>举报头像</a>	";
echo "<a href='http://kor.res.gohogame.com/head/review.php' type=_blank>头像审核</a>	";
?>