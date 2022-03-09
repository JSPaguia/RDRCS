/*
Startup loop:
Have both LEDs start flashing
Press one of the buttons to tell it that it is extended
Press other button to tell it that it is retracted
whichever LED lights up to indicate that the command was received

Main loop:
When starting up when the fan is not running (fan status flag == 0), run the inflation module.

*/

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

const int extendButton = 7;                    // button to extend
const int retractButton = 8;                   // button to retract
const int retractLED = 2;					   // indicates system received retract command
const int extendLED = 3;                       // indicates system received extend command       

int motorflag = 0;
int fanDiagnosticInputFlag = 0;
int fanStatusFlag = 0;
int switchStatusFlag = 0;
int batteryErrorFlag = 0;
int fanErrorFlag = 0;
int ERFlag = 0;                                // extend/retract flag 1 = extend, 0 = retract

int hallEffectCounter = 0;

float solarVoltage;
float batteryVoltage;





void setup()
{
	pinMode(extendButton, INPUT);
	pinMode(retractButton, INPUT);
	pinMode(retractLED, OUTPUT);       
	pinMode(extendLED, OUTPUT);                // input output
}

void loop()
{
	
	
	
	
	if(digitalRead(extendButton) == HIGH)      // extendButton is pushed
	{
		if(ERFlag == 0)                        // checks if system is retracted 
		{
			// run extend module ( contains LED confirmation )
		}
		
		if(ERFlag == 1)                        // do nothing if system is already extended
		{}
	}
	
	
	if(digitalRead(retractButton) == HIGH)     // retractButton is pushed
	{
		if(ERFlag == 0)                        // do nothing if system is already retracted
		{}
	
		if(ERFlag == 1)                        // checks if system is extended
		{
			// run retract module (contains LED confirmation)
		}
	}
	
	
}

void extend()
{
}






void retract()
{
}



void inflate()
{
}