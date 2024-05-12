//<mikey manual pitch home constants>
  const int pin = 11;
  const int mikeyMIN_PULSE_WIDTH = 1000; // 0 degrees
  const int mikeyMAX_PULSE_WIDTH = 2000; // 180 degrees
  const int PULSE_PERIOD = 150;
  const int homeAngle = 90;
//</mikey manual pitch home constants>

void setup() {
    Serial.begin(9600); // initializes the Serial communication between the computer and the microcontroller
    Serial.println("--");
    Serial.println("setup");
    mikeysPitchHome();
}

void mikeysPitchHome(){
  pinMode(pin , OUTPUT);
  Serial.println("home servo");
  int pulseWidth = map(homeAngle, 0, 180, mikeyMIN_PULSE_WIDTH, mikeyMAX_PULSE_WIDTH);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(pin, LOW);
  delayMicroseconds(PULSE_PERIOD * 1000 - pulseWidth);
  Serial.println("servo homed.");
}

////////////////////////////////////////////////
              //  L O O P  //
////////////////////////////////////////////////

void loop() {}