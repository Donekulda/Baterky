﻿<?php
    include "./config.php";
  
	$value = 0;
  $Wh = 0;
    $druh="modra";
	if(isset($_GET['druh'])){
		$druh = $_GET['druh'];
		if($druh == "Modrá"){
			$druh = "modra";
		}else if($druh == "Fialová"){
			$druh = "fialova";
		}else if($druh == "Zelená"){
			$druh = "zelena";
		}else if($druh == "Žlutá"){
			$druh = "zluta";
		}
	}   
	
	if(isset($druh)){
        $volts = 0.0;
       
        $sql = "SELECT U FROM stav WHERE DATE_ADD(cas, INTERVAL 5 MINUTE) >= NOW()";  
        $result = $conn->query($sql);

        if ($result->num_rows > 0) {
          // output data of each row
          $i = 1;
          while($row = $result->fetch_assoc()) {
            if($i == $result->num_rows){
                $volts = $row['U'];
            }  
          }
        }
        
        $sql2 = "SELECT MAX(U), Min(U), COUNT(Wh), COUNT(W*0.0166666667) AS WattHour FROM ".$druh; //důvod proč to(watty na WattHodiny) počítam s minuty je ten že v původní verzi jsem měřil po minutách a v nové verzi měřím po 30 sekundách
        $result2 = $conn->query($sql2);
        
        if ($result2->num_rows > 0) {
          // output data of each row
          while($row = $result2->fetch_assoc()) {
            $max = $row['MAX(U)'];    
            $min = $row['Min(U)'];
            $Wh = $row['COUNT(Wh)'];
            if($Wh == 0 or $Wh == null or !isset($Wh))
              $Wh = $row['WattHour'];
            $delic = ($max-$min); //hodnota pro deleni voltu
            $volts-=$min;
            if($volts >= 0){
              $volts*=100;
              if($delic == 0)
                  $value=0;
              else 
                  $value = ($volts)/$delic;
            }else
                $value = 0;
          }
        } 	
	}
    
    $value = (int)$value;
    
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
  <link rel="stylesheet" type="text/css" href="css/tymovky_nabytost.css" media="all">

  <script type="text/javascript" src="script.js"></script>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="refresh" content="15">

</head>
<body>
<div class="navigace"></div>
<div class="block">
<div>
	<form method="Get" action="">
		<input type="submit" name="druh" value="Modrá">
		<input type="submit" name="druh" value="Fialová">
		<input type="submit" name="druh" value="Zelená">
		<input type="submit" name="druh" value="Žlutá">
	</form>
</div>
<form>
zadej procenta 

<div>


    <p><strong><?php echo ($value > 0)?"Baterie má ".$value."%.":"Baterie není připojená nebo je vybitá!"; ?></strong></p>
    <p><strong>Tato baterie má kapacitu <?php echo $Wh." Watt Hodin."; ?> </strong></p>
</div> 
<div
    data-type="fill"
    data-path="M10 10L90 10L90 90L10 90Z"
    class="ldBar"
    data-fill="data:ldbar/res,bubble(#00ff00,#000000,50,1)"
    data-value="<?php echo $value;?>"

    style="margin-top: 100px;">

  </div>
</div>
</body>
</html>
