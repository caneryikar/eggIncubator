// include the library code:
#include <LiquidCrystal.h>
#include <dht.h>
#include <Servo.h>
#include "WiFiEsp.h"

// set the DHT Pin
#define DHT11_PIN 8
dht DHT;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Emulate Serial1 on pins 7/6 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

//char ssid[] = "TivibuIPTV";     // your network SSID (name)
//char pwd[] = "iptv35izmir";  // your network password


char ssid[] = "ouysal";     // your network SSID (name)
char pwd[] = "fdu0rr35";  // your network password


// Initialize the Wifi client library
WiFiEspClient client;

double counter = 0;
int heaterBulbPin = 13;
int servoPin = 9;
Servo servo;  

int servoAngle = 0;   // servo position in degrees

int direction = 1;

//60 equals to 1min
int servoRotationSecond = 60;
float celciusLimit = 36.5;
int wifiCounter = 0;


int port = 8081;

boolean wifiStatus = false;


void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  // Print a message to the LCD.
  lcd.print("Temp:  Humidity:");

  pinMode(heaterBulbPin, OUTPUT);
  Serial.begin(19200);
  servo.attach(servoPin);
  servo.write(0);

    if(wifiStatus){
      Serial1.begin(9600);  
      WiFi.init(&Serial1);
    }
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

  int chk = DHT.read11(DHT11_PIN);
  if (chk == -2) {
    return;
  }
  
  // read humidity
  float h = DHT.humidity;
  //read temperature in Fahrenheit
  float c = DHT.temperature;
//  float c = (f-32.00)/1.8;
Serial.println(c);

if(wifiStatus){
  if (++wifiCounter == 5) {
     sendData(c);
     wifiCounter = 0;
  }
  }
  

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
 //servo.write(0);      

 for(servoAngle = 0; servoAngle < 90; servoAngle++)  
  {                                  
    servo.write(servoAngle);              
    delay(50);                  
  }
  direction = -1;
    }
   else{
  //turn left
 Serial.println("servo rotated to right");
   //servo.write(90);     

   for(servoAngle = 90; servoAngle > 0; servoAngle--)  
  {                                  
    servo.write(servoAngle);              
    delay(50);                  
  }
  
   direction = 1;
    }

    
    counter = 0;
}

// TODO: refactor me
void sendData(int celcius) {

  bool f;
  int c;

  assertEquals("Check status WL_DISCONNECTED", WiFi.status(), WL_DISCONNECTED);
  
  assertEquals("Connect", WiFi.begin(ssid, pwd), WL_CONNECTED);
  
  assertEquals("Check status WL_CONNECTED", WiFi.status(), WL_CONNECTED);
  
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  assertEquals("Ping", WiFi.ping("95.6.100.21"), true);
  
  assertEquals("Not connected", client.connected(), false);
  assertEquals("Connect to server", client.connect("95.6.100.21", port), 1);
  assertEquals("Connected", client.connected(), true);


  //--------------------------------------------------------------
  // HTTP request without 'Connection: close' command

  char buf[50];
  sprintf(buf, "GET /sensors/temperature/%d HTTP/1.1", celcius); // puts string into buffer
  Serial.print("Command:");
  Serial.println(buf);
  
  client.println(buf);
  client.println("Host: 95.6.100.21");
  client.println("Connection: close");
  client.println();
}

void assertEquals(const char* test, int actual, int expected)
{
  if(actual==expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, char* actual, char* expected)
{
  if(strcmp(actual, expected) == 0)
    pass(test);
  else
    fail(test, actual, expected);
}


void pass(const char* test)
{
  Serial.print(F("********************************************** "));
  Serial.print(test);
  Serial.println(" > PASSED");
  Serial.println();
}

void fail(const char* test, char* actual, char* expected)
{
  Serial.print(F("********************************************** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=\"");
  Serial.print(actual);
  Serial.print("\", expected=\"");
  Serial.print(expected);
  Serial.println("\")");
  Serial.println();
  delay(10000);
}

void fail(const char* test, int actual, int expected)
{
  Serial.print(F("********************************************** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=");
  Serial.print(actual);
  Serial.print(", expected=");
  Serial.print(expected);
  Serial.println(")");
  Serial.println();
  delay(10000);
}

