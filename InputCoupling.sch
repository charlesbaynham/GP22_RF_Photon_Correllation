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
Sheet 4 11
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3550 3400 0    60   Input ~ 0
InputSignal
Text HLabel 3525 4200 0    60   Input ~ 0
InputGnd
Wire Wire Line
	3525 4200 3750 4200
Text HLabel 7450 3400 2    60   Input ~ 0
OutputS+
Text HLabel 7450 4100 2    60   Input ~ 0
OutputS-
Wire Wire Line
	5000 4100 7450 4100
$Comp
L R R13
U 1 1 5898F4B0
P 6650 3750
AR Path="/5898F23C/5898F4B0" Ref="R13"  Part="1" 
AR Path="/589C38E1/5898F4B0" Ref="R19"  Part="1" 
AR Path="/589E6F10/5898F4B0" Ref="R29"  Part="1" 
F 0 "R13" V 6730 3750 50  0000 C CNN
F 1 "R" V 6650 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6580 3750 50  0001 C CNN
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
L C_Small C6
U 1 1 5898F558
P 6750 3600
AR Path="/5898F23C/5898F558" Ref="C6"  Part="1" 
AR Path="/589C38E1/5898F558" Ref="C8"  Part="1" 
AR Path="/589E6F10/5898F558" Ref="C10"  Part="1" 
F 0 "C6" H 6760 3670 50  0000 L CNN
F 1 "C_Small" V 6800 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6750 3600 50  0001 C CNN
F 3 "" H 6750 3600 50  0000 C CNN
	1    6750 3600
	0    -1   -1   0   
$EndComp
$Comp
L C_Small C7
U 1 1 5898F595
P 6750 3900
AR Path="/5898F23C/5898F595" Ref="C7"  Part="1" 
AR Path="/589C38E1/5898F595" Ref="C9"  Part="1" 
AR Path="/589E6F10/5898F595" Ref="C11"  Part="1" 
F 0 "C7" H 6760 3970 50  0000 L CNN
F 1 "C_Small" V 6650 3950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6750 3900 50  0001 C CNN
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
$Comp
L R R12
U 1 1 5898F62F
P 6000 3700
AR Path="/5898F23C/5898F62F" Ref="R12"  Part="1" 
AR Path="/589C38E1/5898F62F" Ref="R18"  Part="1" 
AR Path="/589E6F10/5898F62F" Ref="R28"  Part="1" 
F 0 "R12" V 6080 3700 50  0000 C CNN
F 1 "R" V 6000 3700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5930 3700 50  0001 C CNN
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
L CONN_02X02 P7
U 1 1 5898F928
P 4750 3350
AR Path="/5898F23C/5898F928" Ref="P7"  Part="1" 
AR Path="/589C38E1/5898F928" Ref="P9"  Part="1" 
AR Path="/589E6F10/5898F928" Ref="P11"  Part="1" 
F 0 "P7" H 4750 3500 50  0000 C CNN
F 1 "CONN_02X02" H 4750 3200 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x02_Pitch2.54mm" H 4750 2150 50  0001 C CNN
F 3 "" H 4750 2150 50  0000 C CNN
	1    4750 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3275 4500 3400
$Comp
L CONN_02X02 P8
U 1 1 5898F9B2
P 4750 4150
AR Path="/5898F23C/5898F9B2" Ref="P8"  Part="1" 
AR Path="/589C38E1/5898F9B2" Ref="P10"  Part="1" 
AR Path="/589E6F10/5898F9B2" Ref="P12"  Part="1" 
F 0 "P8" H 4750 4300 50  0000 C CNN
F 1 "CONN_02X02" H 4750 4000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x02_Pitch2.54mm" H 4750 2950 50  0001 C CNN
F 3 "" H 4750 2950 50  0000 C CNN
	1    4750 4150
	1    0    0    1   
$EndComp
$Comp
L R_Small R11
U 1 1 589D05E6
P 7000 3200
AR Path="/5898F23C/589D05E6" Ref="R11"  Part="1" 
AR Path="/589C38E1/589D05E6" Ref="R17"  Part="1" 
AR Path="/589E6F10/589D05E6" Ref="R27"  Part="1" 
F 0 "R11" H 7030 3220 50  0000 L CNN
F 1 "10k" H 7030 3160 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 7000 3200 50  0001 C CNN
F 3 "" H 7000 3200 50  0000 C CNN
	1    7000 3200
	1    0    0    -1  
$EndComp
Connection ~ 7000 3300
$Comp
L R_Small R10
U 1 1 589D066C
P 6850 3200
AR Path="/5898F23C/589D066C" Ref="R10"  Part="1" 
AR Path="/589C38E1/589D066C" Ref="R16"  Part="1" 
AR Path="/589E6F10/589D066C" Ref="R26"  Part="1" 
F 0 "R10" H 6880 3220 50  0000 L CNN
F 1 "10k" H 6880 3160 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 6850 3200 50  0001 C CNN
F 3 "" H 6850 3200 50  0000 C CNN
	1    6850 3200
	-1   0    0    1   
$EndComp
Connection ~ 6850 3300
$Comp
L R_Small R15
U 1 1 589D0720
P 7125 4200
AR Path="/5898F23C/589D0720" Ref="R15"  Part="1" 
AR Path="/589C38E1/589D0720" Ref="R25"  Part="1" 
AR Path="/589E6F10/589D0720" Ref="R31"  Part="1" 
F 0 "R15" H 7155 4220 50  0000 L CNN
F 1 "10k" H 7155 4160 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 7125 4200 50  0001 C CNN
F 3 "" H 7125 4200 50  0000 C CNN
	1    7125 4200
	1    0    0    -1  
$EndComp
$Comp
L R_Small R14
U 1 1 589D0771
P 6925 4200
AR Path="/5898F23C/589D0771" Ref="R14"  Part="1" 
AR Path="/589C38E1/589D0771" Ref="R24"  Part="1" 
AR Path="/589E6F10/589D0771" Ref="R30"  Part="1" 
F 0 "R14" H 6955 4220 50  0000 L CNN
F 1 "10k" H 6955 4160 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 6925 4200 50  0001 C CNN
F 3 "" H 6925 4200 50  0000 C CNN
	1    6925 4200
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR050
U 1 1 589D07ED
P 6850 3100
AR Path="/5898F23C/589D07ED" Ref="#PWR050"  Part="1" 
AR Path="/589C38E1/589D07ED" Ref="#PWR055"  Part="1" 
AR Path="/589E6F10/589D07ED" Ref="#PWR081"  Part="1" 
F 0 "#PWR081" H 6850 2850 50  0001 C CNN
F 1 "GND" H 6850 2950 50  0000 C CNN
F 2 "" H 6850 3100 50  0000 C CNN
F 3 "" H 6850 3100 50  0000 C CNN
	1    6850 3100
	-1   0    0    1   
$EndComp
Connection ~ 6925 4100
Connection ~ 7125 4100
$Comp
L GND #PWR051
U 1 1 589D0825
P 6925 4300
AR Path="/5898F23C/589D0825" Ref="#PWR051"  Part="1" 
AR Path="/589C38E1/589D0825" Ref="#PWR056"  Part="1" 
AR Path="/589E6F10/589D0825" Ref="#PWR082"  Part="1" 
F 0 "#PWR082" H 6925 4050 50  0001 C CNN
F 1 "GND" H 6925 4150 50  0000 C CNN
F 2 "" H 6925 4300 50  0000 C CNN
F 3 "" H 6925 4300 50  0000 C CNN
	1    6925 4300
	1    0    0    -1  
$EndComp
Text Notes 5375 3700 0    60   ~ 0
DC coupling
Text Notes 6475 4700 0    60   ~ 0
Optional pull-up / down / mid
Text Notes 6875 3700 0    60   ~ 0
AC coupling
Text Notes 3900 5425 0    60   ~ 0
N.B. the terminating 50 ohm resistors should not be connected for the "Straight through" option\n\nThe pull up / down resistors are optional and should be connected or not as required. \nFor the ground end of the signal, these resistors will only ever be required if the signal is treated differentially\n(e.g. as by the LTC6957)
$Comp
L +3V3 #PWR052
U 1 1 58BB05B7
P 7000 3100
AR Path="/5898F23C/58BB05B7" Ref="#PWR052"  Part="1" 
AR Path="/589C38E1/58BB05B7" Ref="#PWR057"  Part="1" 
AR Path="/589E6F10/58BB05B7" Ref="#PWR083"  Part="1" 
F 0 "#PWR083" H 7000 2950 50  0001 C CNN
F 1 "+3V3" H 7000 3240 50  0000 C CNN
F 2 "" H 7000 3100 50  0000 C CNN
F 3 "" H 7000 3100 50  0000 C CNN
	1    7000 3100
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR053
U 1 1 58BB05EA
P 7125 4300
AR Path="/5898F23C/58BB05EA" Ref="#PWR053"  Part="1" 
AR Path="/589C38E1/58BB05EA" Ref="#PWR058"  Part="1" 
AR Path="/589E6F10/58BB05EA" Ref="#PWR084"  Part="1" 
F 0 "#PWR084" H 7125 4150 50  0001 C CNN
F 1 "+3V3" H 7125 4440 50  0000 C CNN
F 2 "" H 7125 4300 50  0000 C CNN
F 3 "" H 7125 4300 50  0000 C CNN
	1    7125 4300
	-1   0    0    1   
$EndComp
Wire Wire Line
	5000 3300 7450 3300
Wire Wire Line
	7450 3300 7450 3600
Wire Wire Line
	7450 3900 7450 4100
Text Label 5325 3300 0    60   ~ 0
RF_1
Text Label 5325 3400 0    60   ~ 0
RF_2
Text Label 5325 4200 0    60   ~ 0
RF_4
Text Label 5325 4100 0    60   ~ 0
RF_3
$Comp
L ESDA5V3SC6 D1
U 1 1 58DC66C6
P 4050 3800
AR Path="/5898F23C/58DC66C6" Ref="D1"  Part="1" 
AR Path="/589C38E1/58DC66C6" Ref="D6"  Part="1" 
AR Path="/589E6F10/58DC66C6" Ref="D7"  Part="1" 
F 0 "D1" H 4050 3900 50  0000 C CNN
F 1 "ESDA5V3SC6" H 3900 4050 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5_HandSoldering" H 3750 3800 50  0001 C CNN
F 3 "" H 3750 3800 50  0000 C CNN
	1    4050 3800
	0    1    1    0   
$EndComp
Connection ~ 3750 4200
Wire Wire Line
	3550 3400 3750 3400
Wire Wire Line
	3750 3400 3750 3275
Wire Wire Line
	3750 3275 4500 3275
Connection ~ 4500 3300
Wire Wire Line
	4500 4100 4500 4200
Wire Wire Line
	4500 4200 4250 4200
Wire Wire Line
	4250 4200 4250 4325
Wire Wire Line
	4250 4325 3750 4325
Wire Wire Line
	3750 4325 3750 4200
NoConn ~ 4050 4200
NoConn ~ 4050 3400
$Comp
L GND #PWR054
U 1 1 58DC695B
P 3900 3450
AR Path="/5898F23C/58DC695B" Ref="#PWR054"  Part="1" 
AR Path="/589C38E1/58DC695B" Ref="#PWR059"  Part="1" 
AR Path="/589E6F10/58DC695B" Ref="#PWR085"  Part="1" 
F 0 "#PWR085" H 3900 3200 50  0001 C CNN
F 1 "GND" H 3900 3300 50  0000 C CNN
F 2 "" H 3900 3450 50  0000 C CNN
F 3 "" H 3900 3450 50  0000 C CNN
	1    3900 3450
	-1   0    0    1   
$EndComp
$EndSCHEMATC
