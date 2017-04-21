//bedziemy wysylac czasy i obroty co sekunde
//z korby co sekunde czasy nie sa w miare dobre dlatego skladujemy je co sekunde 






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

// kontraktor 3 - kolo
volatile int steps_kon3;
static unsigned long lastTime2_kon3;
volatile unsigned int set1Second_kon3;
static unsigned long lastTime_kon3;

const int rozmiar = 5;
int tabP1[rozmiar + 3];  //tablcia z czasami od jednego pedalu lewego
int tabP2[rozmiar + 3]; //tablcia z czasami od jednego pedalu prawego
int tempTabP1[rozmiar +3];
int tempTabP2[rozmiar + 3];
unsigned int timer;


void setup()
{
	Serial.begin(9600);
	pinMode(2, INPUT_PULLUP);
	pinMode(3, INPUT_PULLUP);
	pinMode(4, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(2), onStep_1, RISING);   // kontraktor 1 - korba
	attachInterrupt(digitalPinToInterrupt(3), onStep_2, RISING);   // kontraktor 2 - korba
	attachInterrupt(digitalPinToInterrupt(4), onStep_3, RISING);   // kontraktor 3 - kolo
	timer = millis();
}

void loop()
{
	delay(10);

	if (millis()>timer) {
		
		timer = millis() + 1000;
		

		if (tabP1[4] != 0 && tabP2[4] != 0)
		{
			tabToTemp(tabP1, tempTabP1, tabP2, tempTabP2);
			Serial.print("KontaktronPedaly");
			sendToSerial(tabP1);
			sendToSerial(tabP2);
			Serial.println();

		}

		Serial.print("KontaktronKolo ");
		Serial.print(steps_kon3);
		Serial.print(" ");
		Serial.print(set1Second_kon3);
		Serial.println();
		










		
		//// kontaktron 1
		//Serial.print("Steps1: ");
		//Serial.println(steps_kon1);
		//Serial.print("Time1: ");
		//Serial.println(set1Second_kon1);
		//Serial.print("Velocity: ");
		//Serial.println(((steps_kon1 * 1000 * 10000) / set1Second_kon1)*3.6);  // nie dziala poprawnie bo liczy jak na int'ach

		//																	  // kontaktron 2
		//Serial.print("Steps2: ");
		//Serial.println(steps_kon2);
		//Serial.print("Time2: ");
		//Serial.println(set1Second_kon2);
		//Serial.print("Velocity: ");
		//Serial.println(((steps_kon2 * 1000 * 10000) / set1Second_kon2)*3.6);  // nie dziala poprawnie bo liczy jak na int'ach

		//Serial.print("Steps3: ");
		//Serial.println(steps_kon3);
		//Serial.print("Time3: ");
		//Serial.println(set1Second_kon3);
		//Serial.print("Velocity: ");
		//Serial.println(((steps_kon3 * 1000 * 10000) / set1Second_kon3)*3.6);  // nie dziala poprawnie bo liczy jak na int'ach

		//steps_kon1 = 0;
		//steps_kon2 = 0;
		//steps_kon3 = 0;
		//set1Second_kon1 = 0;
		//set1Second_kon2 = 0;
		//set1Second_kon3 = 0;
	}
}
//funkcja do przerwania kontaktron kontaktron
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
//funkcja do przerwania kontaktron kontaktron
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
 

//funkcja kopiująca kopiujaca elementy tablicy
void tabToTemp(int *tabP1, int *tempTabP1,int *tabP2,int *tempTab2)
{
	
	for (int i = 0; i < (rozmiar-1); i++)
	{
		tempTabP1[i] = tabP1[i];
	}
	for (int i = 0; i < (rozmiar - 1); i++)
	{
		tempTabP2[i] = tabP2[i];
	}
}
//funkcja wysyla na Seriala czasy z pedalow
void sendToSerial(int *tab)
{
	for (int i = 0; i < rozmiar - 1; i++)
	{
		Serial.print(" ");
		Serial.print(tabP1[i]);
		Serial.print(tabP2[i]);

	}
}