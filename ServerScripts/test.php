<?php
$limit = $_GET['limit'];
$start = microtime(TRUE);

$number = 1;
for($i=0;$i<$limit;$i++) {
	$number = $number + 2;
	$number = $number - 1;
	//echo $number . "<br/>";
}

$end = microtime(TRUE);
//echo "<h1>" . $start . "</h1>";
//echo "<h1>" .  $end . "</h1>";
echo "<h1>".$limit."\n".($end-$start)." Seconds</h1>";
?>

