<?php
require 'php-includes/connect.php';
//$data = array("c" =>"1","b" =>"20"); 
//echo $response = json_encode($data);
if(isset($_POST['n'])&&($_POST['p'])&&($_POST['k'])){
    $n=$_POST['n'];
    $p=$_POST['p'];
    $k=$_POST['k'];
    //Check if recommanded
    $sql = "SELECT *  FROM crops limit 1";
    $stmt = $db->prepare($sql);
    $stmt->execute();
    $row = $stmt->fetch(PDO::FETCH_ASSOC);
    $ndata=$row['n'];
    $pdata=$row['p'];
    $kdata=$row['k'];
    //condition
    if($n>=$ndata){
        $reco="Not recomanded";
        //$data = array("c" =>"1","b" =>"20"); 
        //echo $response = json_encode($data);
        $arr = array('c' => 1,'b' => 2);
        echo $data = json_encode($arr)."\n";
        $data;
    } else {
        $reco="Recomanded";
        $//data = array("c" =>"2","b" =>"20"); 
        //echo $response = json_encode($data);
        $arr = array('c' => 2,'b' => 2);
        echo $data = json_encode($arr)."\n";
        $data;
    }
    //insert data in database
    $sql ="INSERT INTO dataa (n,p,k,recomanded,address) VALUES (?,?,?,'Recommanded','656526263263,8484555')";
    $stm = $db->prepare($sql);
    if ($stm->execute(array($n,$p,$k))) {
        //
    } else{
        //
    }
}
?>