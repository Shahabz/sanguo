<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml">
 <head>
 <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 <title>无标题文档</title>
 </head>

 <body>
 <div style="float:left;background-color:#FFFFFF;width:200px;height:8px;padding:1px;border:#CCCCCC 1px solid;overflow:hidden;">
     <div id="jindu" style="float:left;width:0;height:8px;background-color:#669966;overflow:hidden;">&nbsp;</div>
   </div>
 <?php

 set_time_limit(0);
 $step = 0;

 for ($i = 0; $i < 20; $i++) {
   sleep(1); //这个是为了测试
   $step+=10; //实际步点你自己掌握
   echo '<script> document.getElementById("jindu").style.width = "'.$step.'px"; </script>';
   ob_flush(); //这个是为了测试
   flush(); //这个是为了测试 
 }
 ?>
 </body>
 </html> 
 