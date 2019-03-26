#include <MPU6050_tockn.h>
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

MPU6050 mpu(Wire);

long timer = 0;
boolean startTimer = true;
boolean endTimer = false;
boolean start1 = false;
boolean firstTime = true;
int timeForOneMeter;
// distance1 en z1 worden uit de dataabase gehaald
int distance1 = 3;
int z1 = 0;
long timer1 = 0;

int modifySpeed = 120;

void setup() {
  pinMode(LF, OUTPUT);   //left motors forward
  pinMode(LB, OUTPUT);   //left motors reverse
  pinMode(RF, OUTPUT);   //right motors forward
  pinMode(RB, OUTPUT);   //right motors reverse

  pinMode(L_IR, INPUT);
  pinMode(R_IR, INPUT);

  Serial.begin(9600);
  mpu.begin();
  mpu.calcGyroOffsets(true);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // beginnen met rijden
  moveWheels(120, 0, 125, 0);
  // huidige tijd vastleggen
  startTimer = millis();
}

void loop() {
  //measureTimeForOneMeter();
  //driveRoute();
}

void moveWheels(int leftForwardSpeed, int leftBackwardSpeed, int rightForwardSpeed, int rightBackwardSpeed) {
  analogWrite(LF, leftForwardSpeed);
  analogWrite(LB, leftBackwardSpeed);
  analogWrite(RF, rightForwardSpeed);
  analogWrite(RB, rightBackwardSpeed);
}

/*
 * Meet de tijd die de bot doet over één meter rijden
 * @return De tijd die de bot over één meter rijden doet in milliseconden
 */
void measureTimeForOneMeter() {
  // tape is HIGH
  // geen tape, dus grond is LOW
  if(!endTimer && digitalRead(L_IR) && digitalRead(R_IR)) {
    // timeForOneMeter is de tijd die de robot doet over één meter, in milliseconden
    timeForOneMeter = (millis() - startTimer);
    // stop huidige loop
    endTimer = true;
    // start volgende loop
    start1 = true;
    lcd.print(timeForOneMeter);
  }
}

/**
 * Deze functie laat de robot een bepaalde afstand afleggen,
 * afhankelijk van een stuke tape op een bepaalde afstand (één meter)
 */
void driveRoute() {
  if(start1) {
    // for each item in the database
    for(int x = 0; x < 1; x++) {
      // de robot in beweging zetten. Het corrigeren komt later
      moveWheels(120, 0, 125, 0);
      // distance1 in meters wordt uit de database gehaald
      // afhankelijk van of de snelheid wordt gemeten of niet
      int tmpDistance;
      if(firstTime) {
        tmpDistance = distance1 - 1;
        firstTime = false;
      } else {
        tmpDistance = distance1;
      }
      // timeToDrive1 is de tijd die de robot moet rijden voor distance1
      // de tijd is in milliseconden
      float timeToDrive1 = (tmpDistance * timeForOneMeter);
      lcd.setCursor(0, 1);
      lcd.print(timeToDrive1);
      // for-loop om robot voor een bepaalde tijd "op te houden",
      // dan rijd de bot voor een bepaalde tijd lang
      for(float t = 0.0; t < timeToDrive1; t++) {
        lcd.setCursor(0, 1);
        lcd.print(t);
        // tijdens het rijden bij laten draaien
        turn(z1);
        delay(1);
      }
      moveWheels(0, 0, 0, 0);
      // ipv de boolean op false te zetten moet nu het volgende item uit de database worden geladen
      start1 = false;
    }
  }
}

/**
 * Deze functie zorgt ervoor dat de robot afhankelijk van
 * de gyroscoop rechtuit blijf rijden
 * @param De hoek die uit de database wordt gehaald
 */
void turn(int z) {
  moveWheels(120, 0, 125, 0);
  mpu.update();
  int currAngle = mpu.getGyroAngleZ();
  // ideaal gezien zit z heel dicht bij currAngle
  // currAngle wordt groter naarmate de robot naar links draait
  // en kleiner naarmate je naar rechts draait
  if(currAngle < z) {
    // naar links
    modifySpeed -= 5;
  } else {
    // naar rechts
    modifySpeed += 5;
  }
  moveWheels(modifySpeed, 0, 125, 0);
}
