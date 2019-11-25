#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial nss(6, 7);
const int  buttonPin = 9;
TinyGPS gps;
int tempPin = 0;

static void gpsdump(TinyGPS &gps);
static bool feedgps();

int temp = 0;
float maxSpeed = 0;
int buttonPushCounter = 0;   
int buttonState = 0;         
int lastButtonState = 0;

void setup()
{
  pinMode(buttonPin, INPUT);
  nss.begin(9600);
  lcd.begin(16, 2);
  lcd.print("SKI-HUD");
  lcd.setCursor(0, 1);
  lcd.print("Ver. 0.4");
  delay(500);
  lcd.clear();

  bool newdata = false;

  
    if (newdata == false){
      lcd.setCursor(0, 0);
    lcd.print("Suche");
    lcd.setCursor(0, 1);
    lcd.print("Satelliten...");
  }
}



void loop()
{
 bool newdata = false;
  unsigned long start = millis();

  while (millis() - start < 200)
  
    if (feedgps()){
      newdata = true;
  }

  if (newdata){   
    gpsdump(gps);
  }
   
    buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
       buttonPushCounter ++;
      }
    }
    
lastButtonState = buttonState;  
 if (buttonPushCounter > 5) {
     buttonPushCounter = 0;}
} 



static void gpsdump(TinyGPS &gps) {

  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minutes, second, hundredths;

  float fkmph = gps.f_speed_kmph();

  if ((gps.f_speed_kmph() > maxSpeed) && (gps.f_speed_kmph() < 1000)){
     maxSpeed = gps.f_speed_kmph(); 
}
    
    temp = ( 5.0 * analogRead(tempPin) * 1000.0) / 1024.0; 
  
   if(buttonPushCounter == 0){
    feedgps();
    gps.crack_datetime(&year, &month, &day, &hour, &minutes, &second, &hundredths, &age);
    lcd.clear();
    lcd.print("Zeit:");
    lcd.setCursor(0,1);
    lcd.print(static_cast<int>(hour + 1));
    lcd.print(":");
    if (static_cast<int>(minutes) < 9) {
    lcd.print("0");
    lcd.print(static_cast<int>(minutes));
     }
     else {
     lcd.print(static_cast<int>(minutes));
     }
  }
  else if (buttonPushCounter == 1){
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("Temperatur:");
  lcd.setCursor(0, 1); 
  lcd.print(temp/10.0, 1); 
  lcd.write(0xD0 + 15); lcd.print("C"); 
}

else if (buttonPushCounter == 2) {
  feedgps();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Geschwindigkeit: "); 
  lcd.setCursor(0, 1);
  lcd.print(fkmph, 1); 
  lcd.print(" km/h");
}

else if (buttonPushCounter == 3) {
  feedgps();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kurs:");
  lcd.setCursor(0, 1); 
  lcd.print(TinyGPS::cardinal(gps.f_course())); 
}

else if (buttonPushCounter == 4) {
  feedgps();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hoehe:");
  lcd.setCursor(0, 1);
  lcd.print(gps.f_altitude(), 1); 
  lcd.print(" m");
}  

else if (buttonPushCounter == 5) {
    feedgps();    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Max. Tempo:");
    lcd.setCursor(0, 1);
    lcd.print(maxSpeed, 1);
    lcd.print(" km/h");
    } 
}


static bool feedgps()
{
  while (nss.available())
  {
    if (gps.encode(nss.read()))
      return true;
  }
  return false;
  
}


