#include "Wire.h"
#include "SPI.h"
#include "RTClib.h"
#include <SD.h>
#define status_car 6
#define control_car 7
File myFile;
const int chipSelect = 4;

RTC_DS3231 RTC;

void setup () {
  pinMode(8,INPUT);
  pinMode(9,OUTPUT);
  pinMode(status_car, INPUT);
  pinMode(control_car, OUTPUT);
  
//  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.print("Initializing SD card...");
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  DateTime now = RTC.now();

}
String timerAlert = "";
int cout = 0;
int car_status = 0 ;
int last_status = 0;
String timer="";
int toggle_car = 0;
void loop () {
  if(digitalRead(8) == HIGH){
    if(cout == 0){
      toggle_car = HIGH;
      cout =1;
    }
  DateTime now = RTC.now();
  timer = (String) now.year() + '/' + (String)now.month() + '/' + (String)now.day() + '/' + ' ' + (String)now.hour() + ':' + (String) now.minute() + ':' + (String)now.second();
  Serial.println(timer);
  car_status = digitalRead(status_car);
  if (car_status != last_status) {
    if (car_status == HIGH) { // HIGH รถ หยุด
      String recg = timer + ' ' + "Stop";
      toggle_car = LOW;
       myFile = SD.open("test1.txt", FILE_WRITE);
      if (myFile) {
        myFile.println(recg);
        myFile.close();
        Serial.println("done.");
      } else {
        Serial.println("error opening test.txt");
      }
      timerAlert = ( now.minute() >= 55 ) ?   (String) now.year() + '/' + (String)now.month() + '/' + (String)now.day() + '/' + ' ' + (String)(now.hour() + 1) + ':' + (String) ((now.minute() + 5) % 60) + ':' + (String)now.second() : (String) now.year() + '/' + (String)now.month() + '/' + (String)now.day() + '/' + ' ' + (String)now.hour() + ':' + (String) (now.minute() + 5) + ':' + (String)now.second();
    }
  }
  last_status = car_status;
  if (timerAlert.equals(timer)) {
    String recg = timer + ' ' + "Walk";    
      myFile = SD.open("DataCar.txt", FILE_WRITE);
    if (myFile) {
      myFile.println(recg);
      myFile.close();
      Serial.println("done.");
    } else {
      Serial.println("error opening test.txt");
    }
    toggle_car = HIGH; // สั่ง รถ เคลื่อน
  }
  }
  digitalWrite(control_car, toggle_car);
//  digitalWrite(LED_BUILTIN,HIGH);
  digitalWrite(9,HIGH);
  delay(100);

  
}
