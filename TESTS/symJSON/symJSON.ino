#include <Wire.h>
#include <Adafruit_MLX90614.h> //biblioteki potrzebne do obslugi pirometru

//program do generowania przykladowych zmiennych 
//wedlug ustalonego wczesniej json'a plik:: nowe_strukturt.txt

//naciski trzeba przeliczyc aby wynik byl podawany w Newton
bool startMeasure; 
const int lewyPin = A0;
int counter;
volatile float lPedal[20]; // tablcia do przechowywania nacisku na lewy pedal 
volatile float maksLPedal;
volatile unsigned  naciskPedalPrawy = 0;
volatile unsigned int naciskPedalLewy = 100;
//
volatile unsigned int naciskKoloPrzod = 200;
volatile unsigned int naciskKoloTyl = 300;

//temperatura podawana w stopniach celsjusza
//TEMPERATURA TYLNIEGO KOLO + TEMPERATURA OTOCZENIA
volatile unsigned int tempKoloTyl = 400;
volatile unsigned int tempOtoczenia = 500;
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //mlx.readAmbientTempC() - temperatura kolo, mlx.readObjectTempC() - temperatura otoczenia

//TYLNIE KOLO
volatile unsigned int tyl = 600; //liczba obrotow z dokladnoscia do 1/4 obwodu kola
volatile unsigned int czasTrwania = 700; //czas obrotu w milisekundach
//KORBA
volatile unsigned int czasObrotuKorbaLewa = 800; //czas obrotu w milisekundach
volatile unsigned int czasObrotuKorbaPrawa = 900; //czas obrotu w milisekundach
//EKG
volatile unsigned int wykresEKG = 1000;
volatile unsigned int puls = 1100;  //wyznaczany na podstawie ekg przy pomocy funkcji z biblioteki adafruit

unsigned int  timer; //zmienna pomocnicza do wyznaczania jednej sekundy
volatile int maximumLewy; //maksymalna wartosc nacisku na lewy pedal
void setup() {
  Serial.begin(9600);
  mlx.begin();
  timer = millis();
  counter = 1;
}

void loop() {
  delay(10);
  
  if(startMeasure == true && counter<21) //warunek logiczny rozpoaczynajacy pobieranie pomiarow naciskow z lewego pedalu
 {
    lPedal[counter] = analogRead(lewyPin);
    if(counter ==20) 
    {
     maksLPedal =  findMax(lPedal);
      counter = 0;
      startMeasure = false;
      
    }
    counter++;
    
 }
 
 if(millis()>timer)
 {
  timer = millis()+1000;
  
  sendData(naciskPedalPrawy,naciskPedalLewy,naciskKoloPrzod,naciskKoloTyl, mlx.readObjectTempC(), mlx.readAmbientTempC(),tyl,czasTrwania,czasObrotuKorbaLewa,czasObrotuKorbaPrawa,wykresEKG,puls);
 
 }



}



void sendData(volatile unsigned int &naciskPedalPrawy,volatile unsigned int &naciskPedalLewy,volatile unsigned int &naciskKoloPrzod,volatile unsigned int &naciskKoloTyl,double tempKoloTyl,double tempOtoczenia,volatile unsigned int &tyl,volatile unsigned int &czasTrwania,volatile unsigned int &czasObrotuKorbaLewa,volatile unsigned int &czasObrotuKorbaPrawa,volatile unsigned int &wykresEKG,volatile unsigned int &puls)
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
  
  naciskPedalPrawy++;
  naciskPedalLewy++;
  naciskKoloPrzod++;
  naciskKoloTyl++;
 // tempKoloTyl++;
  //tempOtoczenia++;
  tyl++;
  czasObrotuKorbaLewa++;
  czasObrotuKorbaPrawa++;
  wykresEKG++;
  puls++;
}

float findMax(volatile float *tab)
{
  maximumLewy = tab[0];
  for(int i = 0;i<21;i++)
  {
    if(tab[i]>maximumLewy) maximumLewy = tab[i];
  }
  return maximumLewy;
}


