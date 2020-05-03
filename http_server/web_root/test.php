<?php
	header("Cache-Control: no-cache");
	header("Content-Length: 13");
	echo "Hello world!".$_REQUEST["name"];
?>
