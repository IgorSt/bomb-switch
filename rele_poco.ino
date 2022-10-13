#include "RTClib.h"
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;

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

//RTC_DS3231 rtc;
RTC_Millis rtc;

char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int RELE = 27;
int LED = 2;
int start_operation = 9;
int end_operation = 16;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup () {
    Serial.begin(57600);
    pinMode(RELE, OUTPUT);
    pinMode(LED, OUTPUT);

    lcd.init();                    
    lcd.backlight();

    #ifndef ESP8266
        while (!Serial);
    #endif

    //for rtc 3231
    //rtc.begin();
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    rtc.begin(DateTime(F(__DATE__), F(__TIME__)));

    showInfos();
}

void loop () {
    DateTime now = rtc.now();

    setupLCD(now);

    //only work when delete delay
    char key_pressed = keyboard.getKey();
    if (key_pressed) {
      Serial.println(key_pressed);
    }
    
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

    delay(600000); //10 minutes
}

void setupLCD(DateTime now) {
  lcd.clear();


  lcd.setCursor(0, 0);
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());
  lcd.print(" ");
  lcd.print(" ");
  lcd.print(" ");
  lcd.print(analogRead(33) * 3.3 / 4096);
  
  lcd.setCursor(0,1);
  lcd.print(now.day());
  lcd.print("/");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());
}

void showInfos() {
  DateTime now = rtc.now();

  // Show time
  Serial.print("Now is: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.print(now.second());
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
