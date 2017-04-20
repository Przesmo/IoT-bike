volatile int steps;
volatile int oldSteps;
volatile unsigned int set1Second;
volatile unsigned int speeed;
static unsigned long lastTime2;
unsigned int timer;
static unsigned long lastTime;
void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3,INPUT);
  attachInterrupt(digitalPinToInterrupt(2), onStep,RISING);
  attachInterrupt(digitalPinToInterrup(3),onStep,RISING);
  timer = millis();
  //dodac dwa kontakrony pin 2,5,7
}

void loop()
{
  delay(10);
 
 if(millis()>timer){
  timer = millis()+1000;
  Serial.print("Steps: ");
  Serial.println(steps);
  Serial.print("Time: ");
  Serial.println(set1Second);
  Serial.println((steps*1000)/set1Second)); //przeliczyc na km/h
 // lastTime =millis();
  steps = 0;
  set1Second = 0; 
 }
}

void onStep()
{
  unsigned long timeNow = millis();
  lastTime2 = timeNow - lastTime;
 set1Second+=lastTime2;
 if (lastTime2 < 50)
    return;

  steps++;
  lastTime = timeNow;
}
