#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

const int extendButton = x;                    // button to extend
const int retractButton = x;                   // button to retract
const int retractLED = x;		       // indicates system received retract command
const int extendLED = x;                       // indicates system received extend command   
const int motor1 = x;
const int motor2 = x;
const int powerSwitch = x;
const int fan1 = x;
const int fan2 = x;
const int fan3 = x;
const int fan4 = x;
const int fan5 = x;
// change these values depending on pin assignments^

int motorFlag1 = 0;
int motorFlag2 = 0;                            // motor flags

int fanFlag1 = 0;
int fanFlag2 = 0;
int fanFlag3 = 0;
int fanFlag4 = 0;
int fanFlag5 = 0;                              // fan flags

int fanDiagnosticInputFlag = 0;
int fanStatusFlag = 0;
int switchStatusFlag = 0;
int batteryErrorFlag = 0;
int fanErrorFlag = 0;                          // diagnostic/error/status flags

int ERFlag = 0;                                // extend/retract flag 1 = extend, 0 = retract              
int ERSwitchFlag = 0;                          // switch flag

int hallEffectCounter = 0;                     // counter for hall effect

float solarV;                                  // solar voltage value
float batteryV;                                // battery voltage value



void setup()                                   // input output
{
	pinMode(extendButton, INPUT);
	pinMode(retractButton, INPUT);
	pinMode(retractLED, OUTPUT);       
	pinMode(extendLED, OUTPUT);                
}

void loop()
{
	for()				       // LEDs blinking on startup
	{
	digitalWrite(retractLED, HIGH);
	digitalwrite(extendLED, HIGH);
	delay(100);
	digitalWrite(retractLED, LOW);
	digitalWrite(extendLED, LOW);
	delay(100);
	
	//insert interrupt here to break standby loop
	
	
	
	}
	
	
	if(digitalRead(extendButton) == HIGH) // extendButton is pushed
	{
		if(ERFlag == 0)               // checks if system is retracted 
		{
			extend();
		}
		
		if(ERFlag == 1)               // do nothing if system is already extended
		{}
	}
	
	
	if(digitalRead(retractButton) == HIGH)// retractButton is pushed
	{
		if(ERFlag == 0)               // do nothing if system is already retracted
		{}
	
		if(ERFlag == 1)               // checks if system is extended
		{
			retract();
		}
	}
	
	
}

void extend()
{
	ERFlag = 1;
/*
While (0 < motor 1 counter <= 180 (enough pulses for 70 ft) && is within __% of motor 2 counter)
set motor 1 direction 1 to HIGH
Else set to LOW (figure out how to do this)

While (0 < motor 2 counter <= 180 (enough pulses for 70 ft) && is within __% of motor 1 counter)
set motor 1 direction 1 to HIGH
Else set to LOW (figure out how to do this)

Have LED confirming that system is extending
*/
	inflate();
	diagnostic();
	
}


void inflate()
{
/*
Set all inflation fans to high
Set inflation fan status flags to high
Delay (1000)  //let fans get up to speed
Run diagnostic module
Delay (inflation time)
Set all but 1 inflation fan to LOW
Set all but the one fan status flag to 0
Run diagnostic module
*/
}


void retract()
{
/*
Set ER flag to 0
Set retract button flag to 0
Set all deflation fans to high
Set deflation fan status flags to high
Delay (1000)  //let fans get up to speed
Run diagnostic module
Delay (deflation time)
Set all fans to LOW

Set motor 1 direction 2 to high
Delay (100 ms?)
Set motor 2 direction 2 to high
When motor 1 counter == 180 (enough pulses for 70 ft), set motor 1 direction 2 to Low
When motor 2 counter == 180 (enough pulses for 70 ft), set motor 2 direction 2 to Low

Have LED confirming that system is retracting

Run diagnostic module
*/
}




void diagnostic()
{
/*
Check which fans have had a diagnostic flag set to high, 
compare to how many should be high, set the fan error flag to high if there is a discrepancy.
Reset all fan diagnostic flags to 0.

If BatteryV > analogReadbatteryvoltage + _arbitrary value_ (set battery error to high)
//compare battery voltage to value at the last check (before and after an extension/retraction), and if it has discharged a lot, set battery error flag to high.
BatteryV = analogRead Battery voltage

SolarV = analogRead Solar voltage
SolarVavg = add solarV into the running average, probably 1000 cycles?
*/
}


void solarMonitor()
{
}

void batteryMonitor()
{
}
