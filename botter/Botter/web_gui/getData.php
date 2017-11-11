<?php
$fp = file('list');
$response = '<table cellspacing="0">';
$color = "c1";
foreach($fp as $line){
    if($color === "c1") $color = "c2";
    else $color = "c1";
    $response .= '<tr><td class="'.$color.'">';
	$array = explode("-",$line);
	if($array[1]== 1) $response .= '<div class="connected"></div>';
	if($array[1] == 0) $response .= '<div class="disconnected"></div>';
    $response .= '<p>'.$array[0].'</p>';
    $response .= '</td></tr>';
}
$response .= '</table>';
echo $response;
?>