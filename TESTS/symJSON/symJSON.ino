#include <Wire.h>
#include <Adafruit_MLX90614.h> //biblioteki potrzebne do obslugi pirometru
#include "mfunction.h"
//program do generowania przykladowych zmiennych 
//wedlug ustalonego wczesniej json'a plik:: nowe_strukturt.txt

//naciski trzeba przeliczyc aby wynik byl podawany w Newton
//NACISKI NA PEDALY LEWY/PRAWY
bool startMeasureL; 
bool startMeasureP;
const int nLPin = A0;
const int nPPin = A1;
int counterL;
int counterP;
volatile float lPedal[20]; // tablica do przechowywania nacisku na lewy pedal 
volatile float pPedal[20]; //tablica do przechowywania nacisku na prawy pedal
volatile float maksLPedal;
volatile float maksPPedal;
volatile unsigned int  naciskPedalPrawy = 0;
volatile unsigned int naciskPedalLewy = 100;

//NACISK NA PRZEDNI ORAZ TYLNIE KOLO
volatile float naciskPrz;
volatile float naciskT;
const int nPrzPin = A2;
const int nTPin = A3;
volatile float offSetPrz;
volatile float offSetT;
volatile unsigned int naciskKoloPrzod = 200;
volatile unsigned int naciskKoloTyl = 300;

//temperatura podawana w stopniach celsjusza
//TEMPERATURA TYLNIEGO KOLO + TEMPERATURA OTOCZENIA
volatile unsigned int tempKoloTyl = 400;
volatile unsigned int tempOtoczenia = 500;
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //mlx.readAmbientTempC() - temperatura kolo, mlx.readObjectTempC() - temperatura otoczenia

//TYLNIE KOLO
const int kPin = 4;
unsigned long lastTimeK; //czas w poprzednim przejsciu kola
unsigned long timeNowK; // czas w ktorym przejdzie kola
volatile unsigned int timeLapK; //czas okrazenia lewej korby
volatile int nrInterrupt; //liczba okrazen kola
volatile unsigned int tyl = 600; //liczba obrotow z dokladnoscia do 1/4 obwodu kola
volatile unsigned int czasTrwania = 700; //czas obrotu w milisekundach

//KORBA LEWA/PRAWA
const int kLPin = 2;
unsigned long lastTimeKL; //czas w poprzednim przejsciu korby
unsigned long timeNowKL; // czas w ktorym przejdzie korba
volatile unsigned int timeLapL; //czas okrazenia lewej korby
const int kPPin = 3;
unsigned long lastTimeKP; //czas w poprzednim przejsciu korby
unsigned long timeNowKP; // czas w ktorym przejdzie korba
volatile unsigned int timeLapP; //czas okrazenia prawej korby
volatile unsigned int czasObrotuKorbaLewa = 800; //czas obrotu w milisekundach
volatile unsigned int czasObrotuKorbaPrawa = 900; //czas obrotu w milisekundach

//EKG
volatile unsigned int wykresEKG = 1000;
volatile unsigned int puls = 1100;  //wyznaczany na podstawie ekg przy pomocy funkcji z biblioteki adafruit

unsigned int  timer; //zmienna pomocnicza do wyznaczania jednej sekundy

void setup() {
  Serial.begin(9600);
  mlx.begin(); //rozpoczyna transmisje temperatury
  timer = millis();
  counterL = 1; //do zliczania pomiarow nacisku na lewy pedal
  counterP = 1; //do zliczania pomiarow nacisku na prawy pedal
  attachInterrupt(digitalPinToInterrupt(kLPin), onStepLewa, RISING); // przerwanie dla lewego pedalu obslugujaca funkcje zliczania czasu jednego okrazenia korby
  attachInterrupt(digitalPinToInterrupt(kPPin), onStepPrawa, RISING); // przerwanie dla lewego pedalu obslugujaca funkcje zliczania czasu jednego okrazenia korby
  attachInterrupt(digitalPinToInterrupt(kPin), onStepKolo, RISING);   // przerwanie dla tylnego kolo mamy cztery(albo dwa) punkty pomiarowe
  offSetPrz = 30;
  offSetT = 30;
}

void loop() {
  delay(10);
  //odczytanie wartosci nacisku z przedniego i tylnego kola odejmujemy offset 
  naciskPrz = analogRead(nPrzPin) - offSetPrz;
  naciskT = analogRead(nTPin) - offSetT;
  if(startMeasureL == true && counterL<21) //warunek logiczny rozpoaczynajacy pobieranie pomiarow naciskow z lewego pedalu
 {
    lPedal[counterL] = analogRead(nLPin);
    if(counterL ==20) 
    {
     maksLPedal =  findMax(lPedal);
      counterL = 0;
      startMeasureL = false;
      
    }
    counterL++;
 }

 if(startMeasureP == true && counterP<21) //warunek logiczny rozpoaczynajacy pobieranie pomiarow naciskow z prawego pedalu
 {
    pPedal[counterP] = analogRead(nPPin);
    if(counterP ==20) 
    {
     maksPPedal =  findMax(pPedal);
      counterP = 0;
      startMeasureP = false;
    }
    counterP++;
 }
 
 if(millis()>timer)
 {
  timer = millis()+1000;
  //sprawdzac czy ktos siedzi na rowerze poprzez zczytanie nacisku pod kolami wartosci powinny byc mniejsze niz 50 plus wyliczyc offset jezeli ktos siedzi i wysylac tylko pomiar rozniacy sie od offsetu
  sendData(maksPPedal,maksLPedal,naciskPrz,naciskT, mlx.readObjectTempC(), mlx.readAmbientTempC(),nrInterrupt,timeLapK,timeLapL,timeLapP,wykresEKG,puls);
 
 }
}



void sendData(volatile float &naciskPedalPrawy,volatile float &naciskPedalLewy,volatile float &naciskKoloPrzod,volatile float &naciskKoloTyl,double tempKoloTyl,double tempOtoczenia,volatile int &tyl,volatile unsigned int &czasTrwania,volatile unsigned int &czasObrotuKorbaLewa,volatile unsigned int &czasObrotuKorbaPrawa,volatile unsigned int &wykresEKG,volatile unsigned int &puls)
{
  timer = millis()+1000;
  Serial.print("{\"naciskPedalPrawy\": ");
  Serial.print(naciskPedalPrawy);
  Serial.print(",");
  Serial.print("\"naciskPedalLewy\": ");
  Serial.print(naciskPedalPrawy);
  Serial.print(",");
  Serial.print("\"naciskKoloPrzod\": ");
  Serial.print(naciskKoloPrzod);
  Serial.print(",");
  Serial.print("\"naciskKoloTyl\": ");
  Serial.print(naciskKoloTyl);
  Serial.print(",");
  Serial.print("\"tempKoloTyl\": ");
  Serial.print(tempKoloTyl);
  Serial.print(",");
  Serial.print("\"tempOtoczenia\": ");
  Serial.print(tempOtoczenia);
  Serial.print(",");
  Serial.print("\"obrotyKolo\":{");  //sprawdzic skladnie
  Serial.print("\"tyl\": ");
  Serial.print(tyl);
  Serial.print(",");
  Serial.print("\"czasTrwania\": ");
  Serial.print(czasTrwania);
  Serial.print("},");
  Serial.print("\"czasObrotuKorbaLewa\": ");
  Serial.print(czasObrotuKorbaLewa);
  Serial.print(",");
  Serial.print("\"czasObrotuKorbaPrawa\": ");
  Serial.print(czasObrotuKorbaPrawa);
  Serial.print(",");
   Serial.print("\"EKG\":{");  //sprawdzic skladnie
  Serial.print("\"wykresEKG\": ");
  Serial.print(wykresEKG);
  Serial.print(",");
  Serial.print("\"puls\": ");
  Serial.print(puls);
  Serial.print("},");
  Serial.print("\"czas\": ");
  Serial.print(millis());
  Serial.print("}");
Serial.println();
  
  //naciskPedalPrawy++;
  //naciskPedalLewy++;
  naciskKoloPrzod++;
  naciskKoloTyl++;
 // tempKoloTyl++;
  //tempOtoczenia++;
  tyl++;
  //czasObrotuKorbaLewa++;
  czasObrotuKorbaPrawa++;
  wykresEKG++;
  puls++;
}

float findMax(volatile float *tab)
{
 int  maximum = tab[0];
  for(int i = 0;i<21;i++)
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

