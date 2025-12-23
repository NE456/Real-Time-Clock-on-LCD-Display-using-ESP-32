#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Try 0x3F if this doesn't work

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);   // ESP32 I2C pins

  // I2C Scanner
  Serial.println("Scanning I2C bus...");
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(address, HEX);
    }
  }

  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    Serial.println("RTC NOT FOUND");
    lcd.setCursor(0, 0);
    lcd.print("RTC NOT FOUND");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, setting time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("RTC initialized successfully");
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();

  // Print to Serial for debugging
  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  // Time format: Hrs:Mins:Sec
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());
  lcd.print(":");
  if (now.second() < 10) lcd.print("0");
  lcd.print(now.second());

  // Date format: DD/MM/YYYY
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  if (now.day() < 10) lcd.print("0");
  lcd.print(now.day());
  lcd.print("/");
  if (now.month() < 10) lcd.print("0");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());

  delay(1000);
}
}
