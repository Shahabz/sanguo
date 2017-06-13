<?php 
require_once 'db.inc.php';
error_reporting(0);
/*session方式*/
session_start();
//header("content-Type: text/html; charset=gbk");

if(!session_is_registered("adminid") || $_SESSION["adminid"] <= 0 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}
if( $_SESSION["adminid"] < 100 )
{
	exit("<script language='javascript'>window.location.href='login_ad.php';</script>");
}

$g_filename = array(0=>'undeadServer.tar.gz',1=>'undeadSql.tar.gz',2=>'f_game_patch.sql');
$g_uploadpath = $http_path."/undead_upload";
$uploadpath=$http_path."/undeadres";

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
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
<!-- <meta http-equiv="refresh" content="30"; charset=gbk"> -->
<script language="javascript">
//var xmlHttp;
//var txtHintid;
var setInnerHTML = function (el, htmlCode)
{
	var ua = navigator.userAgent.toLowerCase();
	if (ua.indexOf('msie') >= 0 && ua.indexOf('opera') < 0)
	{
		htmlCode = '<div style="display:none">for IE</div>' + htmlCode;
		htmlCode = htmlCode.replace(/<script([^>]*)>/gi,'<script$1 defer="true">');
		el.innerHTML = htmlCode;
		el.removeChild(el.firstChild);
	}
	else
	{
		var el_next = el.nextSibling;
		var el_parent = el.parentNode;
		el_parent.removeChild(el);
		el.innerHTML = htmlCode;
		if (el_next)
		{
			el_parent.insertBefore(el, el_next);
		}
		else
		{
			el_parent.appendChild(el);
		}
	}
}

var global_html_pool = [];
var global_script_pool = [];
var global_script_src_pool = [];
var global_lock_pool = [];
var innerhtml_lock = null;
var document_buffer = "";

function set_innerHTML(obj_id, html, time) {
    if (innerhtml_lock == null) {
        innerhtml_lock = obj_id;
    }
    else if (typeof(time) == "undefined") {
        global_lock_pool[obj_id + "_html"] = html;
        window.setTimeout("set_innerHTML('" + obj_id + "', global_lock_pool['" + obj_id + "_html']);", 10);
        return;
    }
    else if (innerhtml_lock != obj_id) {
        global_lock_pool[obj_id + "_html"] = html;
        window.setTimeout("set_innerHTML('" + obj_id + "', global_lock_pool['" + obj_id + "_html'], " + time + ");", 10);
        return;
    }

    function get_script_id() {
        return "script_" + (new Date()).getTime().toString(36)
          + Math.floor(Math.random() * 100000000).toString(36);
    }

    document_buffer = "";

    document.write = function (str) {
        document_buffer += str;
    }
    document.writeln = function (str) {
        document_buffer += str + "\n";
    }

    global_html_pool = [];

    var scripts = [];
    html = html.split(/<\/script>/i);
    for (var i = 0; i < html.length; i++) {
        global_html_pool[i] = html[i].replace(/<script[\s\S]*$/ig, "");
        scripts[i] = {text: '', src: '' };
        scripts[i].text = html[i].substr(global_html_pool[i].length);
        scripts[i].src = scripts[i].text.substr(0, scripts[i].text.indexOf('>') + 1);
        scripts[i].src = scripts[i].src.match(/src\s*=\s*(\"([^\"]*)\"|\'([^\']*)\'|([^\s]*)[\s>])/i);
        if (scripts[i].src) {
            if (scripts[i].src[2]) {
                scripts[i].src = scripts[i].src[2];
            }
            else if (scripts[i].src[3]) {
                scripts[i].src = scripts[i].src[3];
            }
            else if (scripts[i].src[4]) {
                scripts[i].src = scripts[i].src[4];
            }
            else {
                scripts[i].src = "";
            }
            scripts[i].text = "";
        }
        else {
            scripts[i].src = "";
            scripts[i].text = scripts[i].text.substr(scripts[i].text.indexOf('>') + 1);
            scripts[i].text = scripts[i].text.replace(/^\s*<\!--\s*/g, "");
        }
    }

    var s;
    if (typeof(time) == "undefined") {
        s = 0;
    }
    else {
        s = time;
    }

    var script, add_script, remove_script;

    for (var i = 0; i < scripts.length; i++) {
        var add_html = "document_buffer += global_html_pool[" + i + "];\n";
        add_html += "document.getElementById('" + obj_id + "').innerHTML = document_buffer;\n";
        script = document.createElement("script");
        if (scripts[i].src) {
            script.src = scripts[i].src;
            if (typeof(global_script_src_pool[script.src]) == "undefined") {
                global_script_src_pool[script.src] = true;
                s += 2000;
            }
            else {
                s += 10;
            }
        }
        else {
            script.text = scripts[i].text;
            s += 10;
        }
        script.defer = true;
        script.type =  "text/javascript";
        script.id = get_script_id();
        global_script_pool[script.id] = script;
        add_script = add_html;
        add_script += "document.getElementsByTagName('head').item(0)";
        add_script += ".appendChild(global_script_pool['" + script.id + "']);\n";
        window.setTimeout(add_script, s);
        remove_script = "document.getElementsByTagName('head').item(0)";
        remove_script += ".removeChild(document.getElementById('" + script.id + "'));\n";
        remove_script += "delete global_script_pool['" + script.id + "'];\n";
        window.setTimeout(remove_script, s + 10000);
    }

    var end_script = "if (document_buffer.match(/<\\/script>/i)) {\n";
    end_script += "set_innerHTML('" + obj_id + "', document_buffer, " + s + ");\n";
    end_script += "}\n";
    end_script += "else {\n";
    end_script += "document.getElementById('" + obj_id + "').innerHTML = document_buffer;\n";
    end_script += "innerhtml_lock = null;\n";
    end_script += "}";
    window.setTimeout(end_script, s);
}

function ajax_output( id, strvalue )
{
	var txtHintid = id;
	document.getElementById( txtHintid ).innerHTML=strvalue;
}

function ajax_checkstat( id, dirstr, hoststr, sysid, opvalue, sysname, v1, v2 )
{
	var txtHintid = id;
	document.getElementById( txtHintid ).innerHTML="<img src='loading.gif' />";
	
	var ajax=GetXmlHttpObject();
	if( ajax == null )
	{
		alert ("Browser does not support HTTP Request");
		return;
	}
	
	var url="";
	if( opvalue < 10 )
	{
		url="y.php?c="+dirstr+"|"+opvalue+"|"+sysname+"&h="+hoststr+"&id="+sysid+"&v1="+v1+"&v2="+v2;
	}
	else
	{
		url="http://47.90.64.233/y.php?c="+dirstr+"|"+opvalue+"|"+sysname+"&h="+hoststr+"&id="+sysid+"&v1="+v1+"&v2="+v2;
	}
	ajax.open("GET", url);
	ajax.onreadystatechange=function () { stateChanged(ajax, txtHintid); };
	ajax.send(null);
}

function stateChanged( xmlHttp, txtHintid )
{
//	if ( xmlHttp.readyState == 4)
	{
//		if ( xmlHttp.status == 200 )
//		setInnerHTML(document.getElementById(txtHintid),xmlHttp.responseText);
//			document.getElementById(txtHintid).innerHTML=xmlHttp.responseText;
	set_innerHTML(txtHintid,xmlHttp.responseText);
	}
}
function GetXmlHttpObject()
{
	var xmlHttp=null;
	try
	{
		// Firefox, Opera 8.0+, Safari
		xmlHttp=new XMLHttpRequest();
	}
	catch (e)
	{
		// Internet Explorer
		try
		{
			xmlHttp=new ActiveXObject("Msxml2.XMLHTTP");
		}
		catch (e)
		{
			xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
		}
	}
	return xmlHttp;
}

function sel(a)
{
	o = document.getElementsByName(a);
	for( i = 0; i < o.length; i++ )
		o[i].checked=event.srcElement.checked;
}


function submit_data()
{
	var o = document.getElementsByName('favor[]');
	var opobj=document.getElementById('op');
	
	var string="";

	var verobj_ios = document.getElementById('ver_ios');
	var verobj_android = document.getElementById('ver_android');

	for( i = 0; i < o.length; i++ )
	{
		if( o[i].checked )
		{
			var x = document.getElementsByName('ip_arr[]');
			var y = document.getElementsByName('path_arr[]');
			var z = document.getElementsByName('id_arr[]');
			var m = document.getElementsByName('sysname_arr[]');
			
			sysip = x[i].value;
			syspath = y[i].value;
			sysid = z[i].value;
			sysname = m[i].value;

			ajax_checkstat( "hit"+i, syspath, sysip, sysid, opobj.value, sysname, verobj_ios.value, verobj_android.value );

		}
	}
}

function opselect(obj)
{
	if( obj == null )
		return;
//	document.getElementById("add_name1").innerHTML = "";
//	document.getElementById("add_input1").innerHTML = "";
}

</script>
</head>

<body>
<script type="text/javascript" src="upload/jquery.min.js"></script>
<script type="text/javascript" src="upload/jquery.form.js"></script>



<form id='addform' name='addform' method='post' action='serv_action.php'>
<table border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'>
	<th class='DataHeader'>内部编号</th>
	<th class='DataHeader'>服务器名称</th>
	<th class='DataHeader'>IP地址</th>
	<th class='DataHeader'>所在目录</th>
	<th class='DataHeader'>平台</th>
	<th class='DataHeader'></th></tr>
	<tr class='TableHeader'>
	<td class='Data DataRightAlign'><input name='id' size='6' type='text'></td>
	<td class='Data DataRightAlign'><input name='name' size='12' type='text'></td>
	<td class='Data DataRightAlign'><input name='ip' size='12' type='text' value='0.0.0.0'></td>
	<td class='Data DataRightAlign'><input name='servpath' size='48' type='text' value='/data/www/xxx'></td>
	<td class='Data DataRightAlign'><input name='sysname' size='12' type='text' value='android'></td>
	<td class='Data DataRightAlign'><input type="submit" id="submit" name="submit" value="添加新服务器" /></td>
	</tr>
</table>
</form>
<br>
<?php
echo "<form id='form1' name='form1' method='post' action='serv.php'>";

$verfile = $uploadpath."/android/ver_android";
$myfile = fopen( $verfile, "r" ) or die("Unable to open file!");
$android_ver = rtrim( fread($myfile,filesize($verfile)) );
fclose($myfile);

$verfile = $uploadpath."/ios/ver_ios";
$myfile = fopen( $verfile, "r" ) or die("Unable to open file!");
$ios_ver = rtrim( fread($myfile,filesize($verfile)) );
fclose($myfile);

echo"<input id='ver_ios' type='hidden' value='$ios_ver' >";
echo"<input id='ver_android' type='hidden' value='$android_ver' >";

?>
<table border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'>
	<th class='DataHeader'></th>
	<th class='DataHeader'>最近版本</th>
	<th class='DataHeader'>补丁压缩包</th>
	<th class='DataHeader'>版本文件</th>
	</tr>
	<tr class='TableHeader'>
	<td class='Data DataRightAlign'>android</td>
	<td class='Data DataRightAlign'><?php echo $android_ver; ?></td>
	<td class='Data DataRightAlign'><?php echo $android_ver.".tar.gz"; ?></td>
	<td class='Data DataRightAlign'>ver_android</td>
	</tr>

	<tr class='TableHeader'>
	<td class='Data DataRightAlign'>ios</td>
	<td class='Data DataRightAlign'><?php echo $ios_ver; ?></td>
	<td class='Data DataRightAlign'><?php echo $ios_ver.".tar.gz"; ?></td>
	<td class='Data DataRightAlign'>ver_ios</td>
	</tr>
</table>
<br>
<?php


$db = new dbstuff;

$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="SELECT * FROM patch_list order by id";
$query = $db->query($sql);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>选项</th><th class='DataHeader'>服务器ID</th><th class='DataHeader'>服务器名称</th><th class='DataHeader'>地址</th><th class='DataHeader'>路径</th><th class='DataHeader'>平台</th><th class='DataHeader'>状态</th><th class='DataHeader'>进度</th></tr>";
$recno = 2;

$ip_arr = array();
$id_arr = array();
$path_arr = array();
$index = 0;
while($row = $db->fetch_row($query))
{

	$sysid=$row[0];
	$hostname=$row[1];
	$sysip=$row[2];
	$syspath=$row[3];
	$sysname=$row[4];

	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;

	echo"<tr class='Record_$recno'>";
	echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysid' ></td>";
	echo"<td class='Data DataRightAlign'>$sysid</td>";
	echo"<td class='Data DataRightAlign'>$hostname</td>";
	echo"<td class='Data DataRightAlign'>$sysip</td>";
	echo"<td class='Data DataRightAlign'>$syspath</td>";
	echo"<td class='Data DataRightAlign'>$sysname</td>";
	
	echo"<input name='ip_arr[]' type='hidden' value='$sysip' >";
	echo"<input name='path_arr[]' type='hidden' value='$syspath' >";
	echo"<input name='id_arr[]' type='hidden' value='$sysid' >";
	echo"<input name='sysname_arr[]' type='hidden' value='$sysname' >";

	$ip_arr[] = $sysip;
	$id_arr[] = $sysid;
	$path_arr[] = $syspath;
	
	echo"<td class='Data DataLeftAlign'><span id='hit$index'></span></td>";
	echo"<td class='Data DataLeftAlign'><span id='progress$sysid'></span></td>";
	echo"</tr>";
	$index++;

}
echo"<tr class='TableHeader'>";
echo"<td class='Data DataRightAlign'><input type=checkbox onclick=sel('favor[]')></td>";
echo"<td class='Data DataRightAlign'>全选</td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"</table>";
$db->close();
?>
<br>
<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'><th class='DataHeader'> </th><th class='DataHeader'>操作</th><th class='DataHeader'>说明</th></tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">指令选择</th>
		<td class="Data DataRightAlign">
	    <select id="op" name="op" onChange="opselect(this)">
	      <option value="3">A.上传最新补丁到香港服务器</option>
	      <option value="11">B.从香港服务器上传补丁</option>
	      <option value="12">C.从香港服务器启动最新版本</option>
	      <option value="1">D.[直接]上传最新补丁</option>
	      <option value="2">E.[直接]启动最新版本</option>
	    </select>
	    </td>
	    <td><span id="opdesc"></span></td>
	</tr>

	
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"> </th>
		<td class="Data DataRightAlign">
		<input type="button" id="submit" name="submit" value="提交" onclick=submit_data() />
	    </td><td></td>
	</tr>
</table>

</form>
<br>

<script> opselect(document.getElementById('op'));</script>

<br><br><br><br><br><br>
<?php
ob_flush();
flush();
$num = count($ip_arr);

for ($tmpi= 0;$tmpi< $num; $tmpi++)
{
	echo "<script language=\"JavaScript\">";
	echo "ajax_checkstat( 'hit$tmpi', '$path_arr[$tmpi]', '$ip_arr[$tmpi]', '$id_arr[$tmpi]', '0', '', '', '' );";
	echo "</script>";
	ob_flush();
	flush();
}
?>

</body>
</html>
