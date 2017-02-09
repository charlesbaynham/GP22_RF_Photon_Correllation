EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:max3440e
LIBS:Custom_Components
LIBS:GP22_board-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 2950 2700 0    60   Input ~ 0
SineInput
Text HLabel 2950 2850 0    60   Input ~ 0
TTLInput
Text HLabel 2950 3000 0    60   Input ~ 0
StraightThroughInput
Text HLabel 5500 2850 2    60   Input ~ 0
SignalOutput
Text Notes 2525 1650 0    60   ~ 0
This sheet provides the user with a switch to select which of the three input signals will reach the output.\nThe output is to be wired to one of the three signal ports on the GP22. \nSignals are terminated here using Analog's recommended method for AC CMOS signals, as emitted by the LTC6957-4. \nN.B. The "straight through" signal is also terminated here, so the termintation resistor in the input coupling selection sheet should be omitted for this branch.\n\nAll signals are converted to single ended here if they weren't already.\nI.e. ground loops will happen here if you didn't choose AC coupling with the jumpers in the coupling selection sheets
$Comp
L SP3T SW1
U 1 1 58A1D79D
P 4050 2850
AR Path="/58A17F47/58A1D79D" Ref="SW1"  Part="1" 
AR Path="/58A6ADCB/58A1D79D" Ref="SW3"  Part="1" 
AR Path="/58A6D694/58A1D79D" Ref="SW4"  Part="1" 
F 0 "SW4" H 3950 3000 50  0000 C CNN
F 1 "SP3T" H 3950 2750 50  0000 C CNN
F 2 "" H 3425 3025 50  0000 C CNN
F 3 "" H 3425 3025 50  0000 C CNN
	1    4050 2850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2950 2700 3750 2700
Wire Wire Line
	3750 2700 3750 2750
Wire Wire Line
	3750 2850 2950 2850
Wire Wire Line
	2950 3000 3750 3000
Wire Wire Line
	3750 3000 3750 2950
$Comp
L R_Small R32
U 1 1 58A1DB89
P 4850 2950
AR Path="/58A17F47/58A1DB89" Ref="R32"  Part="1" 
AR Path="/58A6ADCB/58A1DB89" Ref="R33"  Part="1" 
AR Path="/58A6D694/58A1DB89" Ref="R34"  Part="1" 
F 0 "R34" H 4880 2970 50  0000 L CNN
F 1 "50R" H 4880 2910 50  0000 L CNN
F 2 "" H 4850 2950 50  0000 C CNN
F 3 "" H 4850 2950 50  0000 C CNN
	1    4850 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 2850 5500 2850
Connection ~ 4850 2850
$Comp
L GND #PWR065
U 1 1 58A1DBD1
P 4850 3250
AR Path="/58A17F47/58A1DBD1" Ref="#PWR065"  Part="1" 
AR Path="/58A6ADCB/58A1DBD1" Ref="#PWR066"  Part="1" 
AR Path="/58A6D694/58A1DBD1" Ref="#PWR067"  Part="1" 
F 0 "#PWR067" H 4850 3000 50  0001 C CNN
F 1 "GND" H 4850 3100 50  0000 C CNN
F 2 "" H 4850 3250 50  0000 C CNN
F 3 "" H 4850 3250 50  0000 C CNN
	1    4850 3250
	1    0    0    -1  
$EndComp
$Comp
L C_Small C15
U 1 1 58A29BDD
P 4850 3150
AR Path="/58A17F47/58A29BDD" Ref="C15"  Part="1" 
AR Path="/58A6ADCB/58A29BDD" Ref="C16"  Part="1" 
AR Path="/58A6D694/58A29BDD" Ref="C17"  Part="1" 
F 0 "C17" H 4860 3220 50  0000 L CNN
F 1 "10p" H 4860 3070 50  0000 L CNN
F 2 "" H 4850 3150 50  0000 C CNN
F 3 "" H 4850 3150 50  0000 C CNN
	1    4850 3150
	1    0    0    -1  
$EndComp
Text Notes 5225 3225 0    60   ~ 0
Not sure about this capacitance: determine by trial and error!
$EndSCHEMATC
