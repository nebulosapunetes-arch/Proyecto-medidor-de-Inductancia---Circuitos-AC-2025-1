#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Capacitor
#define cargaCap     5
#define descargaCap  4
#define mideCap      A0

//Inductor
#define impulse     6
#define time       11

// Capacitor
const float k = 0.0098152; // 1e06/R
const int VrefADC = 648; // 63.2% = 648/1023

//Inductor
float C = 0.0000004785;
float pi = 3.141592;

float capMeter() {
  digitalWrite(cargaCap, HIGH);
  unsigned long t0 = micros();
  while (analogRead(mideCap) < VrefADC);
  unsigned long deltaT = micros() - t0;
  digitalWrite(cargaCap, LOW);

  // Descargar
  pinMode(descargaCap, OUTPUT);
  digitalWrite(descargaCap, LOW);
  while (analogRead(mideCap) > 10);
  pinMode(descargaCap, INPUT);

  // Cálculo
  return deltaT * k - 1.1;
}

float indMeter() {

  digitalWrite(impulse,HIGH);
  delay(10);
  digitalWrite(impulse,LOW);

  unsigned long t = pulseIn(time, LOW,50000);

  if (t == 0) return 0;

  float f = 1000000 / (2*t);

  return (1 / (4 * pi * pi * f * f * C))*1000000;
}

void printCapacitance(float value) {
  lcd.setCursor(0, 1);
  lcd.print("Cx =        "); // Borra línea
  lcd.setCursor(5, 1);

  if (value < 1000.0) {
    lcd.print(value, 1);
    lcd.setCursor(14, 1);
    lcd.print("nF");
   } else {
    lcd.print(value * 0.001, 1);
    lcd.setCursor(14, 1);
    lcd.print("uF");
  }
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

  // Capacitor
  pinMode(descargaCap, INPUT);
  pinMode(cargaCap, OUTPUT);
  digitalWrite(cargaCap, LOW);

  // Inductor
  pinMode(impulse, OUTPUT);
  pinMode(time, INPUT);

}

void loop() {

  float ind = indMeter();
  printInductance(ind);

  float cap = capMeter();
  printCapacitance(cap);

  delay(700);

}