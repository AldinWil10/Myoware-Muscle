
//Define LCD
#define BLYNK_TEMPLATE_ID " "
#define BLYNK_DEVICE_NAME "Test"
#define BLYNK_AUTH_TOKEN " "
#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = " ";  // type your wifi name
char pass[] = " ";  // type your wifi password
//Define Sensor

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

int EMG = A0;
float sensorValue,outputValue;
int enA = 5;
int n1 = 7, n2 = 8;
long pwm;
float  EMGvalue;

const byte ISR = 2;
volatile unsigned char count = 0;

void sendSensor()
{  
  Blynk.virtualWrite(V1, sensorValue);
  Blynk.virtualWrite(V2, sensorValue);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin();
  pinMode(n1, OUTPUT);
  pinMode(n2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(ISR, INPUT_PULLUP);
  attachInterrupt(INT0, InterruptDigital, RISING);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  //Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);
  
  pinMode(EMG,INPUT);
  pinMode(n1, OUTPUT);
  pinMode(n2, OUTPUT);
  pinMode(enA, OUTPUT);
}

void InterruptDigital()
{
  //Serial.println("Count = "); 
  count++;

  if(count == 0){
    Serial.println("Manual Mode");
    }
  if(count == 1){
    Serial.println("Sensor Mode");
    }
    
  if(count == 2){
    Serial.println("Manual Mode");
    count = 0;}
}

void Sensor(){
  //Serial.print(distance);Serial.println(" cm");
  //Serial.println(pwm);
  
   if (EMGvalue>300){
    digitalWrite(n1, HIGH);
    digitalWrite(n2, LOW);
    analogWrite(enA, 255);
  }
  else{
    digitalWrite(n1, HIGH);
    digitalWrite(n2, LOW);
    analogWrite(enA, 0);
  }  
}
  
void Potensio(){
  //Serial.println(outputValue);
  
  analogWrite(enA, outputValue);
  digitalWrite(n1, HIGH);
  digitalWrite(n2, LOW);
}

void loop() {
  EMGvalue = analogRead(A0);
    
  //Baca Potensiometer
  sensorValue = analogRead(A1);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  Blynk.run();
  sendSensor();
  delay(1000);

  //Main Program
  if (count==0){
    Potensio();
    //Serial.println(outputValue);
    lcd.setCursor(0,0);
    lcd.print("MANUAL MODE");
    lcd.setCursor(0,1);
    lcd.print("PWMvalue = ");lcd.print(outputValue);
  }
  else if(count==1){
    Sensor();
    //Serial.print(distance);Serial.println(" cm");
    //Serial.println("PWMvalue = ");Serial.println(pwm);
    lcd.setCursor(0,0);
    lcd.print("SENSOR MODE");
    lcd.setCursor(0,1);
    lcd.print("EMGvalue = ");lcd.print(EMGvalue);
    Serial.println(EMGvalue);
    }
  
}
