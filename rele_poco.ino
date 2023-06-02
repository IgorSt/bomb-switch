
#include <Wire.h> 
#include <RtcDS1307.h>

RtcDS1307<TwoWire> rtc(Wire);

int RELE = 14;

int start_operation = 8;
int end_operation = 16;

void setup(){
  
  Serial.begin(9600);

  rtc.Begin();

  //RtcDateTime tempoatual = RtcDateTime(__DATE__,__TIME__); 
  //rtc.SetDateTime(tempoatual);

  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, HIGH);
}


void loop(){

  RtcDateTime instante = rtc.GetDateTime();

  // somente dias pares
  //boolean isDay = instante.DayOfWeek() % 2 != 0;
  
  /*
  0 - Dom
  1 - Seg
  2 - Ter
  3 - Qua
  4 - Qui
  5 - Sex
  6 - Sab
  */

  boolean isDay = instante.DayOfWeek() == 1 || instante.DayOfWeek() == 3 || instante.DayOfWeek() == 5;
  if (
    instante.Hour() >= start_operation &&
    instante.Hour() < end_operation &&
    isDay
    ) {
    Serial.println("Rele Ligado");
    digitalWrite(RELE, LOW);
  } else {
    Serial.println("Rele Desligado");
    digitalWrite(RELE, HIGH);
  }

  Serial.print(instante.Day());  
  Serial.print("/");
  Serial.print(instante.Month());
  Serial.print("/");
  Serial.print(instante.Year()); 
  Serial.print(" - ");
  Serial.print(instante.DayOfWeek());  
  Serial.print(" - ");
  Serial.print(instante.Hour());  
  Serial.print(":");
  Serial.print(instante.Minute());
  Serial.print(":");
  Serial.print(instante.Second());
  Serial.println("");

  delay(1000);
}
