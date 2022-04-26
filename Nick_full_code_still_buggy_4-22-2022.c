/*
Rapid Deployment Runway Closure System C Code
By: Marc Tanwangco, Jomari Paguia, Nicolas Wolford
Benham Aviation Services
*/

// FANS
const int fan1 = A2;  // Fan 1
const int fan2 = A3;  // Fan 2
const int fan3 = 7; // Fan 3
const int fan4 = 8; // Fan 4
const int fan5 = 9; // Fan 5
int FF1 = 0;  // Fan Flag 1
int FF2 = 0;  // Fan Flag 2
int FF3 = 0;  // Fan Flag 3
int FF4 = 0;  // Fan Flag 4
int FF5 = 0;  // Fan Flag 5

// MOTORS
const int M1R1 = 10;  // Motor 1 Relay 1 (DRIVE WHEELS)
const int M1R2 = 11;  // Motor 1 Relay 2 (DRIVE WHEELS)
const int M2R1 = 12;  // Motor 2 Relay 1 (SPOOL)
const int M2R2 = 13;  // Motor 2 Relay 2 (SPOOL)
const int W1P = 12;  // Number of Pulses for Wheel System (Motor 1)
const int S2P = 11;  // Number of Pulses for Spool System (Motor 2)
volatile byte M1C = 0;  // Motor 1 Counter
volatile byte M2C = 0;  // Motor 2 Counter
bool MF1 = false;  // Motor Flag 1
bool MF2 = false;  // Motor Flag 2 

// BUTTONS & LEDs
const int extendbutton = A5;  // Extend Button
const int retractbutton = A4; // Retract Button
volatile byte buttonstate1 = digitalRead(extendbutton);    // Button 1 is Associated with Extending
volatile byte buttonstate2 = digitalRead(retractbutton);   // Button 2 is Associated with Retracting
volatile byte laststate1 = LOW; // Last Button 1 State
volatile byte laststate2 = LOW; // Last Button 2 State
const int retractLED = 6; // Indicates System Received Retract Command
const int extendLED = 5;  // Indicated System Received Extend/Inflate Command
volatile byte LEDstate1 = LOW;  // LED 1 State, Associated with extendLED
volatile byte LEDstate2 = LOW;  // LED 2 State, Associated with retractLED
unsigned long debounceDuration = 50;  // 50 milliseconds
unsigned long lastchange = 0;

// FLAGS AND COUNTERS
int FDIF = 0; // Fan Diagnostic Input Flag
int FSF = 0;  // Fan Status Flag
int SSF = 0;  // Switch Status Flag
int BEF = 0;  // Battery Error Flag
int FEF = 0;  // Fan Error Flag
int ERF = 0;  // Extend/Retract Flag, Flag 1 = Extend/Flag 0 = Retract
int ERSF = 0; // Extend/Retract Switch Flag
//int HECounter = 0;  // Hall Effect Counter

// HALL EFFECT SENSORS
//volatile byte HElaststate1 = 0;
//volatile byte HElaststate2 = 0;
unsigned long HEdebounceDuration = 500;  // 200 milliseconds
unsigned long HElastchange1 = 0;
unsigned long HElastchange2 = 0;



float solarV;
float batteryV;

/*
---------------------------------------------------------------------------------
*/
 
void setup() {
  // put your setup code here, to run once:
  // Serial.begin(9600);
  // Serial.println("test");
  pinMode (extendbutton, INPUT_PULLUP);
  pinMode (retractbutton, INPUT_PULLUP);

  pinMode(extendLED, OUTPUT);
  pinMode(retractLED, OUTPUT);

  digitalWrite (extendLED, LOW);    // Makes sure both relays are off
  digitalWrite (retractLED, LOW);
  
  pinMode (fan1, OUTPUT);    
  pinMode (fan2, OUTPUT);     
  pinMode (fan3, OUTPUT);      
  pinMode (fan4, OUTPUT);      
  pinMode (fan5, OUTPUT);    

  pinMode (M1R1, OUTPUT);      // motor 1 relay output 1 (DRIVE WHEELS)
  pinMode (M1R2, OUTPUT);      // motor 1 relay output 2 (DRIVE WHEELS)
  pinMode (M2R1, OUTPUT);      // motor 2 relay output 1 (SPOOL)
  pinMode (M2R2, OUTPUT);      // motor 2 relay output 2 (SPOOL)

  digitalWrite (M1R1, LOW);    // Makes sure both relays are off
  digitalWrite (M1R2, LOW);
  digitalWrite (M2R1, LOW);
  digitalWrite (M2R2, LOW);

  pinMode(2, INPUT_PULLUP); // Hall Effect 1 (Wheels)
  pinMode(3, INPUT_PULLUP); // Hall Effect 2 (Spool)
  
  attachInterrupt(digitalPinToInterrupt(2), halleffect1, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), halleffect2, FALLING);

  pinMode(A0, INPUT); // Battery Voltage Divider
  pinMode(A1, INPUT); // Solar Panel Voltage Divider

  delay(50);

  // pulseIn(extendbutton, LOW, 3000000);

  // if(pulseIn(extendbutton, LOW, 3000000) > 1000000){
    // digitalWrite (extendLED, HIGH);
    // digitalWrite (retractLED, HIGH);
  // }

  // while(buttonstate1 == HIGH && buttonstate2 == HIGH){
    // buttonstate1 = digitalRead(extendbutton);
  // }
}

/*
---------------------------------------------------------------------------------
*/

void loop() {
  // put your main code here, to run repeatedly:

  if(millis() - lastchange > debounceDuration){
    buttonstate1 = digitalRead(extendbutton);
    if(buttonstate1 != laststate1){
      lastchange = millis();
      laststate1 = buttonstate1;
      if(buttonstate1 == LOW){
        if(LEDstate1 == HIGH){
          LEDstate1 = LOW;
          extend();
          } 
          else{
          LEDstate1 = HIGH;
          }
        digitalWrite(extendLED, LEDstate1);
      }
    }
  }

  if(millis() - lastchange > debounceDuration){
    buttonstate2 = digitalRead(retractbutton);
    if(buttonstate2 != laststate2){
      lastchange = millis();
      laststate2 = buttonstate2;
      if(buttonstate2 == LOW){
        if(LEDstate2 == HIGH){
          LEDstate2 = LOW;
          retract();
        } else{
          LEDstate2 = HIGH;
        }
        digitalWrite(retractLED, LEDstate2);
      }
    }
  }
}

/*
---------------------------------------------------------------------------------
*/
 
void extend(){
  M1C = 0;
  M2C = 0;
  digitalWrite(M1R1, LOW);
  digitalWrite(M2R1, LOW);
  digitalWrite(M1R2, HIGH);
  digitalWrite(M2R2, HIGH);
  MF1 = true;
  MF2 = true;
  while((MF1 == true && MF2 == true) || (MF1 != MF2)) {
    if (M1C >= W1P) {
      delay(100);
      digitalWrite(M1R2, LOW);
      MF1 = false;
    }
    if (M2C >= S2P) {
      delay(100);
      digitalWrite(M2R2, LOW);
      MF2 = false;
    }
  }
  inflate();
}

/*
---------------------------------------------------------------------------------
*/

void inflate(){
  
  digitalWrite(fan1, HIGH);
  digitalWrite(fan3, HIGH);
  digitalWrite(fan5, HIGH);
    
  delay(35000);

  digitalWrite(fan3, LOW);
  digitalWrite(fan5, LOW);

  delay(100);
  // Night();
}

/*
---------------------------------------------------------------------------------
*/

void retract(){
  digitalWrite(fan1, LOW);
  digitalWrite(fan3, LOW);
  digitalWrite(fan5, LOW); 
  delay(100);
  
  ERF = 0;
  ERSF = 0;
  
  digitalWrite(fan2, HIGH);
  digitalWrite(fan4, HIGH);

  FF2 = 1;
  FF4 = 1;
    
  delay(70000);
  
  digitalWrite(fan2, LOW);  
  digitalWrite(fan4, LOW);
  
  delay(100);
  
  M1C = 0;
  M2C = 0;
  digitalWrite(M1R2, LOW);
  digitalWrite(M2R2, LOW);
  digitalWrite(M1R1, HIGH);
  digitalWrite(M2R1, HIGH);
  MF1 = true;
  MF2 = true;
  while((MF1 == true && MF2 == true) || (MF1 != MF2)) {
    if (M1C >= W1P) {
      delay(100);
      digitalWrite(M1R1, LOW);
      MF1 = false;
    }
    if (M2C >= S2P) {
      delay(100);
      digitalWrite(M2R1, LOW);
      MF2 = false;
    }
  }
 /* 
  while (M1C <= W1P && M2C <= S2P); {
    // Do Nothing
  }
  if (M1C >= W1P){
    delay(50);
    digitalWrite(M1R1, LOW);
    while(M2C <= S2P){
      // Do Nothing While Other One Finishes
    }
    delay(50);
    digitalWrite(M2R1, LOW);
  }
  else { // (M2C >= 10){
    delay(50);
    digitalWrite(M2R1, LOW);
    while(M1C <= W1P){
      // Do Nothing While Other One Finishes
    }
    delay(50);
    digitalWrite(M1R1, LOW);
  } */
}

/*
---------------------------------------------------------------------------------
*/

void halleffect1(){
  if(millis() - HElastchange1 > HEdebounceDuration){
      HElastchange1 = millis();
      M1C++;
      if(LEDstate1 == HIGH){
        LEDstate1 = LOW;
        digitalWrite(extendLED, LOW);   // Turns LED On and Off as Drive Wheels Rotate
      } 
      else{
        LEDstate1 = HIGH;
        digitalWrite(extendLED, HIGH);
      } 
  }
}

/*
---------------------------------------------------------------------------------
*/

void halleffect2(){
  if(millis() - HElastchange2 > HEdebounceDuration){
      HElastchange2 = millis();
      M2C++;
      if(LEDstate2 == HIGH){
        LEDstate2 = LOW;
        digitalWrite(retractLED, LOW);   // Turns LED On and Off as Drive Wheels Rotate
      } 
      else{
        LEDstate2 = HIGH;
        digitalWrite(retractLED, HIGH);
      } 
  }
}

 /* M2C++;
  if(LEDstate2 == HIGH){
    LEDstate2 = LOW;
    digitalWrite(retractLED, LOW);   // Turns LED On and Off as Spool Rotates
  } 
  else{
    LEDstate2 = HIGH;
    digitalWrite(retractLED, HIGH);
  }
}*/

/*
---------------------------------------------------------------------------------
*/

void Night(){
  solarV = analogRead(A1);
  if(solarV < 5){
    digitalWrite(fan1, LOW);
  }
}

/*
---------------------------------------------------------------------------------
*/

void diagnotic(){
  batteryV = analogRead(A0);
  solarV = analogRead(A1);

  Serial.println(batteryV);
  Serial.println(solarV);
}