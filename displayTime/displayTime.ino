#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define L_IR 10      // left infrared sensor
#define R_IR 11      // right infrared sensor

/*-------definning Outputs------*/
#define LF 3         // left motor forward
#define LB 2         // left motor backward
#define RF 9         // right motor forward
#define RB 4         // right motor backward

LiquidCrystal_I2C lcd(0x27, 16, 2);

long timer = 0;
boolean startTimer = true;
boolean endTimer = false;
boolean start1 = false;
long diff;
float speedInMS;
int distance1 = 3;
long timer1 = 0;

void setup() {
  pinMode(LF, OUTPUT);   //left motors forward
  pinMode(LB, OUTPUT);   //left motors reverse
  pinMode(RF, OUTPUT);   //right motors forward
  pinMode(RB, OUTPUT);   //right motors reverse

  pinMode(L_IR, INPUT);
  pinMode(R_IR, INPUT);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // tape is LOW
  // geen tape, dus grond is HIGH
  if(startTimer) {
    moveWheels(120, 0, 120, 0);
    timer = millis();
    startTimer = false;
  }
  if(!endTimer && digitalRead(L_IR) && digitalRead(R_IR)) {
    // diff is de tijd die de robot doet over één meter, in seconden
    diff = (millis() - timer) / 1000;
    timer = 0;
    endTimer = true;
    lcd.print("Tape spotted!");
    start1 = true;
    timer1 = millis();
//    speedInMS = 1 / timeToDrive;
  }
  long timeToDrive1 = (distance1 - 1) * (diff * 1000);
  lcd.setCursor(0, 1);
  lcd.print(timeToDrive1);
  if(start1) {
    if(millis() - timer1 > timeToDrive1) {
      moveWheels(0, 0, 0, 0);
    }
  }
}

void moveWheels(int leftForwardSpeed, int leftBackwardSpeed, int rightForwardSpeed, int rightBackwardSpeed) {
  analogWrite(LF, leftForwardSpeed);
  analogWrite(LB, leftBackwardSpeed);
  analogWrite(RF, rightForwardSpeed);
  analogWrite(RB, rightBackwardSpeed);
}
