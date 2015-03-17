<?xml version='1.0' encoding='UTF-8'?>
<Library LVVersion="13008000">
	<Property Name="Instrument Driver" Type="Str">True</Property>
	<Property Name="NI.Lib.Description" Type="Str">Very basic driver for the arduino controlling a GP22 TDC eval board. 

This driver will read times between pulses on START and STOP1 as quickly as possible for a certain number of seconds, spitting this data into a text file. 

N.B. The configuration of the TDC has been stored in the device's EEPROM (solid state memory). It is reloaded on power up. To change it, use the eval software supplied by ACAM and the PICOPROG USB programmer. You must flash the Arduino blank first or disconnect it, otherwise it will hog the SPI interface. </Property>
	<Property Name="NI.Lib.Icon" Type="Bin">%Q#!!!!!!!)!"1!&amp;!!!-!%!!!@````]!!!!"!!%!!!)\!!!*Q(C=\&gt;1^ENJ!%)&lt;B4SY(&gt;KA&lt;O"1Z\SPI#FSB1[&gt;+)?-+@17OQ"8[#O3/&amp;*+/8Y;'82QM$LR6&gt;N6+WSR]]`=Q%J,[]58[L.WTY`TG]=@NL&lt;6?ZVM^NL&gt;[;??8?GS`D8^I@DW_X3&gt;Z;@ZN`1@)N&lt;XJT;.^N(_U`^@N4Q\[&lt;*^W?O&gt;$'LK53CIIJ[R`OZ&lt;E3:\E3:\E32\E12\E12\E1?\E4O\E4O\E4G\E2G\E2G\E2NZ0=J',8/4M3L*YMF!S;4*"UBG+EJ@%EXA34_,BIR*0YEE]C3@RU%7**`%EHM34?"CGR*.Y%E`C34R-V38:4X)]C9@J&amp;8A#4_!*0)'(*26Y!E#Q7$"R-!E-"9X"G]!4?!)0&lt;R6Y!E`A#4S"BW9&amp;HM!4?!*0Y'&amp;)XZ8IGMN*DI&gt;JZ(A=D_.R0)[(K?6Y()`D=4S/B_8E?"S0AX!7&gt;#;()'?1U](ZY(A=$``E?"S0YX%]DI?G@I7]\]R&amp;=TH*]2A?QW.Y$)`B91I:(M.D?!S0Y7&amp;;'2\$9XA-D_&amp;B+2E?QW.Y$)CR+-P,G-Q9;(1S!M0$8\^&lt;L&amp;_F[",L:`UULT?K[A:5X6CK'U:V)[AOM/L#K3[);K.6'[D;'.586HU2&amp;6#VM'J#65?NP*[I)X7A^N2#&lt;;C:GKDR-P1P&gt;VT86;@43=@D59@$1@P^8MOS;,0:;*ZH4&gt;/E=2TP4[NPH.&gt;DO$W8NLTPN@KQ_TI/OR`4M0M_$^O@SX"P?Y?[0J@_B7?D0KG^?MST2\]!MLWPP1!!!!!</Property>
	<Property Name="NI.Lib.SourceVersion" Type="Int">318799872</Property>
	<Property Name="NI.Lib.Version" Type="Str">1.0.0.0</Property>
	<Item Name="Examples" Type="Folder">
		<Item Name="typedefs" Type="Folder">
			<Item Name="Data.ctl" Type="VI" URL="../Examples/typedefs/Data.ctl"/>
			<Item Name="State.ctl" Type="VI" URL="../Examples/typedefs/State.ctl"/>
		</Item>
		<Item Name="GP22_TDC_Arduinoed Measurement.vi" Type="VI" URL="../Examples/GP22_TDC_Arduinoed Measurement.vi"/>
		<Item Name="GP22_TDC_Arduinoed Single Measurement.vi" Type="VI" URL="../Examples/GP22_TDC_Arduinoed Single Measurement.vi"/>
		<Item Name="StateMachine.vi" Type="VI" URL="../Examples/StateMachine.vi"/>
		<Item Name="Test Calibration wobble.vi" Type="VI" URL="../Examples/Test Calibration wobble.vi"/>
	</Item>
	<Item Name="Private" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">2</Property>
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Item Name="ConvertTimes - Array.vi" Type="VI" URL="../Private/ConvertTimes - Array.vi"/>
		<Item Name="ConvertTimes - Single.vi" Type="VI" URL="../Private/ConvertTimes - Single.vi"/>
		<Item Name="Default Instrument Setup.vi" Type="VI" URL="../Private/Default Instrument Setup.vi"/>
		<Item Name="ConvertTimes.vi" Type="VI" URL="../Private/ConvertTimes.vi"/>
	</Item>
	<Item Name="Public" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">1</Property>
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Item Name="Close.vi" Type="VI" URL="../Public/Close.vi"/>
		<Item Name="Initialize.vi" Type="VI" URL="../Public/Initialize.vi"/>
		<Item Name="VI Tree.vi" Type="VI" URL="../Public/VI Tree.vi"/>
		<Item Name="MeasureMode2.vi" Type="VI" URL="../Public/MeasureMode2.vi"/>
		<Item Name="Reset.vi" Type="VI" URL="../Public/Utility/Reset.vi"/>
		<Item Name="Self-Test.vi" Type="VI" URL="../Public/Self-Test.vi"/>
		<Item Name="SingleMeasurement.vi" Type="VI" URL="../Public/SingleMeasurement.vi"/>
		<Item Name="Calibrate.vi" Type="VI" URL="../Public/Calibrate.vi"/>
	</Item>
</Library>
