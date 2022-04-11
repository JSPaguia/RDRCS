#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

const int extendButton = A5;                    // button to extend
const int retractButton = A4;                   // button to retract
const int retractLED = 19;		       // indicates system received retract command
const int extendLED = 18;                       // indicates system received extend command   
const int motor1relay1 = 10;
const int motor1relay2 = 11;
const int motor2relay1 = 12;
const int motor2relay1 = 13;
const int powerSwitch = x;	// Unsure if we need this
const int fan1 = A2;
const int fan2 = A3;
const int fan3 = 7;
const int fan4 = 8;
const int fan5 = 9;
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

int button1state = 0;
int button2state = 0;

int motor1Counter = 0;
int motor2Counter = 0;

float solarV;                                  // solar voltage value
float batteryV;                                // battery voltage value



void setup()                                   // input output
{
	pinMode(extendButton, INPUT);
	pinMode(retractButton, INPUT);
	pinMode(retractLED, OUTPUT);       
	pinMode(extendLED, OUTPUT);  
	
	pinMode (A2, OUTPUT);      //fan 1
	pinMode (A3, OUTPUT);      //fan 2
	pinMode (7, OUTPUT);       //fan 3
	pinMode (8, OUTPUT);       //fan 4
	pinMode (9, OUTPUT);       //fan 5

	pinMode (10, OUTPUT);      //motor 1 relay output 1 (DRIVE WHEELS)
	pinMode (11, OUTPUT);      //motor 1 relay output 2 (DRIVE WHEELS)
	pinMode (12, OUTPUT);      //motor 2 relay output 1 (SPOOL)
	pinMode (13, OUTPUT);      //motor 2 relay output 2 (SPOOL)

	digitalWrite (10, LOW);    //make sure both relays are off
	digitalWrite (11, LOW);
	digitalWrite (12, LOW);
	digitalWrite (13, LOW);
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
		
	button1state = digitalRead(extendButton);
	button2state = digitalRead(retractButton);
	
	//insert interrupt here to break standby loop
	
	if(button1state == 1) // extendButton is pushed
	{
		ERFlag = 1;
		extend();
	}
	
	if(button2state == 1)// retractButton is pushed
	{
		ERFlag == 0;
		retract();
	}
	
	if(button1state == 1 && button2state == 1){
		;
	}
	}
	
}

// Edge Case: If button is pressed at 100ms delay nothing happens

void extend()
{
	ERFlag = 1;
	
	for(motor1Counter = 0; motor1Counter<=180; motor1Counter++){
		digitalWrite(motor1relay1, HIGH);
	}
	
	for(motor2Counter = 0; motor2Counter<=180; motor2Counter++){
		digitalWrite(motor2relay1, HIGH);
	}
	
	if(motor1Counter == 180 && motor2Counter == 180){
		digitalWrite(motor1relay1, LOW);
		digitalWrite(motor2relay1, LOW);
	}
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
// Does 180 mean 3 minutes and how would we establish the counter?


void inflate()
{
	button1state = digitalRead(extendButton);
	
	digitalWrite (fan1,HIGH);
	digitalWrite (fan2,HIGH);
	digitalWrite (fan3,HIGH);
	
	fanFlag1 = 1;
	fanFlag2 = 1;
	fanFlag3 = 1;
	
	delay(1000);
	
	if(button1state == HIGH){
		digitalWrite(fan1,HIGH);
		digitalWrite(fan2,LOW);
		digitalWrite(fan3,LOW);
		
		fanFlag2 = 0;
		fanFlag3 = 0;
		
		button1state = digitalRead(extendbutton);
		if(button1state == LOW){
			digitalWrite(fan1,LOW);
			digitalWrite(fan2,LOW);
			digitalWrite(fan4,LOW);
		
			fanFlag1 = 0;
			fanFlag2 = 0;
			fanFlag3 = 0;
		}
	}
	else if(button1state == LOW){
		digitalWrite(fan1,LOW);
		digitalWrite(fan2,LOW);
		digitalWrite(fan4,LOW);
		
		fanFlag1 = 0;
		fanFlag2 = 0;
		fanFlag3 = 0;
	}
	
	diagnostic();
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
	button2state = digitalRead(retractButton);
	ERFlag = 0;
	ERSwitchFlag = 0;
	
	digitalWrite (6,HIGH);
	digitalWrite (7,HIGH);
	
	fanFlag4 = 1;
	fanFlag5 = 1;
	
	delay(1000);
	
	if(button2state == HIGH){
		digitalWrite(fan4,HIGH);
		digitalWrite(fan5,HIGH);
		
		delay(121000);
		
		digitalWrite(fan4,LOW);
		digitalWrite(fan5,LOW);
		
		digitalWrite(motor1relay2,HIGH);
		digitalWrite(motor2relay2,HIGH);
		
		delay(120000);
		
		if(motor1Counter == 180 && motor2Counter == 180){
			digitalWrite(motor1relay2,LOW);
			digitalWrite(motor2relay2,LOW);
		}
		
	}
	else if(button2state == LOW){
		digitalWrite(fan4,LOW);
		digitalWrite(fan5,LOW);
		digitalWrite(motor1relay2,LOW);
}
	
	diagnostic();
	
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
