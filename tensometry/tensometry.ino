
//bedziemy wysylac czasy i obroty co sekunde
//z korby co sekunde czasy nie sa w miare dobre dlatego skladujemy je co sekunde


const int size = 5;

// kontraktor 1 - korba
static unsigned long lastTime2_kon1;
static unsigned long lastTime_kon1;
volatile int tab_index_kon1 = 0;
int tab_kon1[size + 3];  //tablcia z czasami od jednego pedalu lewego
int temp_tab_kon1[size +3];

// kontraktor 2 - korba
static unsigned long lastTime2_kon2;
static unsigned long lastTime_kon2;
volatile int tab_index_kon2 = 0;
int tab_kon2[size + 3]; //tablcia z czasami od jednego pedalu prawego
int temp_tab_kon2[size + 3];

// kontraktor 3 - kolo
volatile int steps_kon3;
static unsigned long lastTime2_kon3;
volatile unsigned int set1Second_kon3;
static unsigned long lastTime_kon3;

unsigned int timer;


void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  tab_kon1[4] = 0;
  tab_kon2[4] = 0;
  attachInterrupt(digitalPinToInterrupt(2), onStep_1, RISING);   // kontraktor 1 - korba
  attachInterrupt(digitalPinToInterrupt(3), onStep_2, RISING);   // kontraktor 2 - korba
  attachInterrupt(digitalPinToInterrupt(4), onStep_3, RISING);   // kontraktor 3 - kolo
  timer = millis();

}

void loop()
{
  delay(10);

  if(millis()>timer){
    timer = millis()+1000;

    if (tab_kon1[4] != 0 && tab_kon2[4] != 0)
    {
      tabToTemp(tab_kon1, temp_tab_kon1, tab_kon2, temp_tab_kon2);
      tab_index_kon1 = 0;
      tab_index_kon2 = 0;
      tab_kon1[4] = 0;
      tab_kon2[4] = 0;
      Serial.print("KontaktronPedaly");
      sendToSerial(temp_tab_kon1);
      sendToSerial(temp_tab_kon2);
      Serial.println();

    }

    Serial.print("KontaktronKolo ");
    Serial.print(steps_kon3);
    Serial.print(" ");
    Serial.print(set1Second_kon3);
    Serial.println();

    steps_kon3 = 0;
    set1Second_kon3 = 0;
  }
}

//funkcja kopiujaca elementy tablicy
void tabToTemp(int *tab_kon1, int *temp_tab_kon1,int *tab_kon2,int *temp_tab_kon2)
{

	for (int i = 0; i < (size-1); i++)
	{
		temp_tab_kon1[i] = tab_kon1[i];
	}
	for (int i = 0; i < (size - 1); i++)
	{
		temp_tab_kon2[i] = tab_kon2[i];
	}
}
//funkcja wysyla na Seriala czasy z pedalow
void sendToSerial(int *tab)
{
	for (int i = 0; i < size - 1; i++)
	{
		Serial.print(" ");
		Serial.print(tab[i]);
	}
}

void onStep_1()
{
  unsigned long timeNow_kon1 = millis();
  lastTime2_kon1 = timeNow_kon1 - lastTime_kon1;
  if (lastTime2_kon1 < 50)
    return;
  lastTime_kon1 = timeNow_kon1;
  tab_kon1[tab_index_kon1] = lastTime2_kon1;
  tab_index_kon1++;

}
//funkcja do przerwania kontaktron kontaktron
void onStep_2()
{
  unsigned long timeNow_kon2 = millis();
  lastTime2_kon2 = timeNow_kon2 - lastTime_kon2;
  if (lastTime2_kon2 < 50)
    return;
  lastTime_kon2 = timeNow_kon2;
  tab_kon2[tab_index_kon2] = lastTime2_kon2;
  tab_index_kon2++;

}
//funkcja do przerwania kontaktron kolo
void onStep_3()
{
  unsigned long timeNow_kon3 = millis();
  lastTime2_kon3 = timeNow_kon3 - lastTime_kon3;
  set1Second_kon3 += lastTime2_kon3;
  if (lastTime2_kon3 < 50)
    return;
  steps_kon3++;
  lastTime_kon3 = timeNow_kon3;

}
//funkcja 
 int max(int *tab)
 {
   int max = -1;
   for(int i = 0; i < size_tens; i++){
     if(tab[i] > max)
      max = tab[i]
   }
   return max;
 }
