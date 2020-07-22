#include <analogWrite.h>

//ping sensors ==13,34,39,36

#define trigPin1 13
#define echoPin1 34


long duration, distance,center;


void StopCar()
{ digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  digitalWrite(25, LOW);
  digitalWrite(33, LOW);
  analogWrite(32, 0);
  analogWrite(23, 0);
  Serial.println("Stop");
 }
 
void Reverse()
{
   digitalWrite(27, HIGH);
  digitalWrite(26, LOW);
  digitalWrite(25, LOW);
  digitalWrite(33, HIGH);
  analogWrite(32, 255);
  analogWrite(23, 255);
}
 
void setup()
{
Serial.begin (152000);
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(23, OUTPUT);

}

void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;

}

void loop() {
SonarSensor(trigPin1, echoPin1);
center = distance;
Serial.print("Center");
Serial.print(" - "); 
Serial.print(center);
check();
}

void check()
{
     if(center<20)
     {
      Reverse();          // Quick reverse to Stop quickly
      Serial.println("Reverse");
      delay(300);
      StopCar(); 
      Serial.println("StopCar");
      delay(300);
          Serial.println("Turn Left");
          digitalWrite(27, HIGH);
          digitalWrite(26, LOW);
          digitalWrite(25, HIGH);
          digitalWrite(33, LOW);
          analogWrite(32, 240);
          analogWrite(23, 240);
          delay(300);  
          Serial.println("Move Forward");
          digitalWrite(27, LOW);
          digitalWrite(26, HIGH);
          digitalWrite(25, HIGH);
          digitalWrite(33, LOW);
          analogWrite(32, 255);
          analogWrite(23, 255);
          delay(500); 
          Serial.println("StopCar");
          Serial.println("Turn Right");
          digitalWrite(27, LOW);
          digitalWrite(26, HIGH);
          digitalWrite(25, LOW);
          digitalWrite(33, HIGH);
          analogWrite(32, 240);
          analogWrite(23, 240);
          delay(300); 
          Serial.println("StopCar");       
          digitalWrite(27, LOW);
          digitalWrite(26, LOW);
          digitalWrite(25, LOW);
          digitalWrite(33, LOW);
          analogWrite(32, 0);
          analogWrite(23, 0);        
          Serial.println("Check");
        
         
      }
      else
      {
        Serial.println("Move Forward");
          digitalWrite(27, LOW);
          digitalWrite(26, HIGH);
          digitalWrite(25, HIGH);
          digitalWrite(33, LOW);
          analogWrite(32, 255);
          analogWrite(23, 255);
          delay(100); 
      }
}
