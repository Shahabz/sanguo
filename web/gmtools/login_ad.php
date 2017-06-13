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
	--></style>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
</head>

<body>
    <form name="form1" method="post" action="loginad_action.php">
	<table border='0' cellspacing='0' cellpadding='0' class='TableData' align="center" width="180" >

	<tr class='TableHeader'>
		<th class='DataHeader'></th>
		<th class='DataHeader'>登录</th>
	</tr>
	
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">User:</th>
		<td class="Data DataRightAlign">
			<input type="text" name="name" id="name">
	    </td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign">Pass:</th>
		<td class="Data DataRightAlign">
			<input type="password" name="passwd" id="passwd">
	    </td>
	</tr>
	<tr class="Record_1">
		<th class="DataOfPrimaryKey DataRightAlign"></th>
		<td class="Data DataRightAlign">
			 <input type="submit" name="button" id="button" value="提交"> 
	    </td>
	</tr>

	</table>
    </form>
</body>
</html>