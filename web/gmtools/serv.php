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

//putenv( "JAVA_HOME=/usr/lib/jvm/jdk1.8.0_111" );
//putenv( "ANT_HOME=/usr/local/ant" );

//$PATH=getenv('PATH');
//putenv( "PATH=/usr/lib/jvm/jdk1.8.0_111/bin:/usr/local/ant/bin:$PATH" );

//echo "(".getenv('PATH').")";

$op=0;
if( isset($_POST["op"]) )
{
	$op=$_POST["op"];
	if ( isset($_POST["favor"]) )
			$array = $_POST["favor"];       //将所选的值组成的数组放入$array数组
	else
	{
		echo"没有选择服务器";
		exit;
	}
}
else
{
	$op=0;
}

$g_filename = array(0=>'undeadServer.tar.gz',1=>'undeadSql.tar.gz',2=>'f_game_patch.sql');
$g_uploadpath = $http_path."/undead_upload";

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

function ajax_checkstat( id, dirstr, hoststr, sysid, opvalue, strvalue, sql )
{
	var txtHintid = id;
	document.getElementById( txtHintid ).innerHTML="<img src='loading.gif' />";

//	var ajax = new XMLHttpRequest();
	var ajax=GetXmlHttpObject();
	if( ajax == null )
	{
		alert ("Browser does not support HTTP Request");
		return;
	}
//  ajax.upload.addEventListener("progress", progressHandler, false);

	var url="x.php?c="+dirstr+"|"+opvalue+"|&h="+hoststr+"&id="+sysid+"&str="+encodeURI(strvalue)+"&sql="+sql;
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

function checkitem( checkobj, inputid )
{
	var opobj=document.getElementById('op');
	if( opobj.value == 6 )
	{
		alert("首次创建配置文件不可缺少！");
		checkobj.checked = true;
		return;
	}
	o = document.getElementById(inputid);
	o.disabled = !event.srcElement.checked;
	if( checkobj.checked )
		o.style.color="#000000";
	else
		o.style.color="#808080";
}

function disable_all()
{
	obj = document.getElementById('SERVER_PORT');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('DSRV_PORT');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('GLOBALSRV_HOST');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('GLOBALSRV_PORT');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('DATADB_HOST');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('DATADB_USER');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('DATADB_PASS');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('DATADB_DATABASE');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('GAMEDB_HOST');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('GAMEDB_USER');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('GAMEDB_PASS');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('GAMEDB_DATABASE');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('LOGDB_HOST');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('LOGDB_USER');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('LOGDB_PASS');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('LOGDB_DATABASE');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('MAX_CLUBCOUNT');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('MAX_CONNECTION');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('MAX_CITYCOUNT');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('SERVER_CODE');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('SERVER_START_DATE');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('SERVER_START_HOUR');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('SERVER_START_MIN');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('SHOWFLAG');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('NOPRINT');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('USENETLOG');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('WEBLOG_HOST');
	obj.disabled = true;
	obj.style.color="#808080";

	obj = document.getElementById('WEBLOG_PORT');
	obj.disabled = true;
	obj.style.color="#808080";
}

function submit_data()
{
	var o = document.getElementsByName('favor[]');
	var opobj=document.getElementById('op');
	
	var select_num = 0;	
	for( i = 0; i < o.length; i++ )
	{
		if( o[i].checked )
		{
			select_num++;
		}
	}

	if( opobj.value == 10 && select_num > 1 )
	{
		alert( "为了安全起见，对f_data的更新只能通过一个服务器更新"+select_num );
		return;
	}

	var string="";
	var sql="update server_list set ";
	var hassql = 0;
	if( opobj.value == 6 || opobj.value == 7 )
	{
		if( opobj.value == 6 )
			string += "SERVER_HOST=0.0.0.0\n";
	
		obj = document.getElementById('SERVER_PORT');
		if( !obj.disabled )
			string += "SERVER_PORT=" + obj.value + "\n";
	
		obj = document.getElementById('DSRV_PORT');
		if( !obj.disabled )
			string += "DSRV_PORT=" + obj.value + "\n";
	
		obj = document.getElementById('GLOBALSRV_HOST');
		if( !obj.disabled )
			string += "GLOBALSRV_HOST=" + obj.value + "\n";
	
		obj = document.getElementById('GLOBALSRV_PORT');
		if( !obj.disabled )
			string += "GLOBALSRV_PORT=" + obj.value + "\n";
	
		if( opobj.value == 6 )
			string += "USERSRV_HOST=0.0.0.0\nUSERSRV_PORT=0\n";
	
		obj = document.getElementById('DATADB_HOST');
		if( !obj.disabled )
			string += "DATADB_HOST=" + obj.value + "\n";
	
		obj = document.getElementById('DATADB_USER');
		if( !obj.disabled )
			string += "DATADB_USER=" + obj.value + "\n";
	
		obj = document.getElementById('DATADB_PASS');
		if( !obj.disabled )
			string += "DATADB_PASS=" + obj.value + "\n";
	
		obj = document.getElementById('DATADB_DATABASE');
		if( !obj.disabled )
			string += "DATADB_DATABASE=" + obj.value + "\n";
	
		obj = document.getElementById('GAMEDB_HOST');
		if( !obj.disabled )
		{
			string += "GAMEDB_HOST=" + obj.value + "\n";
			sql += "dbh='"+obj.value+"'";
			hassql = 1;
		}
	
		obj = document.getElementById('GAMEDB_USER');
		if( !obj.disabled )
		{
			string += "GAMEDB_USER=" + obj.value + "\n";
			if( hassql == 1 )
				sql += ", ";
			sql += "dbu='"+obj.value+"'";
			hassql = 1;
		}
	
		obj = document.getElementById('GAMEDB_PASS');
		if( !obj.disabled )
		{
			string += "GAMEDB_PASS=" + obj.value + "\n";
			if( hassql == 1 )
				sql += ", ";
			sql += "dbp='"+obj.value+"'";
			hassql = 1;
		}
	
		obj = document.getElementById('GAMEDB_DATABASE');
		if( !obj.disabled )
		{
			string += "GAMEDB_DATABASE=" + obj.value + "\n";
			if( hassql == 1 )
				sql += ", ";
			sql += "dbd='"+obj.value+"'";
			hassql = 1;
		}
	
		obj = document.getElementById('LOGDB_HOST');
		if( !obj.disabled )
			string += "LOGDB_HOST=" + obj.value + "\n";
	
		obj = document.getElementById('LOGDB_USER');
		if( !obj.disabled )
			string += "LOGDB_USER=" + obj.value + "\n";
	
		obj = document.getElementById('LOGDB_PASS');
		if( !obj.disabled )
			string += "LOGDB_PASS=" + obj.value + "\n";
	
		obj = document.getElementById('LOGDB_DATABASE');
		if( !obj.disabled )
		{
			string += "LOGDB_DATABASE=" + obj.value + "\n";
			if( hassql == 1 )
				sql += ", ";
			sql += "log_dbd='"+obj.value+"'";
			hassql = 1;
		}
	
		obj = document.getElementById('MAX_CLUBCOUNT');
		if( !obj.disabled )
			string += "MAX_CLUBCOUNT=" + obj.value + "\n";
	
		obj = document.getElementById('MAX_CONNECTION');
		if( !obj.disabled )
			string += "MAX_CONNECTION=" + obj.value + "\n";
	
		obj = document.getElementById('MAX_CITYCOUNT');
		if( !obj.disabled )
			string += "MAX_CITYCOUNT=" + obj.value + "\n";
	
		obj = document.getElementById('SERVER_CODE');
		if( !obj.disabled )
			string += "SERVER_CODE=" + obj.value + "\n";
	
		obj = document.getElementById('SERVER_START_DATE');
		if( !obj.disabled )
			string += "SERVER_START_DATE=" + obj.value + "\n";
	
		obj = document.getElementById('SERVER_START_HOUR');
		if( !obj.disabled )
			string += "SERVER_START_HOUR=" + obj.value + "\n";
	
		obj = document.getElementById('SERVER_START_MIN');
		if( !obj.disabled )
			string += "SERVER_START_MIN=" + obj.value + "\n";
	
		obj = document.getElementById('SHOWFLAG');
		if( !obj.disabled )
			string += "SHOWFLAG=" + obj.value + "\n";
	
		obj = document.getElementById('NOPRINT');
		if( !obj.disabled )
			string += "NOPRINT=" + obj.value + "\n";

		obj = document.getElementById('USENETLOG');
		if( !obj.disabled )
			string += "USENETLOG=" + obj.value + "\n";

		obj = document.getElementById('WEBLOG_HOST');
		if( !obj.disabled )
			string += "WEBLOG_HOST=" + obj.value + "\n";

		obj = document.getElementById('WEBLOG_PORT');
		if( !obj.disabled )
			string += "WEBLOG_PORT=" + obj.value + "\n";

//		if( opobj.value == 6 )
//			string += "NOPRINT=0\n";
			
//		alert(string);
		if( hassql == 1 )
			sql += " where id='";
	}

	for( i = 0; i < o.length; i++ )
	{
		if( o[i].checked )
		{
			var x = document.getElementsByName('ip_arr[]');
			var y = document.getElementsByName('path_arr[]');
			var z = document.getElementsByName('id_arr[]');
			var m = document.getElementsByName('port_arr[]');
			
			sysip = x[i].value;
			syspath = y[i].value;
			sysid = z[i].value;
			sysport = m[i].value;
//			alert(opobj.value);
			if( hassql == 1 )
				sql += sysid+"';";
			else
				sql = "";
			if( opobj.value == 1 && sysport != 30001 )
				ajax_output( "hit"+i, "<div class=BlueText>端口30001的分发即可</div>" );
			else
				ajax_checkstat( "hit"+i, syspath, sysip, sysid, opobj.value, string, sql );

		}
	}
}

function opselect(obj)
{
	if( obj == null )
		return;
	document.getElementById("add_name1").innerHTML = "";
	document.getElementById("add_input1").innerHTML = "";
	document.getElementById("add_name2").innerHTML = "";
	document.getElementById("add_input2").innerHTML = "";
	document.getElementById("add_name3").innerHTML = "";
	document.getElementById("add_input3").innerHTML = "";
	document.getElementById("add_name4").innerHTML = "";
	document.getElementById("add_input4").innerHTML = "";
	document.getElementById("add_name5").innerHTML = "";
	document.getElementById("add_input5").innerHTML = "";
	document.getElementById("add_name6").innerHTML = "";
	document.getElementById("add_input6").innerHTML = "";
	document.getElementById("add_name7").innerHTML = "";
	document.getElementById("add_input7").innerHTML = "";
	document.getElementById("add_name8").innerHTML = "";
	document.getElementById("add_input8").innerHTML = "";
	document.getElementById("add_name9").innerHTML = "";
	document.getElementById("add_input9").innerHTML = "";
	document.getElementById("add_name10").innerHTML = "";
	document.getElementById("add_input10").innerHTML = "";
	document.getElementById("add_name11").innerHTML = "";
	document.getElementById("add_input11").innerHTML = "";
	document.getElementById("add_name12").innerHTML = "";
	document.getElementById("add_input12").innerHTML = "";
	document.getElementById("add_name13").innerHTML = "";
	document.getElementById("add_input13").innerHTML = "";
	document.getElementById("add_name14").innerHTML = "";
	document.getElementById("add_input14").innerHTML = "";
	document.getElementById("add_name15").innerHTML = "";
	document.getElementById("add_input15").innerHTML = "";

	switch( obj.value )
	{
	case '6':
	case '7':
		document.getElementById("add_name1").innerHTML = "服务器编号";
		document.getElementById("add_input1").innerHTML = "<font color=red>注意正确</font><input name=SERVER_CODE id=SERVER_CODE type=text size=16 value='1000' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'SERVER_CODE') >";

		document.getElementById("add_name2").innerHTML = "开服时间";
		document.getElementById("add_input2").innerHTML = "<font color=red>日期:</font><input name=SERVER_START_DATE id=SERVER_START_DATE type=text size=10 value='20160926' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'SERVER_START_DATE') >&nbsp;<font color=red>时:</font><input name=SERVER_START_HOUR id=SERVER_START_HOUR type=text size=4 value='1' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'SERVER_START_HOUR') >&nbsp;<font color=red>分:</font><input name=SERVER_START_MIN id=SERVER_START_MIN type=text size=4 value='0' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'SERVER_START_MIN') >";

		document.getElementById("add_name3").innerHTML = "端口";
		document.getElementById("add_input3").innerHTML = "游戏:<input name=SERVER_PORT id=SERVER_PORT type=text size=16 value='10001' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'SERVER_PORT') >&nbsp;GM:<input name=DSRV_PORT id=DSRV_PORT type=text size=16 value='30001' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'DSRV_PORT') >";

		document.getElementById("add_name4").innerHTML = "全局服务器";
		document.getElementById("add_input4").innerHTML = "HOST:<input name=GLOBALSRV_HOST id=GLOBALSRV_HOST type=text size=16 value='192.168.12.228' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'GLOBALSRV_HOST') >&nbsp;PORT<input name=GLOBALSRV_PORT id=GLOBALSRV_PORT type=text size=16 value='31000' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'GLOBALSRV_PORT') >";

		document.getElementById("add_name5").innerHTML = "Data数据库";
		document.getElementById("add_input5").innerHTML = "HOST:<input name=DATADB_HOST id=DATADB_HOST type=text size=42 value='kor-s1.c6ekaseib22e.us-east-1.rds.amazonaws.com' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'DATADB_HOST') >";
		document.getElementById("add_name6").innerHTML = "";
		document.getElementById("add_input6").innerHTML = "USER:<input name=DATADB_USER id=DATADB_USER type=text size=10 value='gamewrite' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'DATADB_USER') >&nbsp;PASS:<input name=DATADB_PASS id=DATADB_PASS type=text size=10 value='9dceA698' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'DATADB_PASS') >&nbsp;<font color=red>DB:</font><input name=DATADB_DATABASE id=DATADB_DATABASE type=text size=16 value='kor_data' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'DATADB_DATABASE') >";

		document.getElementById("add_name7").innerHTML = "Game数据库";
		document.getElementById("add_input7").innerHTML = "HOST:<input name=GAMEDB_HOST id=GAMEDB_HOST type=text size=42 value='kor-s1.c6ekaseib22e.us-east-1.rds.amazonaws.com' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'GAMEDB_HOST') >";
		document.getElementById("add_name8").innerHTML = "";
		document.getElementById("add_input8").innerHTML = "USER:<input name=GAMEDB_USER id=GAMEDB_USER type=text size=10 value='gamewrite' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'GAMEDB_USER') >&nbsp;PASS:<input name=GAMEDB_PASS id=GAMEDB_PASS type=text size=10 value='9dceA698' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'GAMEDB_PASS') >&nbsp;<font color=red>DB:</font><input name=GAMEDB_DATABASE id=GAMEDB_DATABASE type=text size=16 value='kor_game_' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'GAMEDB_DATABASE') >";

		document.getElementById("add_name9").innerHTML = "Log数据库";
		document.getElementById("add_input9").innerHTML = "HOST:<input name=LOGDB_HOST id=LOGDB_HOST type=text size=42 value='kor-s1.c6ekaseib22e.us-east-1.rds.amazonaws.com' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'LOGDB_HOST') >";
		document.getElementById("add_name10").innerHTML = "";
		document.getElementById("add_input10").innerHTML = "USER:<input name=LOGDB_USER id=LOGDB_USER type=text size=10 value='gamewrite' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'LOGDB_USER') >&nbsp;PASS:<input name=LOGDB_PASS id=LOGDB_PASS type=text size=10 value='9dceA698' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'LOGDB_PASS') >&nbsp;<font color=red>DB:</font><input name=LOGDB_DATABASE id=LOGDB_DATABASE type=text size=16 value='kor_log_' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'LOGDB_DATABASE') >";

		document.getElementById("add_name11").innerHTML = "最大值";
		document.getElementById("add_input11").innerHTML = "连接数:<input name=MAX_CONNECTION id=MAX_CONNECTION type=text size=10 value='2048' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'MAX_CONNECTION') >&nbsp;城市数:<input name=MAX_CITYCOUNT id=MAX_CITYCOUNT type=text size=10 value='80000' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'MAX_CITYCOUNT') >&nbsp;联盟数:<input name=MAX_CLUBCOUNT id=MAX_CLUBCOUNT type=text size=10 value='2048' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'MAX_CLUBCOUNT') >";

		document.getElementById("add_name12").innerHTML = "是否显示国旗";
		document.getElementById("add_input12").innerHTML = "<input name=SHOWFLAG id=SHOWFLAG type=text size=16 value='1' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'SHOWFLAG') >";

		document.getElementById("add_name13").innerHTML = "无输出";
		document.getElementById("add_input13").innerHTML = "<input name=NOPRINT id=NOPRINT type=text size=16 value='1' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'NOPRINT') >";

		document.getElementById("add_name14").innerHTML = "是否输出netlog";
		document.getElementById("add_input14").innerHTML = "<input name=USENETLOG id=USENETLOG type=text size=16 value='0' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'USENETLOG') >";

		document.getElementById("add_name15").innerHTML = "日志服务器";
		document.getElementById("add_input15").innerHTML = "HOST:<input name=WEBLOG_HOST id=WEBLOG_HOST type=text size=16 value='0.0.0.0' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'WEBLOG_HOST') >&nbsp;PORT<input name=WEBLOG_PORT id=WEBLOG_PORT type=text size=16 value='0' /> <input name='cfgop[]' type='checkbox' checked=true onclick=checkitem(this,'WEBLOG_PORT') >";

		if( obj.value == 7 )
		{
			cfgobj = document.getElementsByName('cfgop[]');
			for( i = 0; i < cfgobj.length; i++ )
			{
				cfgobj[i].checked=false;
			}
			disable_all();
		}
	default:
		break;
	}

	var opdesc = "";
	switch( obj.value )
	{
	case '1':
		opdesc = "将平台准备好的安装文件，分发到指定到服务器，这是更新服务器以及新装服务器到准备工作";
		break;
	case '2':
		opdesc = "启动指定服务器，如果有core文件或者savesql文件，注意先进行恢复和备份操作";
		break;
	case '3':
		opdesc = "停止指定服务器，注意发布停服公告";
		break;
	case '4':
		opdesc = "将准备好到服务器程序进行更新，包括更新程序和脚本，以及更新f_game_patch.sql，注意：没有包含f_data.sql";
		break;
	case '5':
		opdesc = "仅仅脚本，将从undeadServer.tar.gz获取script目录更新到服务器";
		break;
	case '6':
		opdesc = "在指定服务器部署程序以及配置文件，并且根据配置好的数据库地址尝试创建f_gamexxx和f_logxxx，并且执行f_game.sql以及f_log.sql";
		break;
	case '7':
		opdesc = "仅按照指定到配置选项修改配置文件，无其他操作，比如你要是修改了数据库地址，需要自行部署数据库";
		break;
	case '8':
		opdesc = "将所有到savesqlfile.sql和core文件移动到服务器目录下到一个日期目录，并且复制一份undeadServer程序到该目录";
		break;
	case '9':
		opdesc = "对savesqlfile.sql进行恢复，并且将所有到savesqlfile.sql和core文件移动到服务器目录下到一个日期目录，并且复制一份undeadServer程序到该目录";
		break;
	case '10':
		opdesc = "根据指定服务器的配置文件中的data数据库配置，来更新f_data.sql";
		break;
	case '11':
		opdesc = "在不停服的情况下，重新读取f_data表，使得新修改的数据生效";
		break;
	case '12':
		opdesc = "在不停服的情况下，重新读取lua脚本，使得脚本逻辑可以热更新";
		break;
	case '13':
		opdesc = "Dump出当前到f_game数据库，并且提供下载链接";
		break;
	case '14':
		opdesc = "主要用于开服前清空f_game数据库，注意配置文件的game数据库不要错误，服务器运行时或者开服时间后将禁止此操作";
		break;
	case '15':
		opdesc = "Dump出当前到f_game数据库，保存在本地的文件为f_game.sql.tar.gz";
		break;
	default:
		break;
	}
	document.getElementById("opdesc").innerHTML = opdesc;
}

</script>
</head>

<body>
<script type="text/javascript" src="upload/jquery.min.js"></script>
<script type="text/javascript" src="upload/jquery.form.js"></script>

<a href="tj3.php">返回</a>

<form id='addform' name='addform' method='post' action='serv_action.php'>
<table border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'>
	<th class='DataHeader'>内部编号</th>
	<th class='DataHeader'>服务器名称</th>
	<th class='DataHeader'>内部IP地址</th>
	<th class='DataHeader'>GM通道端口</th>
	<th class='DataHeader'>对外IP地址</th>
	<th class='DataHeader'>对外端口</th>
	<th class='DataHeader'><div class='RedText'>所在目录</div></th>
	<th class='DataHeader'></th></tr>
	<tr class='TableHeader'>
	<td class='Data DataRightAlign'><input name='id' size='6' type='text'></td>
	<td class='Data DataRightAlign'><input name='name' size='12' type='text'></td>
	<td class='Data DataRightAlign'><input name='ip' size='12' type='text' value='0.0.0.0'></td>
	<td class='Data DataRightAlign'><input name='port' size='6' type='text' value='30001'></td>
	<td class='Data DataRightAlign'><input name='out_ip' size='12' type='text' value='0.0.0.0'></td>
	<td class='Data DataRightAlign'><input name='out_port' size='6' type='text' value='10001'></td>
	<td class='Data DataRightAlign'><input name='servpath' size='48' type='text' value='/data/kor_'></td>
	<td class='Data DataRightAlign'><input type="submit" id="submit" name="submit" value="添加新服务器" /></td>
	</tr>
</table>
</form>
<br>

<table border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'>
	<th class='DataHeader'>文件名称</th>
	<th class='DataHeader'>文件大小</th>
	<th class='DataHeader'>修改日期</th>
	<th class='DataHeader'>MD5值</th>
	</tr>

<?php
$getinfo_str = "";
for( $tmpi = 0; $tmpi < 3; $tmpi++ )
{
	$localmd5 = md5_file("$g_uploadpath/$g_filename[$tmpi]");
	$localsize = filesize("$g_uploadpath/$g_filename[$tmpi]");
	$localtime=filemtime("$g_uploadpath/$g_filename[$tmpi]");
	$modtime = date("Y-m-d H:i:s",$localtime);
	if( $tmpi > 0 )
		$getinfo_str .= "|";
	$getinfo_str .= $g_filename[$tmpi];
	$getinfo_str .= "|".$localmd5;
	echo "<tr class='TableHeader'>";
	echo "<td class='Data DataRightAlign'>$g_filename[$tmpi]</td>";
	echo "<td class='Data DataRightAlign'>$localsize</td>";
	echo "<td class='Data DataRightAlign'>$modtime</td>";
	echo "<td class='Data DataRightAlign'>$localmd5</td>";
	echo "</tr>";
}
?>
	<tr class='TableHeader'>
	<td class='Data DataRightAlign'><div class="btn"><span>上传文件</span><input id="fileupload" type="file" name="myfile"></div></td>
	<td class='Data DataRightAlign'><div class="progress"><span class="bar"></span><span class="percent">0%</span ></div></td>
	<td class='Data DataRightAlign'><div class="files"></div></td>
	<td class='Data DataRightAlign'></td>
	</tr>
<script type="text/javascript">
$(function () {
	var bar = $('.bar');
	var percent = $('.percent');
	var progress = $(".progress");
	var files = $(".files");
	var btn = $(".btn span");
	$("#fileupload").wrap("<form id='myupload' action='upload_action.php' method='post' enctype='multipart/form-data'></form>");
    $("#fileupload").change(function(){
		$("#myupload").ajaxSubmit({
			dataType:  'json',
			beforeSend: function() {
				progress.show();
        		var percentVal = '0%';
        		bar.width(percentVal);
        		percent.html(percentVal);
				btn.html("上传中...");
    		},
    		uploadProgress: function(event, position, total, percentComplete) {
        		var percentVal = percentComplete + '%';
        		bar.width(percentVal);
        		percent.html(percentVal);
    		},
			success: function(data) {
				files.html("<b>"+data.name+"("+data.size+"k)</b>");
				var img = "files/"+data.pic;
				btn.html("成功-上传文件");
			},
			error:function(xhr){
				btn.html("失败-上传文件");
				bar.width('0')
				files.html(xhr.responseText);
			}
		});
	});
	
});
</script>

</table>
<br>
<table border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'>
	<td class='Data DataRightAlign'><a href="serv_list.php" target=_blank>服务器列表编辑</a></td>
	<td class='Data DataRightAlign'> | </td>
	<td class='Data DataRightAlign'><a href="serv_watch.php" target=_blank>监控列表编辑</a></td>
	</tr>
</table>

<br>
<?php
$fp= fopen( $hostlist_file, "r" );

$watch_ip = array();
$watch_port = array();
$watch_count = 0;
while (!feof($fp))
{
	$line = fgets($fp);
	$arr = explode( "|", $line );
	$strarr = explode(" ",$arr[0]);
	if ( !isset($arr[1]) )
		break;
	$watch_ip[]=$strarr[0];
	$watch_port[]=$strarr[1];
	$watch_count++;
}
fclose($fp);


echo "<form id='form1' name='form1' method='post' action='serv.php'>";

$db = new dbstuff;

$db->connect( $dbh, $dbu, $dbp, $dbd, 0 );

$sql="SELECT * FROM server_list where id>100 order by id";
$query = $db->query($sql);

echo"<table border='0' cellspacing='0' cellpadding='0' class='TableData' >";
echo "<tr class='TableHeader'><th class='DataHeader'>选项</th><th class='DataHeader'>ID</th><th class='DataHeader'>名称</th><th class='DataHeader'>对内地址</th><th class='DataHeader'>对外地址</th><th class='DataHeader'>所在目录</th><th class='DataHeader'>运行状态</th><th class='DataHeader'>在线</th><th class='DataHeader'>恢复存档时间</th><th class='DataHeader'>分发包状态</th><th class='DataHeader'>状态</th><th class='DataHeader'>进度</th><th class='DataHeader'>配置文件</th><th class='DataHeader'>监控</th></tr>";
$recno = 2;

$ip_arr = array();
$port_arr = array();
$id_arr = array();
$path_arr = array();
$index = 0;
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
	$syspath=$row[12];
	$outip=$row[13];
	$outport=$row[14];

	if( $recno == 1 )
		$recno = 2;
	else
		$recno = 1;

	echo"<tr class='Record_$recno'>";
	echo"<td class='Data DataRightAlign'><input name='favor[]' type='checkbox' value='$sysid' ></td>";
	echo"<td class='Data DataRightAlign'>$sysid<a href='setupagent.php?h=$sysip' target=_blank>重装Agent</a></td>";
	echo"<td class='Data DataRightAlign'>$sysname</td>";
	echo"<td class='Data DataRightAlign'>$sysip:$sysport</td>";
	echo"<td class='Data DataRightAlign'>$outip:$outport</td>";
	echo"<td class='Data DataRightAlign'>$syspath</td>";
	
	echo"<input name='ip_arr[]' type='hidden' value='$sysip' >";
	echo"<input name='path_arr[]' type='hidden' value='$syspath' >";
	echo"<input name='id_arr[]' type='hidden' value='$sysid' >";
	echo"<input name='port_arr[]' type='hidden' value='$sysport' >";

	$ip_arr[] = $sysip;
	$port_arr[] = $sysport;
	$id_arr[] = $sysid;
	$path_arr[] = $syspath;
	
	echo"<td class='Data DataRightAlign'><div id='Stat$index'><div class='RedText'><img src='loading.gif' /></div></div></td>";
	echo"<td class='Data DataRightAlign'><div id='Num$index'>0</div></td>";
/*
	$sock = fsockopen( "$sysip",$sysport,$errno,$errstr,3 );
	if( !$sock )
	{
		echo"<td class='Data DataRightAlign'><div class='RedText'>Disconnect</div></td>";
		echo"<td class='Data DataRightAlign'>0</td>";
	}
	else
	{
		fputs( $sock, "\0\0" );
		$result = fgets( $sock );
		fclose ($sock);

		$arr = explode("|", $result );		
		$player_count  = $arr[0];

		echo"<td class='Data DataRightAlign'><div class='GreenText'>Connected</div></td>";
		echo"<td class='Data DataRightAlign'>$player_count</td>";
	}
*/
	echo"<td class='Data DataLeftAlign'><span id='time$sysid'></span></td>";
	echo"<td class='Data DataLeftAlign'><span id='tarfile$sysid'></span></td>";
	echo"<td class='Data DataLeftAlign'><span id='jindu$sysid'></span><span id='hit$index'></span></td>";
	echo"<td class='Data DataLeftAlign'><span id='progress$sysid'></span></td>";
	echo"<td class='Data DataLeftAlign'><a href='serv_view.php?h=$sysip&dir=$syspath' target=_blank>查看</a></td>";

	$iswatch = 0;
	for ($tmpi= 0;$tmpi< $watch_count; $tmpi++)
	{
		if( $outip == $watch_ip[$tmpi] && $outport == $watch_port[$tmpi] )
		{
			$iswatch = 1;
			break;
		}
	}
	if( $iswatch == 1 )
	{
		echo"<td class='Data DataLeftAlign'> <a href='serv_watch.php' target=_blank><div class='GreenText'>监控中</div></a> </td>";
	}
	else
	{
		echo"<td class='Data DataLeftAlign'> <a href='serv_watch.php' target=_blank><div class='RedText'>未监控</div></a> </td>";
	}

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
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataRightAlign'> </td>";
echo"<td class='Data DataLeftAlign'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>";
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
	      <option value="1">A.分发服务器包(更新前以及开服前的准备工作)</option>
	      <option value="10">B.更新f_data.sql(为了避免意外，只允许选择一个服务器进行操作)</option>
	      <option value="4">C.更新服务器包(需要分发完最新服务器包以及服务器停止状态)</option>
	      <option value="5">D.更新脚本(需要分发完最新服务器包)</option>
	      <option value="2">E.启动服务器(服务器在停止的时候执行，注意！！！！！确定game_path执行完毕)</option>
	      <option value="3">F.关闭服务器(服务器在运行的时候执行)</option>
	      <option value="6">G.首次安装服务器包(需要上传最新服务器包)</option>
	      <option value="7">H.修改服务器配置(对server.cfg进行修改，仅修改勾选的选项)</option>
	      <option value="9">I.恢复savesqlfile.sql并转存core文件和复制服务器文件</option>
	      <option value="8">J.直接转存core文件和复制服务器文件</option>
	      <option value="11">K.刷新服务器使f_data生效[热更新，不停服](undeadServer sql)</option>
	      <option value="12">L.更新服务器lua脚本[热更新，不停服](undeadServer lua)</option>
	      <option value="13">M.导出game数据库，并提供下载(只可允许一个服务器进行)</option>
	      <option value="14">N.清空game数据库，原数据自动备份到/tmp目录</option>
	      <option value="15">O.导出game数据库，保存在本地</option>
	    </select>
	    </td>
	    <td><span id="opdesc"></span></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name1"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input1"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name2"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input2"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name3"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input3"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name4"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input4"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name5"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input5"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name6"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input6"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name7"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input7"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name8"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input8"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name9"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input9"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name10"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input10"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name11"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input11"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name12"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input12"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name13"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input13"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name14"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input14"></span></td><td></td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"><span id="add_name15"></span></th>
		<td class="Data DataLeftAlign"><span id="add_input15"></span></td><td></td>
	</tr>
	
	<tr class="Record_2">
		<th class="DataOfPrimaryKey DataRightAlign"> </th>
		<td class="Data DataRightAlign">
<!--		<input type="submit" id="submit" name="submit" value="提交" />    -->
		<input type="button" id="submit" name="submit" value="提交" onclick=submit_data() />
	    </td><td></td>
	</tr>
</table>

</form>
<br>

<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >
	<tr class='TableHeader'><th class='DataHeader'>情景</th><th class='DataHeader'>工作流</th></tr>
	<tr class="Record_1">
		<td class="Data DataRightAlign">更新服务器</td>
	    <td>上传服务器包->A->B->F->O->C->E</td>
	</tr>
	<tr class="Record_2">
		<td class="Data DataRightAlign">开新服务器</td>
	    <td>准备好服务器(安装好mysql等服务)->点击添加新服务器->安装Agent->A->G->E</td>
	</tr>
	<tr class="Record_1">
		<td class="Data DataRightAlign">故障维护</td>
	    <td>M->I->E</td>
	</tr>
	<tr class="Record_1">
		<td class="Data DataRightAlign">其他</td>
	    <td>D只更新脚本 H修改服务器配置 J只转存崩溃记录文件</td>
	</tr>
</table>
<script> opselect(document.getElementById('op'));</script>

<br><br><br><br><br><br>
<?php
ob_flush();
flush();
$num = count($ip_arr);

for ($tmpi= 0;$tmpi< $num; $tmpi++)
{
	echo "<script language=\"JavaScript\">";
	$sock = fsockopen( $ip_arr[$tmpi],$port_arr[$tmpi],$errno,$errstr,3 );
	if( !$sock )
	{
		echo "document.getElementById(\"Stat$tmpi\").innerHTML = \"<div class='RedText'>断开</div>\";\n";
	}
	else
	{
		fputs( $sock, "\0\0" );
		$result = fgets( $sock );
		fclose ($sock);

		$arr = explode("|", $result );		
		$player_count  = $arr[0];

		echo "document.getElementById(\"Stat$tmpi\").innerHTML = \"<div class='GreenText'>连接</div>\";\n";
	    echo "document.getElementById(\"Num$tmpi\").innerHTML = \"$player_count\";\n";

	}
	echo "ajax_checkstat( 'hit$tmpi', '$path_arr[$tmpi]', '$ip_arr[$tmpi]', '$id_arr[$tmpi]', '0', '$getinfo_str', '' );";
	echo "</script>";
	ob_flush();
	flush();
}
?>

</body>
</html>
