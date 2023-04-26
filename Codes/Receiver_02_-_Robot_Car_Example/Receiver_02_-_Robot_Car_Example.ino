#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define enA 5   
#define in1 3
#define in2 4
#define enB 6   
#define in3 7
#define in4 8
RF24 radio(9, 10); 
  
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};

Data_Package data; 

int  xAxis, yAxis;
int motorSpeedA = 0;
int motorSpeedB = 0;

const int ldrsen = A15; 
int Led = 26; 
int value3 = 0; 


int irSensorS = 32; 
int buzzerS = 34;   
int irSensorD = 2;  
int buzzerD = 12;   

void setup() {
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); 
  resetData();

  pinMode(Led, OUTPUT); 
  pinMode(ldrsen, INPUT);

  pinMode(irSensorS, INPUT);
  pinMode(buzzerS, OUTPUT);
  pinMode(irSensorD, INPUT);
  pinMode(buzzerD, OUTPUT);
  
} 


void loop() {
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { 
    resetData(); 
  }

  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); 
    lastReceiveTime = millis(); 
  }
  xAxis = data.j1PotX;
  yAxis = data.j1PotY;


  if (yAxis < 110) {

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    motorSpeedA = map(yAxis, 110, 0, 0, 255);
    motorSpeedB = map(yAxis, 110, 0, 0, 255);
    Serial.print(motorSpeedA);
    Serial.print("  ");
    Serial.println(yAxis);
  }
  else if (yAxis > 140) {

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
   
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    
    motorSpeedA = map(yAxis, 140, 255, 0, 255);
    motorSpeedB = map(yAxis, 140, 255, 0, 255);
  }
  
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  if (xAxis < 110) {
 
    int xMapped = map(xAxis, 110, 0, 0, 255);
  
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;

    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  if (xAxis > 140) {
   
    int xMapped = map(xAxis, 140, 255, 0, 255);

    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;

    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }

  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); 
  analogWrite(enB, motorSpeedB);



  value3 = analogRead(ldrsen)/4;
    if(value3 < 160){
    digitalWrite(Led, HIGH);
  }
  else
  {
    digitalWrite(Led, LOW);
  }

  int value = digitalRead(irSensorS);
  Serial.println("");
  Serial.print("Sensor Value = ");
  Serial.print(value);
  
  if(value == 0)
  {
    digitalWrite(buzzerS, HIGH);
  }
  else
  {
    digitalWrite(buzzerS, LOW);
  }


 int value2 = digitalRead(irSensorD);
  Serial.println("");
  Serial.print("Sensor Value = ");
  Serial.print(value2);
  
  if(value2 == 0)
  {
    digitalWrite(buzzerD, HIGH);
  }
  else
  {
    digitalWrite(buzzerD, LOW);
  }
        
} 

  
void resetData() {
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}
