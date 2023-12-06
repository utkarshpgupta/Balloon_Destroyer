#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);
Adafruit_DCMotor *M3 = AFMS.getMotor(3);
Adafruit_DCMotor *M4 = AFMS.getMotor(4);

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  AFMS.begin();
  M2->setSpeed(255);
  M3->setSpeed(150);
  M4->setSpeed(200);

  pinMode(2, OUTPUT); //Controls the relays for the laser
  pinMode(3, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read(); // Reads incoming commands from the Python script

    // Execute the command
    switch (command) {
      case 'H':
        M2->run(BACKWARD);// right
        M4->run(BACKWARD);//up
        M3->run(FORWARD);//up
        delay(5000);
        M2->run(RELEASE);// right
        M4->run(RELEASE);//up
        M3->run(RELEASE);//up
        break;
      case 'B':
        M2->run(FORWARD);// right
        M4->run(FORWARD);//up
        M3->run(BACKWARD);//up
        delay(3000);
        M2->run(RELEASE);// right
        M4->run(RELEASE);//up
        M3->run(RELEASE);//up
        break;
      case 'R': // Rotate base to the right
        M2->run(FORWARD);
        break;
      case 'L': // Rotate base to the left
        M2->run(BACKWARD);
        break;
      case 'U': // Move the arm up
        M4->run(BACKWARD);
        M3->run(FORWARD);
        break;
      case 'D': // Move the arm down
        M4->run(FORWARD);
        M3->run(BACKWARD);
        break;
      case 'S': // Stops the movement of the motors
        M2->run(RELEASE);
        M4->run(RELEASE);
        M3->run(RELEASE);
        break;
      case 'X':
        digitalWrite(2,LOW); // Turns on the laser only when X is sent over serial
        digitalWrite(3,LOW);
        delay(2000);
        break;
      case 'K':
        M4->run(BACKWARD);
        break;

      case 'J':
        M4->run(FORWARD);
        break;

      case 'O':
        M3->run(BACKWARD);
        break;

      case 'P':
        M3->run(FORWARD);
        break;
      
      default:
        digitalWrite(2,HIGH); // Keeps the relay/laser off otherwise
        digitalWrite(3,HIGH);

    }
  }
  else{
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
  }
  
}
