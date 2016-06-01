<!DOCTYPE html>
<html>
<body>

<?php
$start=microtime(TRUE);
$servername = "localhost";
$username = "root";
$password = "webqwebq";
$dbname = "moodleM";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
         die("Connection failed: " . $conn->connect_error);
}

for ($x = 0; $x <= 1000; $x++) {
$sql = "select id,category,sortorder from mdl_course";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
         // output data of each row
         while($row = $result->fetch_assoc()) {
                   //   echo "<br> id: ". $row["id"]. " - Name: ". $row["category"]. " " . $row["sortorder"] . "<br>";
                           }
} else {
         echo "0 results";
}
}
$conn->close();
$end=microtime(TRUE);
echo "<h1>".$limit."\n".($end-$start)." Seconds</h1>";
?> 

</body>
</html>
