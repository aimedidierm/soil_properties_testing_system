#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

int juiceInterrupt = 1;
int juicesensorPin       = 3;
int juiceValve = 7;
int waterInterrupt = 0;
int watersensorPin       = 2;
int waterValve = 8;
float calibrationFactor = 90; 
volatile byte pulseCount =0;  
float flowRate = 0.0;
unsigned int flowMilliLitres =0;
unsigned long totalMilliLitres = 270;
unsigned long oldTime = 0;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char newNum[12]="";
//define the cymbols on the buttons of the keypads
char keys[ROWS][COLS] = {

    {'1','2','3','A'},

    {'4','5','6','B'},

    {'7','8','9','C'},

    {'*','0','#','D'}

};

byte rowPins[ROWS] = {A0, A1, A2, A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int drink=0,drinkvolume=0;


void setup() 
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  SPI.begin();  
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(juiceValve , OUTPUT);
  pinMode(juicesensorPin, INPUT);
  digitalWrite(juicesensorPin, HIGH);
  attachInterrupt(juiceInterrupt, pulseCounter, FALLING);
  pinMode(waterValve , OUTPUT);
  pinMode(watersensorPin, INPUT);
  digitalWrite(watersensorPin, HIGH);
  attachInterrupt(waterInterrupt, pulseCounter, FALLING);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Electronic Drink");
  lcd.setCursor(0,1);
  lcd.print("Vending machine");
  delay(2000);

}

void loop() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Select payment");
  lcd.setCursor(0,1);
  lcd.print("Method");
  paymentmethod();
}

void paymentmethod(){
  unsigned int method=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1. MOMO");
  lcd.setCursor(0,1);
  lcd.print("2. Card");
  int key = keypad.getKey();
  if (key=='1') {
    momo();
  }
  if (key=='2') {
    readcard();
  }
  delay(100);
  paymentmethod();
  }

void momo(){
  int i=0,j=0,m=0,x=0,s=0,code=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Code");
  lcd.setCursor(0,1);
  lcd.print("and press #");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter code");
  for(i=2;i>0;i++){
    lcd.setCursor(0,1);
    int key = keypad.getKey();
    if (key!=NO_KEY && key!='#' && key!='*'){
        newNum[j] = key;
        newNum[j+1]='\0';   
        j++;
        lcd.setCursor(0,1);
        lcd.print(newNum);
    }
    if (key=='#'&& j>0)
    {
        j=0;
        m=0;
        code=newNum;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("1. Juice");
        lcd.setCursor(0,1);
        lcd.print("2. Water");
        for( int d=2; d>1;d++){
          int key = keypad.getKey();
          if (key=='1') {
            drink=1;
          }
          if (key=='2') {
            drink=2;
          }
          if(drink==1 || drink==2){
            int k= 0;
            lcd.clear();
            lcd.print("Waiting");
            Serial.println("code=10");
            while(k==0){
              if (Serial.available() > 0) {
              DynamicJsonBuffer jsonBuffer;
              JsonObject& root = jsonBuffer.parseObject(Serial.readStringUntil('\n'));
              int outml = root["outml"];
              if(outml==0){
                } else{
                  drinkvolume=outml;
                  drinkout();
                  }
              }
              }
          }
          delay(100);
          }
        
    }
    else if (key=='*')
    {
        lcd.rightToLeft();
        lcd.print(" ");
        lcd.leftToRight();
        lcd.print(" ");
        lcd.rightToLeft();
        lcd.print(" ");
        lcd.leftToRight();
        j--;
        newNum[x] = 0;
    }
    delay(100);
  }
  }
  
void(* resetFunc) (void) = 0;

void drinkout(){
  if(drink==1){
    digitalWrite(juiceValve, HIGH);
    while(drinkvolume>20){
      if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    detachInterrupt(juiceInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    drinkvolume -= flowMilliLitres;
 
    unsigned int frac;
    lcd.clear();
    lcd.setCursor(0,0);
    // Print the flow rate for this second in litres / minute
    lcd.print("Thank you!");
    lcd.setCursor(0,1);
    lcd.print("Enjoy ^_^");  
// Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
 
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(juiceInterrupt, pulseCounter, FALLING);
  }
      }
    digitalWrite(juiceValve,LOW);
    resetFunc();
    }else{
      digitalWrite(waterValve, HIGH);
    while(drinkvolume>20){
      if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    detachInterrupt(waterInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    drinkvolume -= flowMilliLitres;
 
    unsigned int frac;
    lcd.clear();
    lcd.setCursor(0,0);
    // Print the flow rate for this second in litres / minute
    lcd.print("Thank you!");
    lcd.setCursor(0,1);
    lcd.print("Enjoy ^_^");  
// Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
 
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(waterInterrupt, pulseCounter, FALLING);
  }
      }
    digitalWrite(waterValve,LOW);
    resetFunc();
      }
  delay(3000);
  resetFunc();
  }
  
void readcard(){
  // Look for new cards
  int i=0,j=0,m=0,x=0,s=0,money=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tap card");
  delay(500);
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    readcard();
    //return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    readcard();
    //return;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  String card=content.substring(1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter money");
  for(i=2;i>0;i++){
    int key = keypad.getKey();
    if (key!=NO_KEY && key!='#' && key!='*'){
        newNum[j] = key;
        newNum[j+1]='\0';   
        j++;
        lcd.setCursor(0,1);
        lcd.print(newNum);
    }
    if (key=='#'&& j>0)
    {
        j=0;
        m=0;
        money=newNum;
        int amountpaid=0;
        //kureba niba amafaranga ari kwikarita 
        if(amountpaid==1){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Insufficient ");
          lcd.setCursor(0,1);
          lcd.print("balance");
          delay(2000);
          resetFunc();
          } else{
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("1. Juice");
            lcd.setCursor(0,1);
            lcd.print("2. Water");
            for( int d=2; d>1;d++){
              int key = keypad.getKey();
              if (key=='1') {
                drink=1;
              }
              if (key=='2') {
                drink=2;
              }
              if(drink==1 || drink==2){
                int k= 0;
                lcd.clear();
                lcd.print("Waiting");
                Serial.println("card=10");
                while(k==0){
                  if (Serial.available() > 0) {
                  DynamicJsonBuffer jsonBuffer;
                  JsonObject& root = jsonBuffer.parseObject(Serial.readStringUntil('\n'));
                  int outml = root["outml"];
                  if(outml==0){
                    } else{
                      drinkvolume=outml;
                      drinkout();
                      }
                  }
                  }
              }
              delay(100);
              }
            }
        
    }
    else if (key=='*')
    {
        lcd.rightToLeft();
        lcd.print(" ");
        lcd.leftToRight();
        lcd.print(" ");
        lcd.rightToLeft();
        lcd.print(" ");
        lcd.leftToRight();
        j--;
        newNum[x] = 0;
    }
    delay(100);
  }
}

void pulseCounter()
{
  pulseCount++;
}
