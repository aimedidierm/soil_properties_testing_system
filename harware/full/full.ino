#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


#define RE 8
#define DE 7
const int buttonPin = 4;
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

int buttonState = 0,k=0;
byte values[11];
SoftwareSerial mod(2,3);
String data="";
void setup() {
  Serial.begin(9600);
  mod.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  pinMode(buttonPin, INPUT);
  lcd.setCursor(0,0);
  lcd.print("Soil properties");
  lcd.setCursor(0,1);
  lcd.print("testing system");
  delay(4000);
}
 
void loop() {
  byte val1,val2,val3;
  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press button");
  lcd.setCursor(0,1);
  lcd.print("to correct values");
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("N:");
    lcd.print(val1);
    lcd.print(" ");
    lcd.print("P:");
    lcd.print(val2);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print("K:");
    lcd.print(val3);
    lcd.print(" ");
    delay(1000);
    Serial.print((String)"n="+val1+"&p="+val2+"&k="+val3);
    while(k==0){
      if (Serial.available() > 0) {
        data = Serial.readStringUntil('\n');
      Serial.println(data);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(data);
      if (root["c"]) {
      int cstatus = root["c"];
      int perc = root["percentage"];
      if(cstatus==1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("NPK value:");
        lcd.setCursor(0, 1);
        lcd.print("is high");
        delay(3000);
        } else if (cstatus==2){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("NPK value:");
          lcd.setCursor(0, 1);
          lcd.print("is enouth");
          delay(3000);
          } else if (cstatus==3){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("NPK value:");
          lcd.setCursor(0, 1);
          lcd.print("is low");
          delay(3000);
          }
      }
      }
      }
  }
}
 
byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
