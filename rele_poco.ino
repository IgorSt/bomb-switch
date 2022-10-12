#include "RTClib.h"
#include <Keypad.h>

const byte LINES = 4;
const byte COLUMNS = 4;

char KEYS_MATRIZ[LINES][COLUMNS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte PIN_LINES[LINES] = {18, 19, 21, 22};
byte PIN_COLUMNS[COLUMNS] = {26, 27, 32, 33};

Keypad keyboard = Keypad(
  makeKeymap(KEYS_MATRIZ),
  PIN_LINES,
  PIN_COLUMNS,
  LINES,
  COLUMNS
);

RTC_Millis rtc;

char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int RELE = 27;
int LED = 2;
int start_operation = 9;
int end_operation = 16;

void setup () {
    Serial.begin(57600);
    pinMode(RELE, OUTPUT);
    pinMode(LED, OUTPUT);

#ifndef ESP8266
    while (!Serial);
#endif

    rtc.begin(DateTime(F(__DATE__), F(__TIME__)));

    showInfos();
}

void loop () {
    DateTime now = rtc.now();
    
    int HOUR = now.hour();
    boolean isDay = now.dayOfTheWeek()%2 != 0;
    if (start_operation && HOUR < end_operation) {
      digitalWrite(RELE, HIGH);
    } else {
      digitalWrite(RELE, LOW);
    }

    float voltageBattery = (analogRead(33) * 3.3 / 4096);
    if(voltageBattery < 1.5) {
      digitalWrite(LED, HIGH);
    }

    delay(1200000); //20 minutes

    char key_pressed = keyboard.getKey();
}

void showInfos() {
  DateTime now = rtc.now();

  // Show time
  Serial.print("Now is: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.println();
  Serial.print("Today is: ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print(" - ");
  Serial.print(days[now.dayOfTheWeek()]);
  Serial.print(" - ");
  Serial.print(now.dayOfTheWeek());
  Serial.println();

  //Show Battery voltage
  Serial.print("Battery voltage: ");
  Serial.print(analogRead(33) * 3.3 / 4096);
  Serial.println();
}
