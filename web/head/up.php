<?php
require_once 'review_resize.php';
error_reporting(E_ALL);
if ($_FILES["thefile"]["error"] > 0)
{
	echo "Error: " . $_FILES["thefile"]["error"] . "<br />";
}
else
{
	echo "Upload: " . $_FILES["thefile"]["name"] . "<br />";
	echo "Type: " . $_FILES["thefile"]["type"] . "<br />";
	echo "Size: " . ($_FILES["thefile"]["size"] / 1024) . " Kb<br />";
	echo "Stored in: " . $_FILES["thefile"]["tmp_name"] . "<br />";

  if (file_exists("upload/" . $_FILES["thefile"]["name"]))
	{
		//echo $_FILES["thefile"]["name"] . " already exists. ";
		unlink("upload/" . $_FILES["thefile"]["name"]);
	}

	if ( !move_uploaded_file($_FILES["thefile"]["tmp_name"],"upload/" . $_FILES["thefile"]["name"]) )
	{
		echo "error";
	}
	else
	{
		echo "Stored in: " . "upload/" . $_FILES["thefile"]["name"];
	}
	
	$src_file = "upload/" . $_FILES["thefile"]["name"];
	$dst_file = "upload/" . $_FILES["thefile"]["name"];
	my_image_resize($src_file, $dst_file, 128 , 128);
}
?>
