#include <Stepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// initialize the steppers and steps per revolution constants
const int spr30degrees = 256;
const int spr90degrees = 553;
const int spr180degrees = 1106;

const int rolePerMinute = 10;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm


Stepper stepper1(spr180degrees, 6, 5, 4, 3);
Stepper stepper2(spr180degrees, 8, 9, 10, 11);
Stepper stepper3(spr180degrees, 14, 15, 16, 17);
Stepper stepper4(spr180degrees, 31, 33, 35, 37);
Stepper stepper5(spr180degrees, 39, 41, 43, 45);
Stepper stepper6(spr180degrees, 47, 49, 51, 53);

int redPin=7, greenPin=2, bluePin=12;
int OffRGB=0, OnRGB=255;
int buzzerPin=13;
int buttonPin=1;

//Continuously read the value of the switch
int switchValue = analogRead(A2);

bool powerState = 0;

void setup() {
  //Set up pin modes for LED & Buzzer
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(A2, INPUT_PULLUP);

  //Turn off the RGB Light
  digitalWrite(redPin, OffRGB);
  digitalWrite(greenPin, OffRGB);
  digitalWrite(bluePin, OffRGB);

  //Set speed on steppers
  stepper1.setSpeed(rolePerMinute);
  stepper2.setSpeed(rolePerMinute);
  stepper3.setSpeed(rolePerMinute);
  stepper4.setSpeed(rolePerMinute);
  stepper5.setSpeed(rolePerMinute);
  stepper6.setSpeed(rolePerMinute);

  //Initialize the serial port:
  Serial.begin(9600);
   
  //Initialize the LCD and turn on the blacklight 
  lcd.begin();
  lcd.backlight();

  //Testing LED & Buzzer 
  digitalWrite(bluePin, OnRGB);
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(bluePin, OffRGB);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  //???
  char inputVariable = Serial.read();

  int switchValue = analogRead(A2);

  if (switchValue < 500) { // you can adjust the threshold value according to your needs
    Serial.println("Switch is closed");
    Serial.println(switchValue);
    //lcd.print("Obstruction Detected");    
}
  else {
    Serial.println("Switch is open");
    Serial.println(switchValue);
  }

  delay(100);

  if(digitalRead(buttonPin) == 0) {
    power();
    delay(500);
    lcd.clear();
  }  
}

//Based on powerState, table extends or retracts
void power(){
  if(powerState == 0) {
    powerState = 1;

    //Stepper extends, buzzer buzzes, Green LED lights
    Serial.println("Steppers Extending...");

    tone(buzzerPin, 100);
    digitalWrite(greenPin, OnRGB);

    //Steppers 1 & 2 rotate clockwise 30 degrees
    for(int numSteps=0; numSteps<spr30degrees; numSteps++)
    {
      //Continuously read the value of the switch
      switchValue = analogRead(A2);

      if(switchValue > 500) {
        tone(buzzerPin, 50);        
        stepper1.step(1);
        stepper2.step(-1); 
      } 
      else {
        //no step
        numSteps -= 1;
        obstructionDetection();
      }
    }

    //Steppers 3 & 4 rotate counterclockwise 180 degrees
    for(int numSteps=0; numSteps<spr180degrees; numSteps++)
    {
      //Continuously read the value of the switch
      switchValue = analogRead(A2);

      if(switchValue > 500) {
        tone(buzzerPin, 50);        
        stepper3.step(-1);
        stepper4.step(1); 
      } 
      else {
        //no step
        numSteps -= 1;
        obstructionDetection();
      }
    }

    //Steppers 5 & 6 rotate clockwise 90 degrees
    for(int numSteps=0; numSteps<spr90degrees; numSteps++)
    {
      //Continuously read the value of the switch
      switchValue = analogRead(A2);

      if(switchValue > 500) {
        tone(buzzerPin, 50);        
        stepper5.step(1);
        stepper6.step(-1); 
      } 
      else {
        //no step
        numSteps -= 1;
        obstructionDetection();
      }
    }

    //Steppers 1 & 2 rotate counterclockwise 30 degrees
    for(int numSteps=0; numSteps<spr30degrees; numSteps++)
    {
      //Continuously read the value of the switch
      switchValue = analogRead(A2);

      if(switchValue > 500) {
        tone(buzzerPin, 50);        
        stepper1.step(1);
        stepper2.step(-1); 
      } 
      else {
        //no step
        numSteps -= 1;
        obstructionDetection();
      }
    }
    
    //Buzzer and Green LED stop
    noTone(buzzerPin);
    digitalWrite(greenPin, OffRGB);

    delay(1000);
    Serial.println("Steppers Extension Complete.");
    lcd.print("Desk Successfully Extended");    
  }
  else {
    powerState = 0;

    //Stepper retract, buzzer buzzes, Red LED lights
    Serial.println("Steppers Retracting..");
  
    tone(buzzerPin, 50);
    digitalWrite(redPin, OnRGB);

    //Steppers 1 & 2 rotate clockwise 30 degrees
    for(int numSteps=0; numSteps<spr30degrees; numSteps++)
    {
      //Continuously read the value of the switch
      switchValue = analogRead(A2);

      if(switchValue > 500) {
        tone(buzzerPin, 50);        
        stepper1.step(1);
        stepper2.step(-1); 
      } 
      else {
        //no step
        numSteps -= 1;
        obstructionDetection();
      }
    }

    //Steppers 5 & 6 rotate counterclockwise 90 degrees
    for(int numSteps=0; numSteps<spr90degrees; numSteps++)
    {
      //Continuously read the value of the switch
      switchValue = analogRead(A2);

      if(switchValue > 500) {
        tone(buzzerPin, 50);        
        stepper5.step(-1);
        stepper6.step(1); 
      } 
      else {
        //no step
        numSteps -= 1;
        obstructionDetection();
      }
    }


    //Steppers 3 & 4 rotate clockwise 180 degrees
    for(int numSteps=0; numSteps<spr180degrees; numSteps++)
    {
      //Continuously read the value of the switch
      switchValue = analogRead(A2);

      if(switchValue > 500) {
        tone(buzzerPin, 50);        
        stepper3.step(1);
        stepper4.step(-1); 
      } 
      else {
        //no step
        numSteps -= 1;
        obstructionDetection();
      }
    }

    //Steppers 1 & 2 rotate counterclockwise 30 degrees
    for(int numSteps=0; numSteps<spr30degrees; numSteps++)
    {
      //Continuously read the value of the switch
      switchValue = analogRead(A2);

      if(switchValue > 500) {
        tone(buzzerPin, 50);        
        stepper1.step(-1);
        stepper2.step(1); 
      } 
      else {
        //no step
        numSteps -= 1;
        obstructionDetection();
      }
    }
    
    //Buzzer and Red LED stop
    noTone(buzzerPin);
    digitalWrite(redPin, OffRGB);

    delay(1000);
    Serial.println("Steppers Retraction Complete.");
    lcd.print("Desk Successfully Retracted");    
  }
}

void obstructionDetection(){
  tone(buzzerPin, 500);
  lcd.print("Obstruction Detected"); 
  delay(1000);
  lcd.clear();
}
 
