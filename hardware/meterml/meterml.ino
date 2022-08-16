#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int sensorInterrupt = 1;
int sensorPin       = 3;
int solenoidValve = 5;

LiquidCrystal_I2C lcd(0x27,20,4);
unsigned int SetPoint = 400; //400 milileter
float calibrationFactor = 90; 
volatile byte pulseCount =0;  
float flowRate = 0.0;
unsigned int flowMilliLitres =0;
unsigned long totalMilliLitres = 270;
unsigned long oldTime = 0;
 
void setup()
{
  lcd.init(); 
  lcd.init();
  lcd.backlight();
  pinMode(solenoidValve , OUTPUT);
  digitalWrite(solenoidValve, HIGH);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
 
  /*The Hall-effect sensor is connected to pin 2 which uses interrupt 0. Configured to trigger on a FALLING state change (transition from HIGH
  (state to LOW state)*/
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING); //you can use Rising or Falling
}
 
void loop()
{
 
   if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to the host
    detachInterrupt(sensorInterrupt);
 
    // Because this loop may not complete in exactly 1 second intervals we calculate the number of milliseconds that have passed since the last execution and use that to scale the output. We also apply the calibrationFactor to scale the output based on the number of pulses per second per units of measure (litres/minute in this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
 
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
 
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
 
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres -= flowMilliLitres;
 
    unsigned int frac;
    lcd.clear();
    lcd.setCursor(0,0);
    // Print the flow rate for this second in litres / minute
    lcd.print("Flow rate: ");
    lcd.print(flowMilliLitres, DEC);  // Print the integer part of the variable
    lcd.print("mL/Sd");
    // Print the cumulative total of litres flowed since starting
    lcd.setCursor(0,1);
    lcd.print("L Quantity: ");        
    lcd.print(totalMilliLitres);
    lcd.print("mL");    
        
    if (totalMilliLitres <= 20)
    {
      SetSolinoidValve();
    }
    
// Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
 
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}
 
//Insterrupt Service Routine
 
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
 
void SetSolinoidValve()
{
  digitalWrite(solenoidValve, LOW);
}
