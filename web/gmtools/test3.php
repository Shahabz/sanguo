<?php
header('Content-type: multipart/x-mixed-replace;boundary=endofsection');
print "\n--endofsection\n";

$pmt = array("-", "\\", "|", "/" );
for( $i = 0; $i <10; $i ++ )
{
	sleep(1);
	print "Content-type: text/html\n\n";
	print "Part $i\t".$pmt[$i % 4];
	print "--endofsection\n";
	ob_flush();
	flush();
}
print "Content-type: text/html\n\n";
print "<html>The end</html>\n";
print "--endofsection--\n";
?>
 