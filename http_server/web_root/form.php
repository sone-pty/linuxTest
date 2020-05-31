<?php
	header("Cache-Control: no-cache");
	$first_name = $_REQUEST["firstName"];
	$last_name = $_REQUEST["lastName"];
	$res = "my name is ".$first_name.$last_name;
	header("Content-Length: ".strlen($res));
	echo $res;
?>
