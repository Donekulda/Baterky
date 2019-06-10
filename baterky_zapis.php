<?php
	include "./config.php";

	//$kam = $_GET['druh'];
	if(isset($_GET['Napeti']) && isset($_GET['Proud']) && isset($_GET['Watt'])){
		$napeti = $_GET['Napeti'];
		$proud = $_GET['Proud'];
		$watt = $_GET['Watt'];
		$tabulka = $_GET['tabulka'];
	}elseif (isset($_POST['data'])) {
		$data = (float)$_POST['data'];
	}else{
		echo "data not recived";
	}

	$sql = "INSERT INTO ".$tabulka." (I, U, W) VALUES (".$proud.",".$napeti.",".$watt.")";
	if ($conn->query($sql) === TRUE) {
    	echo "New record created successfully";
	} else {
    	echo "Error: " . $sql . "<br>" . $conn->error;
	}

	$conn->close();
?>