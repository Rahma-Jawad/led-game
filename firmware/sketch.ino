// Arduino Blink Example
#include <Wire.h>                
#include <LiquidCrystal_I2C.h>
#include <Ds1302.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
Ds1302 rtc(7,6,5);  
const int mode = 2;
const int startStopButton = 3;

bool stopwatchMode = false;
bool running = false;
unsigned long start = 0;
unsigned long end = 0;

void setup() {
  pinMode(mode, INPUT_PULLUP);
  pinMode(startStopButton, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  rtc.init();
}

void loop() {
  // Read buttons
  if (digitalRead(mode) == LOW) {
    stopwatchMode = !stopwatchMode;
    delay(200); // debounce
  }
  if (digitalRead(startStopButton) == LOW) {
    if (stopwatchMode) {
      if (!running) {
        running = true;
        start = millis() - end; 
      } 
      else {
        running = false;
        end = millis() - start; 
        }
    }
    delay(200); 
  }

  lcd.clear();
  if (!stopwatchMode) {
 
    Ds1302::DateTime now;
    rtc.getDateTime(&now);
    lcd.setCursor(0,0);
    lcd.print("Time: ");
    lcd.print(now.hour); lcd.print(":");
    lcd.print(now.minute); lcd.print(":");
    lcd.print(now.second);
    lcd.setCursor(0,1);
    lcd.print("Date: ");
    lcd.print(now.day); lcd.print("/");
    lcd.print(now.month); lcd.print("/");
    lcd.print(now.year);
  } else {
    // Stopwatch mode
    if (running) end = millis() - start;
    unsigned long seconds = end / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    seconds %= 60; minutes %= 60;
    lcd.setCursor(0,0);
    lcd.print("Stopwatch:");
    lcd.setCursor(0,1);
    lcd.print(hours); lcd.print(":");
    lcd.print(minutes); lcd.print(":");
    lcd.print(seconds);
  }
  delay(100);
}
