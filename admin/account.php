<?php
require '../php-includes/connect.php';
require 'php-includes/check-login.php';

?>
<!DOCTYPE html>
<html>
<head>

	<title>Admin Account</title>
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js"></script>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" />
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
	
	<link href="../css/dataTables.bootstrap.css" rel="stylesheet">
	<link href="../css/dataTables.responsive.css" rel="stylesheet">

</head>
<body>
<?php require 'php-includes/menu.php'; ?>
<div class="container">
	<div class="row">
	<div class="col-lg-12">
                <div class="col-lg-8">
				<span style="font-size:18px; margin-left:10px; margin-buttom:10px position:relative; top:13px;"><strong><span class="glyphicon glyphicon-user"></span> My Account</strong></span>
				<div class="container-fluid">
					<?php
					$sql = "SELECT *  FROM admin WHERE email = ? limit 1";
					$stmt = $db->prepare($sql);
					$stmt->execute(array($_SESSION['email']));
					$row = $stmt->fetch(PDO::FETCH_ASSOC);
					$names=$row['names'];
					$phone=$row['phone'];
					$email=$row['email'];
					$mpassword=$row['password'];
					if(isset($_POST['update'])){
					$unames=$_POST['names'];
					$uphone=$_POST['phone'];
					$cpassword=md5($_POST['cpassword']);
					$apassword=md5($_POST['apassword']);
					if ($apassword == $cpassword){
						if($apassword == $mpassword){
							$sql ="UPDATE admin SET phone = ? WHERE email = ?";
                            $stm = $db->prepare($sql);
                            if ($stm->execute(array($uphone, $_SESSION['email']))) {
                                print "<script>alert('your data updated');window.location.assign('account.php')</script>";

                                }
						} else{
							echo "<script>alert('Comfirmation password is incorect');window.location.assign('account.php')</script>";
						}
				    } else{
						echo "<script>alert('Passwords are not match');window.location.assign('account.php')</script>";
					}
				    }
					?>
				<form method="POST">
					<div style="height: 10px;"></div>
					<div class="form-group input-group">
						<span class="input-group-addon" style="width:150px;">Name:</span>
						<input type="text" style="width:350px;" class="form-control" name="names" value="<?php echo $row['names']; ?>" disabled>
					</div>
					<div class="form-group input-group">
						<span class="input-group-addon" style="width:150px;">Email:</span>
						<input type="text" style="width:350px;" class="form-control" name="email" value="<?php echo $row['email']; ?>" disabled>
					</div>
					<div class="form-group input-group">
						<span class="input-group-addon" style="width:150px;">Phone:</span>
						<input type="text" style="width:350px;" class="form-control" name="phone" value="<?php echo $row['phone']; ?>">
					</div>
					<hr>
					<span>Enter current password to save changes:</span>
					<div style="height: 10px;"></div>
					<div class="form-group input-group">
						<span class="input-group-addon" style="width:150px;">Password:</span>
						<input type="password" style="width:350px;" class="form-control" name="cpassword">
					</div>
					<div class="form-group input-group">
						<span class="input-group-addon" style="width:150px;">Again:</span>
						<input type="password" style="width:350px;" class="form-control" name="apassword">
					</div>
					<button type="submit" class="btn btn-success" name="update"><span class="glyphicon glyphicon-check"></span> Update</button>
				
                </div> 
			</form>
            </div>
	</div>
</div>
<script src="../js/jquery.dataTables.min.js"></script>
<script src="../js/dataTables.bootstrap.min.js"></script>
<script src="../js/dataTables.responsive.js"></script>	
</body>
</html>