

const int analogInPin = A0;
int sensorValue1 = 0;  
int sensorValue2 = 0;      
int nacisk = 0;

void setup() {
  
  Serial.begin(9600);
}

void loop() 
{
 sensorValue1 = analogRead(A0);
 sensorValue2 = analogRead(A1);
 Serial.println(sensorValue1);
 //Serial.print(" ");
 //Serial.print(sensorValue1);
 delay(50);
}
