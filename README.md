<<<<<<< HEAD
# TL - WiringPi

Objective
---------
To implement a traffic light system for pedestrian crossing, using event-driven programming approach.

Tools
-----
	Hardware:
	---------
		Raspberry Pi with SD Card installed with Raspbian
		One Ubuntu based PC
		One Ethernet Cable
		One Micro-USB Cable (for supplying power to RPi board by the PC)
		Pibrella Board
	Software:
	--------
		WiringPi GPIO Access Library, usable with C and C++ programming languages
		PUTTY for SSH Remote access

Product Specification
---------------------

		|---------------------------|-------------------------------------------------------|
		|		SPECIFICATION		|						OPERATION					    |
		|---------------------------|-------------------------------------------------------|
		|			1				|	- Blink 3 LEDs (Red, Green and Yellow) once for 1s  |
		|							|	  when the system is powered.						|
		|							|	- Ring buzzer for 1s simultaneously with the LED    |
		|							|	  blink.                                            |
		|---------------------------|-------------------------------------------------------|
		|			2				|	- Turn red LED ON and monitor Push button press     |
		|							|	  indefinitely.                                     |
		|---------------------------|-------------------------------------------------------|
		|			3				|	- When push-button is pressed, turn OFF Red LED     |
		|							|	  after 2s.                                         |
		|---------------------------|-------------------------------------------------------|
		|			4				|	- After red LED is OFF, turn ON green LED for 5s    |
		|---------------------------|-------------------------------------------------------|
		|			5				|	- Blink Green LED at 1Hz for 5 seconds.				|
		|							|	- Turn the Buzzer ON and OFF at 1Hz for 5 seconds.  |
		|---------------------------|-------------------------------------------------------|
		|			6				|	- Blink Green LED at 2Hz for 5 seconds.				|
		|							|	- Turn the Buzzer ON and OFF at 2Hz for 5 seconds.  |
		|---------------------------|-------------------------------------------------------|
		|			7				|	- Turn ON Yellow LED for 2 seconds.					|
		|							|	- Ring Buzzer for 2 seconds continuously.			|
		|---------------------------|-------------------------------------------------------|
		|			8				|	- Turn OFF Yellow LED and turn ON Red LED.			|
		|---------------------------|-------------------------------------------------------|
		|			9				|	- When Pushbutton is pressed, repeat specifications |
		|							|	  3 to 8.											|
		|---------------------------|-------------------------------------------------------|
		|			10				|	- Between steps 3 and 8, press of pushbutton shall  |
		|							|	  have no effect.									|
		|---------------------------|-------------------------------------------------------|

Background Information
----------------------

		|-------------------|-----------------------|----------------|
		|	PIN NUMBER		|	COMPONENT			|	TYPE		 |
		|-------------------|-----------------------|----------------|
		|		14			|	Push Button			|	Input		 |
		|-------------------|-----------------------|----------------|
		|		7			|	Red LED				|	Output		 |
		|-------------------|-----------------------|----------------|
		|		2			|	Green LED			|	Output		 |
		|-------------------|-----------------------|----------------|
		|		0			|	Yellow LED			| 	Output		 |
		|-------------------|-----------------------|----------------|
		|		1			|	Buzzer				|	PWM Output	 |
		|-------------------|-----------------------|----------------|

Compilation and Execution
-------------------------
	gcc -Wall -o traffic.out traffic.c -lwiringPi -lpthread
	./traffic.out
=======
# TL-WiringPi
>>>>>>> 26a9e8208ae87331689c7f78ab9f7ab0ffa18eb8
