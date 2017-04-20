
// kontraktor 1 - korba
volatile int steps_kon1;
static unsigned long lastTime2_kon1;
volatile unsigned int set1Second_kon1;
static unsigned long lastTime_kon1;

// kontraktor 2 - korba
volatile int steps_kon2;
static unsigned long lastTime2_kon2;
volatile unsigned int set1Second_kon2;
static unsigned long lastTime_kon2;

unsigned int timer;


void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), onStep_1, RISING);   // kontraktor 1 - korba
  attachInterrupt(digitalPinToInterrupt(3), onStep_2, RISING);   // kontraktor 2 - korba
  timer = millis();
}

void loop()
{
  delay(10);

  if(millis()>timer){
    timer = millis()+1000;

    // kontaktron 1
    Serial.print("Steps1: ");
    Serial.println(steps_kon1);
    Serial.print("Time1: ");
    Serial.println(set1Second_kon1);
    Serial.print("Velocity: ");
    Serial.println((((float)steps_kon1*1000)/set1Second_kon1)*3.6);

    // kontaktron 2
    Serial.print("Steps2: ");
    Serial.println(steps_kon2);
    Serial.print("Time2: ");
    Serial.println(set1Second_kon2);
    Serial.print("Velocity: ");
    Serial.println((((float)steps_kon2*1000)/set1Second_kon2)*3.6);

    steps_kon1 = 0;
    steps_kon2 = 0;
    set1Second_kon1 = 0;
    set1Second_kon2 = 0;
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

void onStep_2()
{
  unsigned long timeNow_kon2 = millis();
  lastTime2_kon2 = timeNow_kon2 - lastTime_kon2;
  set1Second_kon2 += lastTime2_kon2;
  if (lastTime2_kon2 < 50)
    return;

  steps_kon2++;
  lastTime_kon2 = timeNow_kon2;
}
