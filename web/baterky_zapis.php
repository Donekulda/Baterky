<?php
	include "./config.php";

	//$kam = $_GET['druh'];
	if(isset($_GET['Napeti']) && isset($_GET['Proud']) && isset($_GET['Watt'])){
		$napeti = $_GET['Napeti'];
		if(isset($_GET['Proud'])){ $proud = $_GET['Proud']; }else{$proud = 0.0;}
		if(isset($_GET['Watt'])){ $watt = $_GET['Watt']; }else{$watt=0.0;}
		$tabulka = $_GET['tabulka'];
		if(isset($_GET['Wh'])){ $Wh = $_GET['Wh']; }else{ $Wh = 0.0; }
	}elseif (isset($_POST['data'])) {
		$data = (float)$_POST['data'];
	}else{
		echo "data not recived";
	}

	$sql = "INSERT INTO ".$tabulka." (I, U, W, Wh) VALUES (".$proud.",".$napeti.",".$watt.",".$Wh.")";
	if ($conn->query($sql) === TRUE) {
    	echo "New record created successfully";
	} else {
    	echo "Error: " . $sql . "<br>" . $conn->error;
	}

	$conn->close();
?>