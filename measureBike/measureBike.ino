
//DO ZROBIEN
#include <Wire.h>
//program do generowania przykladowych zmiennych
//wedlug ustalonego wczesniej json'a plik:: nowe_strukturt.txt

//naciski trzeba przeliczyc aby wynik byl podawany w Newton
//NACISKI NA PEDALY LEWY/PRAWY
volatile bool startMeasureL;  //zostana ustawione na true jezeli zostanie wywolane funkcje onStep
volatile bool startMeasureP;
const int nLPin = A3;
const int nPPin = A4;
int counterL;
int counterP;
const int liczbaPomiarowNacisk = 20;
volatile float lPedal[liczbaPomiarowNacisk]; // tablica do przechowywania nacisku na lewy pedal
volatile float pPedal[liczbaPomiarowNacisk]; //tablica do przechowywania nacisku na prawy pedal
volatile float maksLPedal;
volatile float maksPPedal;



//NACISK NA PRZEDNI ORAZ TYLNIE KOLO
volatile float naciskPrz;
volatile float naciskT;
const int nPrzPin = A2;
const int nTPin = A1;
volatile float offSetPrz =35;
volatile float offSetT = 150;


//temperatura podawana w stopniach celsjusza
//TEMPERATURA TYLNIEGO KOLO + TEMPERATURA OTOCZENIA
byte tempKoloTylPin = 5; //temperatura obiektu
byte tempOtoczeniaPin = 6; //temperatura otoczenia
volatile float tempKoloTyl;
volatile float tempOtoczenia;

//TYLNIE KOLO
const int kPin = 1;
unsigned long lastTimeK; //czas w poprzednim przejsciu kola
unsigned long timeNowK; // czas w ktorym przejdzie kola
volatile unsigned int timeLapK; //czas okrazenia lewej korby
volatile int nrInterrupt; //liczba okrazen kola
volatile unsigned int tyl = 600; //liczba obrotow z dokladnoscia do 1/4 obwodu kola
volatile unsigned int czasTrwania = 700; //czas obrotu w milisekundach*/

//KORBA LEWA/PRAWA
const int kLPin = 2;
unsigned long lastTimeKL; //czas w poprzednim przejsciu korby
unsigned long timeNowKL; // czas w ktorym przejdzie korba
volatile unsigned int timeLapL; //czas okrazenia lewej korby
const int kPPin = 3;
unsigned long lastTimeKP; //czas w poprzednim przejsciu korby
unsigned long timeNowKP; // czas w ktorym przejdzie korba
volatile unsigned int timeLapP; //czas okrazenia prawej korby


//EKG
volatile unsigned int wykresEKG  = 0;
volatile unsigned int puls = 0;  //wyznaczany na podstawie ekg przy pomocy funkcji z biblioteki adafruit



unsigned int  timer; //zmienna pomocnicza do wyznaczania jednej sekundy
void setup() {
  
  Serial.begin(9600);
  timer = millis(); 
  counterL = 0; //do zliczania pomiarow nacisku na lewy pedal
  counterP = 0; //do zliczania pomiarow nacisku na prawy pedal
  
  //definicja pinow zwiazanych z przerwaniami( koontaktrony):
  pinMode(kLPin,INPUT);
  pinMode(kPPin,INPUT);
  pinMode(kPin,INPUT);

  //deklaracja przerwan:
  attachInterrupt(digitalPinToInterrupt(kLPin), onStepLewa, RISING); // przerwanie dla lewego pedalu obslugujaca funkcje zliczania czasu jednego okrazenia korby
  attachInterrupt(digitalPinToInterrupt(kPPin), onStepPrawa, RISING); // przerwanie dla lewego pedalu obslugujaca funkcje zliczania czasu jednego okrazenia korby
  attachInterrupt(digitalPinToInterrupt(kPin), onStepKolo, RISING);   // przerwanie dla tylnego kolo mamy cztery(albo dwa) punkty pomiarowe


 
  //definicja pinow dla odczytu temperatury z pirometrow
  pinMode(tempKoloTylPin, INPUT);
  pinMode(tempOtoczeniaPin, INPUT);
  
}

void loop() {
  if(startMeasureL == true && counterL < liczbaPomiarowNacisk) //warunek logiczny rozpoaczynajacy pobieranie pomiarow naciskow z lewego pedalu
 {
    lPedal[counterL] = analogRead(nLPin);
    if(counterL == liczbaPomiarowNacisk-1)
    {
    maksLPedal =  findMax(lPedal);
     counterL = -1; // po wyjsciu z if counter zwiekszy sie o 1 i bedzie 0
     startMeasureL = false;
    }
    counterL++;
 }

 if(startMeasureP == true && counterP < liczbaPomiarowNacisk + 1) //warunek logiczny rozpoaczynajacy pobieranie pomiarow naciskow z prawego pedalu
 {
    pPedal[counterP] = analogRead(nPPin);
    if(counterP == liczbaPomiarowNacisk)
    {
     maksPPedal =  findMax(pPedal);
     counterP = -1; // po wyjsciu z if counter zwiekszy sie o 1 i bedzie 0
     startMeasureP = false;
    }
    counterP++;
 }

 if(millis()>timer)
 {
  timer = millis()+1000;

  //odczytanie PWM (temperatury)
 /* tempKoloTyl = pulseIn(tempKoloTylPin, HIGH,100);
  tempOtoczenia = pulseIn(tempOtoczeniaPin, HIGH,100);
  tempKoloTyl = tempKoloTyl/10;
  tempOtoczenia = tempOtoczenia/10;*/

  //odczytanie wartosci nacisku z przedniego i tylnego kola odejmujemy offset
  naciskPrz = analogRead(nPrzPin) - offSetPrz;
  naciskT = analogRead(nTPin) - offSetT;
  //sprawdzac czy ktos siedzi na rowerze poprzez zczytanie nacisku pod kolami wartosci powinny byc mniejsze niz 50 plus wyliczyc offset jezeli ktos siedzi i wysylac tylko pomiar rozniacy sie od offsetu
  sendData(maksPPedal,maksLPedal,naciskPrz,naciskT, tempOtoczenia, tempKoloTyl,nrInterrupt,timeLapK,timeLapL,timeLapP,wykresEKG,puls);
  maksLPedal = 0; // zerowanie wartosc (chyba niepotrzebne ale moze byc dla pewnosci)
  maksPPedal = 0;
  timeLapL = 0;
  timeLapP = 0;
  timeLapK = 0;
  nrInterrupt = 0;
 }
}

 

void sendData(volatile float &naciskPedalPrawy,volatile float &naciskPedalLewy,volatile float &naciskKoloPrzod,volatile float &naciskKoloTyl,double tempKoloTyl,double tempOtoczenia,volatile int &tyl,volatile unsigned int &czasTrwania,volatile unsigned int &czasObrotuKorbaLewa,volatile unsigned int &czasObrotuKorbaPrawa,volatile unsigned int &wykresEKG,volatile unsigned int &puls)
{
  
  Serial.print("{\"naciskPedalPrawy\": ");
  Serial.print(naciskPedalPrawy);
  Serial.print(",");
  Serial.print("\"naciskPedalLewy\": ");
  Serial.print(naciskPedalLewy);
  Serial.print(",");
  Serial.print("\"naciskKoloPrzod\": ");
  Serial.print(naciskKoloPrzod);
  Serial.print(",");
  Serial.print("\"naciskKoloTyl\": ");
  Serial.print(naciskKoloTyl);
  Serial.print(",");
  Serial.print("\"tempKoloTyl\": ");
  //if(tempKoloTyl == 0 ) Serial.print("null");
  Serial.print(tempKoloTyl);
  Serial.print(",");
  Serial.print("\"tempOtoczenia\": ");
  //if(tempOtoczenia == 0 ) Serial.print("null");
  Serial.print(tempOtoczenia);
  Serial.print(",");
  Serial.print("\"obrotyKolo\": {");
  
  Serial.print("\"tyl\": ");
  Serial.print(tyl);
  Serial.print(",");
  Serial.print("\"czasTrwania\": ");
  Serial.print(czasTrwania);
  Serial.print("},");
  Serial.print("\"czasObrotuKorbaLewa\": ");
  if(czasObrotuKorbaLewa>3000)
  Serial.print(0);
  else
  Serial.print(czasObrotuKorbaLewa);
  Serial.print(",");
  Serial.print("\"czasObrotuKorbaPrawa\": ");
  Serial.print(czasObrotuKorbaPrawa);
  Serial.print(",");
  Serial.print("\"EKG\": {");
  Serial.print("\"wykresEKG\": ");
  Serial.print(wykresEKG);
  Serial.print(",");
  Serial.print("\"puls\": ");
  Serial.print(puls);
  Serial.print("}");
  Serial.print("}");
  Serial.println();
 
}

float findMax(volatile float *tab)
{
 int  maximum = tab[0];
 //Serial.println("JESTES W FUNKCJI FIND MAX");
  for(int i = 1; i < liczbaPomiarowNacisk ; i++)
  {
    if(tab[i]>maximum) maximum = tab[i];
  }
  return maximum;
}

void onStepLewa()
{
  timeNowKL = millis();
  timeLapL = timeNowKL - lastTimeKL;
  if (timeLapL < 50)
    return;
  lastTimeKL = timeNowKL;
  startMeasureL = true;
}

void onStepPrawa()
{
  timeNowKP = millis();
  timeLapP = timeNowKP - lastTimeKP;
  if (timeLapP < 50)
    return;
  lastTimeKP = timeNowKP;
  startMeasureP = true;
}

void onStepKolo()
{
  unsigned long timeNowK = millis();
  lastTimeK = timeNowK- lastTimeK;
  timeLapK += lastTimeK;
  if (lastTimeK < 50)
    return;
  nrInterrupt++;
  lastTimeK = timeNowK;
}

    
