<?php
require_once 'db.inc.php';
$picname = $_FILES['myfile']['name'];
$picsize = $_FILES['myfile']['size'];
if ($picname != "") {
	if ($picsize > 102400000) {
		echo '不能超过100M';
		exit;
	}
	//上传路径
	$pic_path = $http_path."/undead_upload/". $picname;
	move_uploaded_file($_FILES['myfile']['tmp_name'], $pic_path);
}
$size = round($picsize/1024,2);
$arr = array(
	'name'=>$picname,
	'pic'=>$pics,
	'size'=>$size
);
echo json_encode($arr);
?>
