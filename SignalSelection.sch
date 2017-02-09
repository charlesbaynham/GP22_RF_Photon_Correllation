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
Sheet 7 11
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4525 2150 2    60   Input ~ 0
SineInput
Text HLabel 3675 2150 0    60   Input ~ 0
TTLInput
Text HLabel 3675 2050 0    60   Input ~ 0
StraightThroughInput
Text HLabel 5500 2850 2    60   Input ~ 0
SignalOutput
Text Notes 2525 1650 0    60   ~ 0
This sheet provides the user with a switch to select which of the three input signals will reach the output.\nThe output is to be wired to one of the three signal ports on the GP22. \nSignals are terminated here using Analog's recommended method for AC CMOS signals, as emitted by the LTC6957-4. \nN.B. The "straight through" signal is also terminated here, so the termintation resistor in the input coupling selection sheet should be omitted for this branch.\n\nAll signals are converted to single ended here if they weren't already.\nI.e. ground loops will happen here if you didn't choose AC coupling with the jumpers in the coupling selection sheets
$Comp
L R_Small R32
U 1 1 58A1DB89
P 4850 2950
AR Path="/58A17F47/58A1DB89" Ref="R32"  Part="1" 
AR Path="/58A6ADCB/58A1DB89" Ref="R33"  Part="1" 
AR Path="/58A6D694/58A1DB89" Ref="R34"  Part="1" 
F 0 "R34" H 4880 2970 50  0000 L CNN
F 1 "50R" H 4880 2910 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4850 2950 50  0001 C CNN
F 3 "" H 4850 2950 50  0000 C CNN
	1    4850 2950
	1    0    0    -1  
$EndComp
Connection ~ 4850 2850
$Comp
L GND #PWR062
U 1 1 58A1DBD1
P 4850 3250
AR Path="/58A17F47/58A1DBD1" Ref="#PWR062"  Part="1" 
AR Path="/58A6ADCB/58A1DBD1" Ref="#PWR063"  Part="1" 
AR Path="/58A6D694/58A1DBD1" Ref="#PWR064"  Part="1" 
F 0 "#PWR064" H 4850 3000 50  0001 C CNN
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
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4850 3150 50  0001 C CNN
F 3 "" H 4850 3150 50  0000 C CNN
	1    4850 3150
	1    0    0    -1  
$EndComp
Text Notes 5225 3225 0    60   ~ 0
Not sure about this capacitance: determine by trial and error!
Wire Wire Line
	3675 2350 3500 2350
Wire Wire Line
	3500 2350 3500 2850
Wire Wire Line
	3500 2850 5500 2850
NoConn ~ 4525 1950
$Comp
L ROTARY_SWITCH_ALPS_SRBM SW1
U 1 1 58DAF2B4
P 4100 2450
AR Path="/58A17F47/58DAF2B4" Ref="SW1"  Part="1" 
AR Path="/58A6ADCB/58DAF2B4" Ref="SW2"  Part="1" 
AR Path="/58A6D694/58DAF2B4" Ref="SW3"  Part="1" 
F 0 "SW3" H 3975 3110 50  0000 C CNN
F 1 "ROTARY_SWITCH_ALPS_SRBM" H 4125 2350 50  0000 C CNN
F 2 "Custom:SP3T_switch" H 3875 3010 50  0001 C CNN
F 3 "" H 3975 3110 50  0001 C CNN
	1    4100 2450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4525 2350 4525 2850
Connection ~ 4525 2850
NoConn ~ 4525 2050
NoConn ~ 3675 1950
$EndSCHEMATC
