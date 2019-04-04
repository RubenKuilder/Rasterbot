#define L_IR 10      // left infrared sensor
#define R_IR 11      // right infrared sensor

/*-------definning Outputs------*/
#define LF 3         // left motor forward
#define LB 2         // left motor backward
#define RF 9         // right motor forward
#define RB 4         // right motor backward

int timeForOneMeter;
long timer = 0;
int startTimer = 0;
boolean endTimer, start = false;
boolean firstTime = true;

void setup() {
  pinMode(LF, OUTPUT);   //left motors forward
  pinMode(LB, OUTPUT);   //left motors reverse
  pinMode(RF, OUTPUT);   //right motors forward
  pinMode(RB, OUTPUT);   //right motors reverse

  pinMode(L_IR, INPUT);
  pinMode(R_IR, INPUT);
}

void loop() {
  vanSpelNaarSpel(3);
}

void moveWheels(int leftForwardSpeed, int leftBackwardSpeed, int rightForwardSpeed, int rightBackwardSpeed) {
  analogWrite(LF, leftForwardSpeed);
  analogWrite(LB, leftBackwardSpeed);
  analogWrite(RF, rightForwardSpeed);
  analogWrite(RB, rightBackwardSpeed);
}

/**
 * Laat de robot een bepaalde afstand afleggen van spel naar spel,
 * afhankelijk van een stuke tape op een bepaalde afstand (één meter) en
 * ervan uitgaande dat de robot de goede richting op staat
 * @param distance De af te leggen afstand van spel naar spel in meters
 */
void vanSpelNaarSpel(int distance) {  
  if(firstTime) {
    // beginnen met rijden
    moveWheels(120, 0, 130, 0);
    // huidige tijd vastleggen
    startTimer = millis();
    firstTime = false;
  }

  // tijd meten die de robot over één meter doet
  if(!endTimer && digitalRead(L_IR) && digitalRead(R_IR)) {
    int timeForOneMeter = (millis() - startTimer); // timeForOneMeter is de tijd die de robot doet over één meter, in milliseconden
    endTimer = true; // stop huidige loop
    start = true; // start volgende loop
  }
  
  if(start) {
    int distanceToDrive = distance - 1; // de robot heeft al één meter gereden
    moveWheels(120, 0, 130, 0);
    float timeToDrive = (distanceToDrive * (timeForOneMeter / 20));  // timeToDrive is de tijd in milliseconden die de robot moet rijden voor distanceToDrive
    for(float t = 0.0; t < timeToDrive; t++) { // for-loop om robot voor een bepaalde tijd "op te houden", dan rijd de bot voor een bepaalde tijd lang
      delay(50);
    }
    moveWheels(0, 0, 0, 0);
    start = false;
  }
}
