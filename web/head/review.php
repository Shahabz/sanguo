<?php
require_once 'FileUtil.php';
error_reporting(E_ALL);
$dir = "./upload/";  //要获取的目录
echo "********** 以下为待审核头像，默认为全部通过，哪个不符合要求就去掉哪个的框 ***********<hr/>";
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
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
</head>

<script language="javascript">
	
function sel( a, tmpi )
{
	obj=document.getElementsByName(a)
	if( tmpi<obj.length )
	{
		if( event.srcElement.checked )
			obj[tmpi].value=1;
		else
			obj[tmpi].value=0;
	}
}

</script>

<form id='form1' name='form1' method='post' action='review_action.php'>
<table  border='0' cellspacing='0' cellpadding='0' class='TableData' >

<tr class="Record_1">
<input type=checkbox checked='checked' onclick=sel('op[]')>全选
		
<?php
$total = 0;
$count = 0;
//先判断指定的路径是不是一个文件夹
if (is_dir($dir))
{
	if ($dh = opendir($dir))
	{
		while (($file = readdir($dh))!= false)
		{
			list($filesname,$kzm) = explode(".",$file);//获取扩展名
			if( $kzm=="gif" or $kzm=="jpg" or $kzm=="JPG" or $kzm=="png" or $kzm=="PNG" )
			{ //文件过滤
				
				 if (!is_dir('./'.$file)) 
				 { //文件夹过滤
						//文件名的全路径 包含文件名
						$filePath = $dir.$file;
						
						if ( $count == 0 )
						{
							echo"<tr>";
						}
						echo"<td class='Data DataRightAlign'>";
						echo "<img src='".$filePath."'/>";
						echo "<input name='op[]' type='checkbox' checked onclick=sel('checkitem[]','$total')>".$file."</input> ";
						echo"<input name='checkitem[]' type='hidden' value='1' >";
						echo"<input name='checkfile[]' type='hidden' value='".$file."' >";
						echo "</td>\n";
						
						$count = $count + 1;
						if ( $count == 10 )
						{
							echo"</tr>";
							$count = 0;
						}
						
						$total = $total + 1;
						if ( $total > 50 ) // 暂时一页50个
							break;
	
				 }
			}
		}
		closedir($dh);
	}
}

?>
	
</tr>

<tr class="Record_2">
	<th class="DataOfPrimaryKey DataRightAlign"> </th>
		<td class="DataOfPrimaryKey DataRightAlign" id="ok">
			<input type="submit" id="submit" name="submit" value="提交" onclick="" />
  	</td>
</tr>

</table>
</form>
</html>

