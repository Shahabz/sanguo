<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<style type="text/css"><!--
		body {font-family: Arial,Helvetica,sans-serif; font-size: 12px;}
		h1 {font-size: 18px; text-decoration: bold;}
		h2 {font-size: 16px; text-decoration: bold;}
		h3 {font-size: 14px; text-decoration: bold;}
		th {font-size: 12px; border-style: solid; border-width: 1px; text-decoration: bold; padding: 1px;}
		td {font-size: 12px; border-style: solid; border-width: 1px; padding: 1px;}
		code {font-size: 12px; white-space: pre;}
		.TableObject {border-collapse: collapse; border-color: #000000; font-family: 宋体}
		.TableData {border-collapse: collapse; border-color: #000000; font-family: 宋体}
		.TableHeader {border-color: #000000; text-decoration: bold; background-color: #e0e0e0;}
		.ObjectHeader {padding-left: 5px; text-align: left; border-color: #000000; text-decoration: bold;}
		.ObjectOfPrimaryKey {text-align: left; border-color: #aaaaaa; text-decoration: bold; background-color: #e0e0e0;}
		.ObjectOfUniqueKey {text-align: left; border-color: #aaaaaa; background-color: #e0e0e0;}
		.Object {text-align: left; border-color: #aaaaaa;}
		.Record_1 {}
		.Record_2 {background-color: #f0f0f0;}
		.DataHeader {padding-left: 5px; text-align: left; text-decoration: bold; border-color: #000000; background-color: #e0e0e0;}
		.DataOfPrimaryKey {text-align: left; text-decoration: bold; border-color: #aaaaaa; background-color: #e0e0e0;}
		.DataOfUniqueKey {border-color: #aaaaaa;}
		.Data {border-color: #aaaaaa;}
		.DataRightAlign {text-align: right;}
		.DataNull {color: #999999; border-color: #aaaaaa;}
		.btn{position: relative;overflow: hidden;margin-right: 1px;display:inline-block;*display:inline;padding:1px 16px 1px;line-height:13px;*line-height:16px;color:#fff;text-align:center;vertical-align:middle;cursor:pointer;background-color:#808080;border:1px solid #cccccc;border-color:#e6e6e6 #e6e6e6 #bfbfbf;border-bottom-color:#b3b3b3;-webkit-border-radius:1px;-moz-border-radius:1px;border-radius:1px;}
		.btn input {position: absolute;top: 0; right: 0;margin: 0;border: solid transparent;opacity: 0;filter:alpha(opacity=0); cursor: pointer;}
		.progress { position:relative; margin-left:1px; margin-top:1px; width:200px;padding: 1px; border-radius:1px; display:none}
		.bar {background-color:#FFCC66; display:block; width:0%; height:16px; border-radius: 1px; }
		.percent { position:absolute; height:10px; display:inline-block; top:1px; left:2%; color:#FF6633 }
		.files{height:12px; line-height:12px; margin:1px 0}
		.RedText {color:#ff0000 ;text-shadow: 2px 2px 5px #999;}
		.GreenText {color:#008800 ;text-shadow: 2px 2px 5px #999;}
		.BlueText {color:#0000ee ;text-shadow: 2px 2px 5px #999;}
		.PurpleText {color:#ee00ee ;text-shadow: 2px 2px 5px #999;}
	--></style>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<body>
<?php
if (!get_magic_quotes_gpc())
{
	$host=addslashes(@$_GET['h']);
}
else
{
	$host=@$_GET['h'];
}

?>

<form id='setupform' name='adsetupformdform' method='post' action='setup_action.php'>
<table border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'>
	<th class='DataHeader'>服务器地址</th>
	<th class='DataHeader'>用户名</th>
	<th class='DataHeader'>密码</th>
	<th class='DataHeader'></th></tr>
	<tr class='TableHeader'>
	<td class='Data DataRightAlign'><input name='host' size='16' type='text' value='<?php echo $host; ?>'>:<input name='port' size='16' type='text' value='22'></td>
	<td class='Data DataRightAlign'>root</td>
	<td class='Data DataRightAlign'><input name='pass' size='20' type='password' value=''></td>
	<td class='Data DataRightAlign'><input type="submit" id="submit" name="submit" value="安装agent" /></td>
	</tr>
</table>
</form>

</body>

</html>
