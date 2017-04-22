

const int analogInPin = A0;
int sensorValue = 0;        
int nacisk = 0;

void setup() {
  
  Serial.begin(9600);
}

void loop() 
{
 sensorValue = analogRead(analogInPin);
 //nacisk = ((sensorValue/1500)/(0.4*5))*2000;
 Serial.print("{\"NACISK\": ");
 Serial.print(sensorValue);
 Serial.println("}");
 delay(50);
}
