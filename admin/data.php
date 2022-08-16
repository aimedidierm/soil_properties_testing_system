<?php
require '../php-includes/connect.php';
require 'php-includes/check-login.php';
?>
<!DOCTYPE html>
<html>
<head>

	<title>Data taken</title>
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
	<h1>Data taken</h2>
		<table width="100%" class="table table-striped table-bordered table-hover" id="chatRoom">
        <thead>
            <th>Time</th>
			<th>N</th>
			<th>P</th>
			<th>K</th>
            <th>Recomanded</th>
            <th>Address</th>
		</thead>
		<tbody>
		<?php
			$sql = "SELECT * FROM dataa";
			$stmt = $db->prepare($sql);
			$stmt->execute();
			if ($stmt->rowCount() > 0) {
				$count = 1;
				while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
				?>
			<tr>
			    <td><?php print $row['time']?></td>
				<td><?php print $row['n']?></td>
				<td><?php print $row['p']?></td>
				<td><?php print $row['k']?></td>
                <td><?php print $row['recomanded']?></td>
                <td><?php print $row['address']?></td>
			</tr>
			<?php
			$count++;
			}
		}
		//gusiba umu client
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
	</div>
</div>
<script src="../js/jquery.dataTables.min.js"></script>
<script src="../js/dataTables.bootstrap.min.js"></script>
<script src="../js/dataTables.responsive.js"></script>	
</body>
</html>