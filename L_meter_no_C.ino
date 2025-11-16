#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inductor
#define impulse     6
#define time       11

// Inductor
float C = 0.0000004785;
float pi = 3.141592;

float indMeter() {
  digitalWrite(impulse,HIGH);
  delay(10);
  digitalWrite(impulse,LOW);

  unsigned long t = pulseIn(time, LOW,50000);

  if (t == 0) return 0;

  float f = 1000000 / (2*t);

  return (1 / (4 * pi * pi * f * f * C))*1000000;
}

void printInductance(float value) {
  lcd.setCursor(0,0);
  lcd.print("Lx =          ");
  lcd.setCursor(5,0);

  if (value < 1000) {
    lcd.print(value , 1);
    lcd.setCursor(14,0);
    lcd.print("uH");
  } else {
    lcd.print(0.001 * value , 1);
    lcd.setCursor(14,0);
    lcd.print("mH");
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("    LC METER    ");
  lcd.setCursor(0, 1);
  delay(2000);
  lcd.clear();

  // Inductor
  pinMode(impulse, OUTPUT);
  pinMode(time, INPUT);
}

void loop() {
  float ind = indMeter();
  printInductance(ind);
  delay(700);
}