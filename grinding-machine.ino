
#define potentiometr_pin A1
int potentiometr, potentiometr_last, potentiometr_min, potentiometr_max = 0;
int hysteresis = 10;
int speed_val;

#define step_pin 4
#define dir_pin 5
#define enable_pin 6

unsigned long stepTimer;

#define sensor_pin 2
boolean sensorFlag = true;
boolean sensor = false;
unsigned long sensorTimer;

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, step_pin, dir_pin);


void setup() {
  //Serial.begin(9600);
  
  pinMode(sensor_pin, INPUT_PULLUP);

  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(enable_pin, OUTPUT);
  
  digitalWrite(step_pin, LOW);
  digitalWrite(dir_pin, LOW);
  digitalWrite(enable_pin, HIGH);

  stepper.setMaxSpeed(6000);
  stepper.setAcceleration(300);
}

void loop() {

  potentiometr = analogRead(potentiometr_pin);
  potentiometr_min = potentiometr_last - hysteresis;
  potentiometr_max = potentiometr_last + hysteresis;
  if (potentiometr != potentiometr_last) {
    if ((potentiometr > potentiometr_max) || (potentiometr < potentiometr_min)) {
      potentiometr_last = potentiometr;
    }    
  }
  potentiometr_last = map(potentiometr_last, 0, 1023, 50, 1023);
  //Serial.println(potentiometr_last);
  
  speed_val = potentiometr_last * 5.6;
  //Serial.println(speed_val);

  if (sensorFlag) {
    stepper.setSpeed(-speed_val);
  } else {
    stepper.setSpeed(speed_val);
  }


  sensor = digitalRead(sensor_pin);
  //Serial.println(sensor);

  if (sensor && millis() - sensorTimer > 2000) {
    sensorFlag = !sensorFlag;
    sensorTimer = millis();
    //Serial.println(sensorFlag);

    digitalWrite(enable_pin, LOW);
    delay(200);
    digitalWrite(enable_pin, HIGH);
  }

  stepper.runSpeed(); 

}
