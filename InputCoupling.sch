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
Sheet 5 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4250 3400 0    60   Input ~ 0
InputSignal
Wire Wire Line
	4500 3400 4250 3400
Connection ~ 4500 3400
Text HLabel 4250 4200 0    60   Input ~ 0
InputGnd
Wire Wire Line
	4500 4200 4250 4200
Connection ~ 4500 4200
Text HLabel 7450 3400 2    60   Input ~ 0
OutputSignal
Text HLabel 7450 4100 2    60   Input ~ 0
OutputGnd
Wire Wire Line
	5000 3300 7450 3300
Wire Wire Line
	7450 3300 7450 3600
Wire Wire Line
	5000 4100 7450 4100
$Comp
L R R?
U 1 1 5898F4B0
P 6650 3750
F 0 "R?" V 6730 3750 50  0000 C CNN
F 1 "R" V 6650 3750 50  0000 C CNN
F 2 "" V 6580 3750 50  0000 C CNN
F 3 "" H 6650 3750 50  0000 C CNN
	1    6650 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3400 6650 3400
Wire Wire Line
	6650 3400 6650 3600
Wire Wire Line
	6650 3900 6650 4200
Wire Wire Line
	6650 4200 5000 4200
$Comp
L C_Small C?
U 1 1 5898F558
P 6750 3600
F 0 "C?" H 6760 3670 50  0000 L CNN
F 1 "C_Small" V 6800 3250 50  0000 L CNN
F 2 "" H 6750 3600 50  0000 C CNN
F 3 "" H 6750 3600 50  0000 C CNN
	1    6750 3600
	0    -1   -1   0   
$EndComp
$Comp
L C_Small C?
U 1 1 5898F595
P 6750 3900
F 0 "C?" H 6760 3970 50  0000 L CNN
F 1 "C_Small" V 6650 3950 50  0000 L CNN
F 2 "" H 6750 3900 50  0000 C CNN
F 3 "" H 6750 3900 50  0000 C CNN
	1    6750 3900
	0    1    1    0   
$EndComp
Connection ~ 6650 3900
Connection ~ 6650 3600
Wire Wire Line
	7450 3600 6850 3600
Wire Wire Line
	6850 3900 7450 3900
Wire Wire Line
	7450 3900 7450 4100
$Comp
L R R?
U 1 1 5898F62F
P 6000 3700
F 0 "R?" V 6080 3700 50  0000 C CNN
F 1 "R" V 6000 3700 50  0000 C CNN
F 2 "" V 5930 3700 50  0000 C CNN
F 3 "" H 6000 3700 50  0000 C CNN
	1    6000 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3550 6000 3300
Connection ~ 6000 3300
Wire Wire Line
	6000 3850 6000 4100
Connection ~ 6000 4100
$Comp
L CONN_02X02 P?
U 1 1 5898F928
P 4750 3350
F 0 "P?" H 4750 3500 50  0000 C CNN
F 1 "CONN_02X02" H 4750 3200 50  0000 C CNN
F 2 "" H 4750 2150 50  0000 C CNN
F 3 "" H 4750 2150 50  0000 C CNN
	1    4750 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3400 4500 3300
$Comp
L CONN_02X02 P?
U 1 1 5898F9B2
P 4750 4150
F 0 "P?" H 4750 4300 50  0000 C CNN
F 1 "CONN_02X02" H 4750 4000 50  0000 C CNN
F 2 "" H 4750 2950 50  0000 C CNN
F 3 "" H 4750 2950 50  0000 C CNN
	1    4750 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 4200 4500 4100
$EndSCHEMATC
