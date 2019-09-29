#include <TroykaGPS.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#include <AmperkaKB.h>

AmperkaKB KB(12, 11, 10, 9, 8);

GPS gps(Serial1);

char time[24];
char latitude[24];
char longitude[24];
char speed[24];
char altitude[24];
char date[24];

enum State
{
    Coordinates,
    DateTime,
    Speed,
    Sat
};

State state;
void setup()
{
  state = Coordinates; 
  Serial.begin(115200);
  Serial.print("Serial init OK\r\n");
  Serial1.begin(115200);
  KB.begin(KB1x4);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, Kirill!");
  lcd.setCursor(0,1);
  lcd.print("AmperkaGPSNavia!");
  delay(3000);
}
 
void loop()
{
  // если пришли данные с gps-модуля
  if (gps.available()) {
    // считываем данные и парсим
    gps.readParsing();
    // проверяем состояние GPS-модуля
    switch(gps.getState()) {
      // всё OK
      case GPS_OK:
          char dispLatitude[24];
  dispLatitude[0] = latitude[0];
  dispLatitude[1] = latitude[1];
  if (KB.onPress()) {
    if(KB.getNum == 1) {
      state = Coordinates;
    } else if(KB.getNum == 2){
      state = DateTime;
    } else if(KB.getNum == 3){
      state = Speed;
    } else if(KB.getNum == 4){
      state = Sat;
    }
  }
  if (state == Coordinates) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(gps.getLatitudeBase10(), 6);
    
    lcd.setCursor(0,1);
    lcd.print(gps.getLongitudeBase10(), 6);
    
  } else if (state == DateTime) {
    lcd.clear();
    gps.getTime(time, 24);
    gps.getDate(date, 24);
    lcd.setCursor(0,0);
    lcd.print("Date: ");
    lcd.print(date);
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    lcd.print(time);
  } else if (state == Speed) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Speed: ");
    lcd.print(gps.getSpeedKm());
    lcd.setCursor(0,1);
    lcd.print("Altit: ");
    lcd.print(gps.getAltitude());
  } else if (state == Sat) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sat: ");
    lcd.print(gps.getSat());
    lcd.setCursor(0,1);
    lcd.print("B1:");
    lcd.print(analogRead(A0)/204.8*1.25625);
    lcd.setCursor(8,1);
    lcd.print("B2:");
    lcd.print(analogRead(A1)/204.8*1.25625);
  }
       break;
      // ошибка данных
      case GPS_ERROR_DATA:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("GPS error data");
        lcd.setCursor(0,1);
        lcd.print("B1:");
        lcd.print(analogRead(A0)/204.8*1.25625);
        lcd.setCursor(8,1);
        lcd.print("B2:");
        lcd.print(analogRead(A1)/204.8*1.25625);
        break;
      // нет соединение со спутниками
      case GPS_ERROR_SAT:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("NoConnectToSat!!!");
        lcd.setCursor(0,1);
        lcd.print("B1:");
        lcd.print(analogRead(A0)/204.8*1.25625);
        lcd.setCursor(8,1);
        lcd.print("B2:");
        lcd.print(analogRead(A1)/204.8*1.25625);
        break;
      }
    }
  }
