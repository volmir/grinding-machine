#define delay_min 1
#define delay_max 12

#define potentiometr_pin A1
int potentiometr_val;
int delay_val;

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

  stepper.setMaxSpeed(7000);
  stepper.setAcceleration(3000);
}

void loop() {

  potentiometr_val = analogRead(potentiometr_pin);
  delay_val = map(potentiometr_val, 0, 1023, delay_max, delay_min);
  //Serial.println(potentiometr_val);
  //Serial.println(delay_val);


  delay_val = delay_val * 480;

  if (sensorFlag) {
    stepper.setSpeed(-delay_val);
  } else {
    stepper.setSpeed(delay_val);
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
