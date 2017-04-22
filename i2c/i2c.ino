

#include <Adafruit_MLX90614.h> //biblioteki potrzebne do obslugi pirometru

Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //mlx.readAmbientTempC() - temperatura kolo, mlx.readObjectTempC() - temperatura otoczenia

void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  mlx.begin();
  analogWriteResolution(8);
}

void loop(){

  float sensorVal1 = mlx.readObjectTempC();
  float sensorVal2 = mlx.readAmbientTempC();
  
  Serial.print(sensorVal1);
  Serial.print(" ");
  Serial.println(sensorVal2);
  int timeStart = micros();
  analogWrite(5, map(sensorVal1, 0, 100, 0 ,255));
  int timeEnd = micros();
  Serial.print("time1: ");
  Serial.println(timeEnd-timeStart);

  timeStart = micros();
  analogWrite(6, map(sensorVal2, 0, 100, 0 ,255));
  timeEnd = micros();
  Serial.print("time2: ");
  Serial.println(timeEnd-timeStart);
  
  delay(1000);
  
}


