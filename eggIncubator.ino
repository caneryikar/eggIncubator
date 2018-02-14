
// include the library code:
#include <LiquidCrystal.h>
#include "DHT.h"
#include <Servo.h>

// set the DHT Pin
#define DHTPIN 8
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

double counter = 0;
int heaterBulbPin = 13;
int servoPin = 9;
Servo servo;  
int servoAngle = 0;
int direction = 1;
int servoRotationSecond = 300;
float celciusLimit = 38.5;


void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  dht.begin();
  
  // Print a message to the LCD.
  lcd.print("Temp:  Humidity:");

  pinMode(heaterBulbPin, OUTPUT);
  Serial.begin(19200);
  servo.attach(servoPin);


}


void loop() {
  delay(1000);
  counter++;
  //digitalWrite(heaterBulbPin, HIGH); // relay test


  if(counter==servoRotationSecond){
   runServo(direction); 
   }
   
    
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // read humidity
  float h = dht.readHumidity();
  //read temperature in Fahrenheit
  float c = dht.readTemperature(false);
//  float c = (f-32.00)/1.8;
Serial.println(c);

  if (isnan(h) || isnan(c)) {
    lcd.print("ERROR");
    return;
  }

  lcd.print(c);
  lcd.setCursor(7,1);
  lcd.print(h);  
  if (c<celciusLimit){
  lcd.setCursor(15,1);
  lcd.print("Y");
  }
  else{
  lcd.setCursor(15,1);
  lcd.print("N");
    }

//light relay control
if (c>celciusLimit)
  {
    digitalWrite(heaterBulbPin, LOW);
    Serial.println("heaterBulb turned off");
     //Serial.println(incomingByte, DEC);
    }
  else
  {
    digitalWrite(heaterBulbPin, HIGH);
     Serial.println("heaterBulb turned on");
     //Serial.println(incomingByte, DEC);
    }
    
}

void runServo(int dir){
  if (dir==1)
  {// turn right
Serial.println("servo rotated to left");
 servo.write(0);      // Turn SG90 servo Left to 45 degrees
  direction = -1;
    }
   else{
  //turn left
 Serial.println("servo rotated to right");
   servo.write(90);     // Turn SG90 servo Right to 135 degrees
   direction = 1;
    }
    counter = 0;
}

