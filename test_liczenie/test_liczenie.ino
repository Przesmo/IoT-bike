
volatile int steps_kon1 = 2;
volatile unsigned int set1Second_kon1 = 1391 ;


void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.println((((float)steps_kon1*1000)/set1Second_kon1)*3.6);

}
