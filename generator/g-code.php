<?php

const LIMIT_X = 200;
const LIMIT_Y = 50;
const LIMIT_Z = 30;

const Z_STEP = 0.01;
const Y_STEP = 2;


$stdin = fopen("php://stdin", "r");
echo "Type 'X' length (in millimeters): ";
$x_len = (int) trim(fgets(STDIN));
fclose($stdin);

$stdin = fopen("php://stdin", "r");
echo "Type 'Y' length: ";
$y_len = (int) trim(fgets(STDIN));
fclose($stdin);

$stdin = fopen("php://stdin", "r");
echo "Type 'Z' length: ";
$z_len = (float) trim(fgets(STDIN));
fclose($stdin);

$stdin = fopen("php://stdin", "r");
echo "Type 'Y_STEP' value: ";
$y_step = (float) trim(fgets(STDIN));
fclose($stdin);


if ($x_len > LIMIT_X) {
    $x_len = LIMIT_X;
}
if ((ceil($y_len / $y_step) * $y_step) > LIMIT_Y) {
    $y_len = LIMIT_Y;
}
if ($z_len > LIMIT_Z) {
    $z_len = LIMIT_Z;
}
if ($y_step > $y_len) {
    $y_len = $y_step;
}


$z_steps = ceil($z_len / Z_STEP);
$y_steps = ceil($y_len / $y_step);

$gcode = '(Surface Grinding)' . PHP_EOL;
$gcode .= 'G21 G40 G49 G54 G80 G90' . PHP_EOL;
$gcode .= 'M03 S3000' . PHP_EOL;
$gcode .= 'M07' . PHP_EOL;
$gcode .= 'G0 X0 Y0 Z0' . PHP_EOL;

$z_current = 0;

for ($i = 0; $i < ceil($z_steps / 2); $i++) {
    $x_current = 0;
    $y_current = 0;    
    
    /*------------------------------------*/
    /* Forward */
    $z_current += Z_STEP;
    
    $gcode .= 'G0 Z-' . $z_current . PHP_EOL;
    for ($j = 0; $j < $y_steps; $j++) {       
        if ($x_current == 0) {
            $x_current = $x_len;
        } else {
            $x_current = 0;
        } 
        $gcode .= 'G0 X' . $x_current . PHP_EOL;
        
        $y_current += $y_step;
        $gcode .= 'G0 Y' . $y_current . PHP_EOL;
    }
    if ($y_steps % 2 === 0) {
        $x_current = $x_len;
        $gcode .= 'G0 X' . $x_current . PHP_EOL;
    }
    
    /*------------------------------------*/
    /* Backward */  
    if ($z_current < $z_len) {
        $z_current += Z_STEP;
    }
    
    $gcode .= 'G0 Z-' . $z_current . PHP_EOL;
    for ($j = 0; $j < $y_steps; $j++) {       
        if ($x_current == 0) {
            $x_current = $x_len;
        } else {
            $x_current = 0;
        } 
        $gcode .= 'G0 X' . $x_current . PHP_EOL;
        
        $y_current -= $y_step;
        $gcode .= 'G0 Y' . $y_current . PHP_EOL;
    }
    
    /*------------------------------------*/
    /* Home */
    $gcode .= 'G0 X0' . PHP_EOL;
    $gcode .= 'G0 Y0' . PHP_EOL;
}

$gcode .= 'M09' . PHP_EOL;
$gcode .= 'M05' . PHP_EOL;
$gcode .= 'M30' . PHP_EOL;

$filename = 'g-code_' . $x_len . 'x' . $y_len . 'x' . $z_len . '_step' . $y_step . '.ngc';
file_put_contents($filename, $gcode);

echo "File generated: " . $filename . PHP_EOL;



