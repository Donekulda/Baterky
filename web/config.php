<?php
	 $db_host = "md40.wedos.net";
	 $db_user = "w212071_baterky";
	 $db_pass = "pRnUuHd4";
	 $db_db = "d212071_baterky";
	  
	 $conn = new mysqli($db_host, $db_user, $db_pass, $db_db);
	 if ($conn->connect_error) {
	   die("Connection failed: " . $conn->connect_error);
	 }
	  
	 mysqli_set_charset($conn,"utf8");

?>