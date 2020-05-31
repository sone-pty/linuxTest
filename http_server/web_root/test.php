<?php
	header("Cache-Control: no-cache");
	$name = $_REQUEST["name"];
	$res = "my name is ".$name;
	header("Content-Length: ".strlen($res));
	echo $res;
?>
