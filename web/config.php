<?php
	 $db_host = "host";
	 $db_user = "username";
	 $db_pass = "heslo";
	 $db_db = "databaze";
	  
	 $conn = new mysqli($db_host, $db_user, $db_pass, $db_db);
	 if ($conn->connect_error) {
	   die("Connection failed: " . $conn->connect_error);
	 }
	  
	 mysqli_set_charset($conn,"utf8");

?>