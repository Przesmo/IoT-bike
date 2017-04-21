
volatile int steps_kon1;
static unsigned long lastTime2_kon1;
volatile unsigned int set1Second_kon1;
static unsigned long lastTime_kon1;
unsigned int timer;


void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), onStep_1, RISING);
  timer = millis();
}

void loop()
{
  delay(10);

  if(millis()>timer){
    timer = millis()+1000;

    Serial.print("Steps1: ");
    Serial.println(steps_kon1);
 }
}

void onStep_1()
{
  unsigned long timeNow_kon1 = millis();
  lastTime2_kon1 = timeNow_kon1 - lastTime_kon1;
  set1Second_kon1 += lastTime2_kon1;
  if (lastTime2_kon1 < 50)
    return;

  steps_kon1++;
  lastTime_kon1 = timeNow_kon1;
}
