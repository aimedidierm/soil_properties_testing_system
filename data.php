<?php
require 'php-includes/connect.php';
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
    if($n>$ndata){
        $arr = array('c' => 1,'b' => 2);
        $de="HIGH";
        echo $data = json_encode($arr)."\n";
    } else if($p>$pdata){
        $arr = array('c' => 1,'b' => 2);
        $de="HIGH";
        echo $data = json_encode($arr)."\n";
    } else if($k>$kdata){
        $arr = array('c' => 1,'b' => 2);
        $de="HIGH";
        echo $data = json_encode($arr)."\n";
    } else if($n<$ndata){
        $arr = array('c' => 3,'b' => 2);
        $de="LOW";
        echo $data = json_encode($arr)."\n";
    } else if($p<$pdata){
        $arr = array('c' => 3,'b' => 2);
        $de="LOW";
        echo $data = json_encode($arr)."\n";
    } else if($k<$kdata){
        $arr = array('c' => 3,'b' => 2);
        $de="LOW";
        echo $data = json_encode($arr)."\n";
    } else {
        $arr = array('c' => 2,'b' => 2);
        $de="ENOGHT";
        echo $data = json_encode($arr)."\n";
    }
    //insert data in database
    $sql ="INSERT INTO dataa (n,p,k,recomanded,address) VALUES (?,?,?,?,'656526263263,8484555')";
    $stm = $db->prepare($sql);
    if ($stm->execute(array($n,$p,$k,$de))) {
        //
    } else{
        //
    }
}
?>