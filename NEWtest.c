/*
Rapid Deployment Runway Closure System C Code
By: Marc Tanwangco, Jomari Paguia, Nicholas Wolford
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
int M1C;  // Motor 1 Counter
int M2C;  // Motor 2 Counter
int MF1 = 0;  // Motor Flag 1
int MF2 = 0;  // Motor Flag 2

// BUTTONS & LEDs
const int extendbutton = A5;  // Extend Button
const int retractbutton = A4; // Retract Button
int buttonstate1 = digitalRead(extendbutton);
int buttonstate2 = digitalRead(retractbutton);
int laststate1 = LOW; // Last Button 1 State
int laststate2 = LOW; // Last Button 2 State
const int retractLED = 5; // Retract LED
const int extendLED = 6;  // Extend LED
int LEDstate1 = LOW;  // LED 1 State
int LEDstate2 = LOW;  // LED 2 Staate
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
int HECounter = 0;  // Hall Effect Counter

float solarV;
float batteryV;

/*
---------------------------------------------------------------------------------
*/
 
void setup() {
  // put your setup code here, to run once:
  pinMode (extendbutton, INPUT);
  pinMode (retractbutton, INPUT);

  pinMode(extendLED, OUTPUT);
  pinMode(retractLED, OUTPUT);
  
  pinMode (fan1, OUTPUT);    
  pinMode (fan2, OUTPUT);     
  pinMode (fan3, OUTPUT);      
  pinMode (fan4, OUTPUT);      
  pinMode (fan5, OUTPUT);     

  pinMode (M1R1, OUTPUT);      //motor 1 relay output 1 (DRIVE WHEELS)
  pinMode (M1R2, OUTPUT);      //motor 1 relay output 2 (DRIVE WHEELS)
  pinMode (M2R1, OUTPUT);      //motor 2 relay output 1 (SPOOL)
  pinMode (M2R2, OUTPUT);      //motor 2 relay output 2 (SPOOL)

  digitalWrite (M1R1, LOW);    //make sure both relays are off
  digitalWrite (M1R2, LOW);
  digitalWrite (M2R1, LOW);
  digitalWrite (M2R2, LOW);
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
        } else{
          LEDstate1 = HIGH;
        }
        digitalWrite(extendLED, LEDstate1);
        extend();
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
        } else{
          LEDstate2 = HIGH;
        }
        digitalWrite(retractLED, LEDstate2);
        retract();
      }
    }
  }
}

/*
---------------------------------------------------------------------------------
*/
 
void extend(){
  ERF = 1;

  for(M1C = 0; M1C <= 180; M1C++){
    digitalWrite(M1R1, HIGH);
  }

  for(M2C = 0; M2C <= 180; M2C++){
    digitalWrite(M2R1, HIGH);
  }

  if(M1C == 180 && M2C == 180){
    digitalWrite(M1R1, LOW);
    digitalWrite(M2R1, LOW);
  }

  inflate();
  diagnostic();
}

/*
---------------------------------------------------------------------------------
*/

void inflate(){
  buttonstate1 = digitalRead(extendbutton);
  
  digitalWrite(fan1, HIGH);
  digitalWrite(fan3, HIGH);
  digitalWrite(fan5, HIGH);

  FF1 = 0;
  FF3 = 0;
  FF5 = 0;

  delay(1000);

  if(buttonstate1 == HIGH){
    digitalWrite(fan1, HIGH);
    digitalWrite(fan3, LOW);
    digitalWrite(fan5, LOW);

    FF3 = 0;
    FF5 = 0;
  }
  else if(buttonstate1 == LOW){
    digitalWrite(fan1, LOW);
    digitalWrite(fan3, LOW);
    digitalWrite(fan5, LOW);

    FF1 = 0;
    FF3 = 0;
    FF5 = 0;
  }

  diagnostic();
}

/*
---------------------------------------------------------------------------------
*/

void retract(){
  buttonstate2 = digitalRead(retractbutton);
  ERF = 0;
  ERSF = 0;

  digitalWrite(A3, HIGH);
  digitalWrite(8, HIGH);

  FF2 = 1;
  FF4 = 1;

  delay(1000);

  if(buttonstate2 == HIGH){
    digitalWrite(fan2, HIGH);
    digitalWrite(fan4, HIGH);

    delay(121000);

    digitalWrite(fan2, LOW);
    digitalWrite(fan4, LOW);

    digitalWrite(M1R2, HIGH);
    digitalWrite(M2R2, HIGH);

    delay(120000);

    if(M1C == 180 && M2C == 180){
      digitalWrite(M1R2, LOW);
      digitalWrite(M2R2, LOW);
    }
  }
  else if(buttonstate2 == LOW){
    digitalWrite(fan2, LOW);
    digitalWrite(fan4, LOW);
    digitalWrite(M1R2, LOW);
    digitalWrite(M2R2, LOW);
  }

  diagnostic();
}

/*
---------------------------------------------------------------------------------
*/
 
void diagnostic(){
  
}
