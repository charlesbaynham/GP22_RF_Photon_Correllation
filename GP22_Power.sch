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
Sheet 11 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3650 3675 0    39   Input ~ 0
3V3_in
Text HLabel 6675 3475 2    39   Input ~ 0
3V_out1
Text HLabel 6675 3900 2    39   Input ~ 0
3V_out2
$Comp
L XC6206 U10
U 1 1 58CEB17A
P 4925 3725
F 0 "U10" H 5075 3529 50  0000 C CNN
F 1 "XC6206" H 4925 3925 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 4925 3725 50  0001 C CNN
F 3 "" H 4925 3725 50  0000 C CNN
	1    4925 3725
	1    0    0    -1  
$EndComp
Text Notes 4725 3325 0    39   ~ 0
Low noise, low current power regulator\nLDR = 250mV so this should (just) work with 3V3
$Comp
L GND #PWR076
U 1 1 58CEB4F8
P 4925 3975
F 0 "#PWR076" H 4925 3725 50  0001 C CNN
F 1 "GND" H 4925 3825 50  0000 C CNN
F 2 "" H 4925 3975 50  0000 C CNN
F 3 "" H 4925 3975 50  0000 C CNN
	1    4925 3975
	1    0    0    -1  
$EndComp
Text Notes 3825 4375 0    39   ~ 0
Odd resistors etc are as per the recommended power supply by ACAM, in application note AN032\n
$Comp
L R_Small R36
U 1 1 58CEB528
P 3900 3675
F 0 "R36" H 3930 3695 50  0000 L CNN
F 1 "1k" H 3930 3635 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 3900 3675 50  0001 C CNN
F 3 "" H 3900 3675 50  0000 C CNN
	1    3900 3675
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 3675 3800 3675
Wire Wire Line
	4000 3675 4525 3675
$Comp
L C_Small C22
U 1 1 58CEB58C
P 4225 3775
F 0 "C22" H 4235 3845 50  0000 L CNN
F 1 "4u7" H 4235 3695 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4225 3775 50  0001 C CNN
F 3 "" H 4225 3775 50  0000 C CNN
	1    4225 3775
	1    0    0    -1  
$EndComp
Connection ~ 4225 3675
$Comp
L GND #PWR077
U 1 1 58CEB629
P 4225 3875
F 0 "#PWR077" H 4225 3625 50  0001 C CNN
F 1 "GND" H 4225 3725 50  0000 C CNN
F 2 "" H 4225 3875 50  0000 C CNN
F 3 "" H 4225 3875 50  0000 C CNN
	1    4225 3875
	1    0    0    -1  
$EndComp
$Comp
L R_Small R35
U 1 1 58CEBDDF
P 5575 3475
F 0 "R35" H 5605 3495 50  0000 L CNN
F 1 "22R" H 5605 3435 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5575 3475 50  0001 C CNN
F 3 "" H 5575 3475 50  0000 C CNN
	1    5575 3475
	0    1    1    0   
$EndComp
$Comp
L C_Small C21
U 1 1 58CEBE21
P 5800 3575
F 0 "C21" H 5810 3645 50  0000 L CNN
F 1 "33u" H 5810 3495 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5800 3575 50  0001 C CNN
F 3 "" H 5800 3575 50  0000 C CNN
	1    5800 3575
	1    0    0    -1  
$EndComp
Wire Wire Line
	5325 3475 5475 3475
Wire Wire Line
	5325 3475 5325 3900
Wire Wire Line
	5675 3475 6675 3475
Connection ~ 5800 3475
$Comp
L GND #PWR078
U 1 1 58CEBE87
P 5800 3675
F 0 "#PWR078" H 5800 3425 50  0001 C CNN
F 1 "GND" H 5800 3525 50  0000 C CNN
F 2 "" H 5800 3675 50  0000 C CNN
F 3 "" H 5800 3675 50  0000 C CNN
	1    5800 3675
	1    0    0    -1  
$EndComp
$Comp
L R_Small R37
U 1 1 58CEBF86
P 5575 3900
F 0 "R37" H 5605 3920 50  0000 L CNN
F 1 "22R" H 5605 3860 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5575 3900 50  0001 C CNN
F 3 "" H 5575 3900 50  0000 C CNN
	1    5575 3900
	0    1    1    0   
$EndComp
$Comp
L C_Small C23
U 1 1 58CEBF8C
P 5800 4000
F 0 "C23" H 5810 4070 50  0000 L CNN
F 1 "33u" H 5810 3920 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5800 4000 50  0001 C CNN
F 3 "" H 5800 4000 50  0000 C CNN
	1    5800 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5325 3900 5475 3900
Wire Wire Line
	5675 3900 6675 3900
Connection ~ 5800 3900
$Comp
L GND #PWR079
U 1 1 58CEBF95
P 5800 4100
F 0 "#PWR079" H 5800 3850 50  0001 C CNN
F 1 "GND" H 5800 3950 50  0000 C CNN
F 2 "" H 5800 4100 50  0000 C CNN
F 3 "" H 5800 4100 50  0000 C CNN
	1    5800 4100
	1    0    0    -1  
$EndComp
Connection ~ 5325 3675
$Comp
L PWR_FLAG #FLG080
U 1 1 58D215F9
P 6325 3475
F 0 "#FLG080" H 6325 3570 50  0001 C CNN
F 1 "PWR_FLAG" H 6325 3655 50  0000 C CNN
F 2 "" H 6325 3475 50  0000 C CNN
F 3 "" H 6325 3475 50  0000 C CNN
	1    6325 3475
	-1   0    0    1   
$EndComp
Connection ~ 6325 3475
$Comp
L PWR_FLAG #FLG081
U 1 1 58D21622
P 6325 3900
F 0 "#FLG081" H 6325 3995 50  0001 C CNN
F 1 "PWR_FLAG" H 6325 4080 50  0000 C CNN
F 2 "" H 6325 3900 50  0000 C CNN
F 3 "" H 6325 3900 50  0000 C CNN
	1    6325 3900
	-1   0    0    1   
$EndComp
Connection ~ 6325 3900
Text Notes 6250 3800 0    30   ~ 0
Power flags to inform Kicad that this is a power line
$Comp
L PWR_FLAG #FLG082
U 1 1 58D22AD5
P 4225 3675
F 0 "#FLG082" H 4225 3770 50  0001 C CNN
F 1 "PWR_FLAG" H 4225 3855 50  0000 C CNN
F 2 "" H 4225 3675 50  0000 C CNN
F 3 "" H 4225 3675 50  0000 C CNN
	1    4225 3675
	1    0    0    -1  
$EndComp
Text Notes 4675 4250 0    39   ~ 0
8796980 on Farnell
$EndSCHEMATC
