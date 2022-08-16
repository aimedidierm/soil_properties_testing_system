<?php
require '../php-includes/connect.php';
require 'php-includes/check-login.php';
?>
<!DOCTYPE html>
<html>
<head>

	<title>Crops</title>
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
	<h1>Crops management</h2>
		<table width="100%" class="table table-striped table-bordered table-hover" id="chatRoom">
        <thead>
            <th>N</th>
			<th>Names</th>
			<th>N</th>
			<th>P</th>
			<th>K</th>
			<th></th>
		</thead>
		<tbody>
		<?php
			$sql = "SELECT * FROM crops";
			$stmt = $db->prepare($sql);
			$stmt->execute();
			if ($stmt->rowCount() > 0) {
				$count = 1;
				while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
				?>
			<tr>
			    <td><?php print $count?></td>
                <td><?php print $row['names']?></td>
				<td><?php print $row['n']?></td>
				<td><?php print $row['p']?></td>
				<td><?php print $row['k']?></td>
				<td><form method="POST"><button type="submit" class="btn btn-danger" id="<?php echo $row["id"];$mid=$row["id"]; ?>" name="delete"><span class="glyphicon glyphicon-trash"></span> Delete</button><form></td>

			</tr>
			<?php
			$count++;
			}
		}
		if(isset($_POST['delete'])){
		$sql ="DELETE FROM crops WHERE id = ?";
		$stm = $db->prepare($sql);
		if ($stm->execute(array($mid))) {
			print "<script>alert('Crop deleted');window.location.assign('crops.php')</script>";

		} else {
			print "<script>alert('Delete fail');window.location.assign('crops.php')</script>";
		}
		}
		?>
        </tbody>
    	</table> 
		<h1>Add new crop</h1>
		    <form method="POST">
			<?php
			if(isset($_POST['crop'])){
			$unames=$_POST['names'];
			$n=$_POST['n'];
			$p=$_POST['p'];
			$k=$_POST['k'];
				$sql ="INSERT INTO crops (names, n, p, k) VALUES (?,?,?,?)";
				$stm = $db->prepare($sql);
			if ($stm->execute(array($unames, $n, $p, $k))) {
				print "<script>alert('Crop Registered');window.location.assign('crops.php')</script>";
			} else{
				echo "<script>alert('Fail');window.location.assign('crops.php')</script>";
			}
			}
			?>
			<div style="height: 10px;"></div>
			<div class="form-group input-group">
				<span class="input-group-addon" style="width:150px;">Names:</span>
				<input type="text" style="width:350px;" class="form-control" name="names">
			</div>
			<div class="form-group input-group">
				<span class="input-group-addon" style="width:150px;">N:</span>
				<input type="text" style="width:350px;" class="form-control" name="n">
			</div>
			<div class="form-group input-group">
				<span class="input-group-addon" style="width:150px;">P:</span>
				<input type="text" style="width:350px;" class="form-control" name="p">
			</div>
			<div class="form-group input-group">
				<span class="input-group-addon" style="width:150px;">K:</span>
				<input type="text" style="width:350px;" class="form-control" name="k">
			</div>
			<button type="submit" class="btn btn-success" name="crop"><span class="glyphicon glyphicon-save"></span> Save</button>
		
		</div> 
	</form>
	</div>
</div>
<script src="../js/jquery.dataTables.min.js"></script>
<script src="../js/dataTables.bootstrap.min.js"></script>
<script src="../js/dataTables.responsive.js"></script>	
</body>
</html>