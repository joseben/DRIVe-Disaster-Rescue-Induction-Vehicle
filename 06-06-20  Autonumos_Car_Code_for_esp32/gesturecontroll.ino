

 void move()
    { if(myData.x < 415) //Forward
{
  digitalWrite(27, LOW);
  digitalWrite(26, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(33, LOW);
  analogWrite(32, 255);
  analogWrite(23, 255);
  Serial.println("1");
  
}else if(myData.x > 435) // Reverse
{
  digitalWrite(27, HIGH);
  digitalWrite(26, LOW);
  digitalWrite(25, LOW);
  digitalWrite(33, HIGH);
  analogWrite(32, 255);
  analogWrite(23, 255);
  Serial.println("2");
  
} else if(myData.y < 460)//LeftTurn
{
  digitalWrite(27, HIGH);
  digitalWrite(26, LOW);
  digitalWrite(25, HIGH);
  digitalWrite(33, LOW);
  analogWrite(32, 240);
  analogWrite(23, 240);
  Serial.println("3");
  
}else if(myData.y > 490)//RightTurn
{
  digitalWrite(27, LOW);
  digitalWrite(26, HIGH);
  digitalWrite(25, LOW);
  digitalWrite(33, HIGH);
  analogWrite(32, 240);
  analogWrite(23, 240);
  Serial.println("4");
}else                    //stop
{
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  digitalWrite(25, LOW);
  digitalWrite(33, LOW);
  analogWrite(32, 0);
  analogWrite(23, 0);
  Serial.println("5");
  }
 }
  
