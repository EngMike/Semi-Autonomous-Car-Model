#include <NewPing.h>
#include <Servo.h>
#define TRIGGER_PIN A3
#define ECHO_PIN A4
#define max_distance 50
#define irLeft A1
#define irRight A2

// Motoarele
int ENA = 5; 
int ENB = 6; 
int MOTOR_A1 = 3; 
int MOTOR_A2 = 4; 
int MOTOR_B1 = 7; 
int MOTOR_B2 = 8; 

Servo servo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, max_distance);

int distance = 0;
int leftDistance;
int rightDistance;
boolean object;

void setup() {
  Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  servo.attach(11);
  servo.write(90);

pinMode(ENA, OUTPUT); 
pinMode(ENB, OUTPUT);
pinMode(MOTOR_A1, OUTPUT); 
pinMode(MOTOR_A2, OUTPUT); 
pinMode(MOTOR_B1, OUTPUT); 
pinMode(MOTOR_B2, OUTPUT); 
}

void loop() {
  if (digitalRead(irLeft) == 0 && digitalRead(irRight) == 0 ) {
    objectAvoid();
  }
  else if (digitalRead(irLeft) == 0 && digitalRead(irRight) == 1 ) {
    objectAvoid();
    Serial.println("TL");
    moveLeft();
  }
  else if (digitalRead(irLeft) == 1 && digitalRead(irRight) == 0 ) {
    objectAvoid();
    Serial.println("TR");
    moveRight();
  }
  else if (digitalRead(irLeft) == 1 && digitalRead(irRight) == 1 ) {
    Stop();
  }
} 


void objectAvoid() {
  distance = getDistance();
  if (distance <= 15) {
    //stop
    Stop();
    Serial.println("Stop");

    lookLeft();
    lookRight();
    delay(100);
    if (rightDistance <= leftDistance) {
      // stânga
      object = true;
      turn();
      Serial.println("moveLeft");
    } else {
      // dreapta
      object = false;
      turn();
      Serial.println("moveRight");
    }
    delay(100);
  }
  else {
    // înainte
    Serial.println("moveforward");
    moveForward();
  }
}

int getDistance() {
  delay(50);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 100;
  }
  return cm;
}

int lookLeft () {
  servo.write(150);
  delay(500);
  leftDistance = getDistance();
  delay(100);
  servo.write(90);
  Serial.print("Left:");
  Serial.print(leftDistance);
  return leftDistance;
  delay(100);
}

int lookRight() {
  servo.write(30);
  delay(500);
  rightDistance = getDistance();
  delay(100);
  servo.write(90);
  Serial.print("   ");
  Serial.print("Right:");
  Serial.println(rightDistance);
  return rightDistance;
  delay(100);
}
void Stop() {
  analogWrite(ENA, 0); 
  analogWrite(ENB, 0); 

  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}
void moveForward() {
 analogWrite(ENA, 50);  
 analogWrite(ENB, 50); 
  

  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}
void moveBackward() {
 analogWrite(ENA, 100); 
  analogWrite(ENB, 100);
  
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);

}
void turn() {
  if (object == false) {
    Serial.println("turn Right");
    moveRight();
    delay(600);
    moveForward();
    delay(2500);
    moveLeft();
    delay(1200);
    moveForward();
    delay(2500);
    moveRight();
    delay(600);    
    if (digitalRead(irRight) == 1) {
      loop();
    } else {
      moveForward();
    }
  }
  else {
    Serial.println("turn left");
    moveLeft();
    delay(600); 
    moveForward();
    delay(2500); 
    moveRight();
    delay(1200); 
    moveForward();
    delay(2500); 
    moveLeft();
    delay(600); 
    if (digitalRead(irLeft) == 1) {
      loop();
    } else {
      moveForward();
    }
  }
}
void moveRight() { 
  analogWrite(ENA, 95); 
  analogWrite(ENB, 95); 
  
  // Rotire spre dreapta
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}
void moveLeft() { 
  analogWrite(ENA, 95); 
  analogWrite(ENB, 95); 

  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}
