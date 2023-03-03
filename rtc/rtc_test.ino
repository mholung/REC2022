#include "rf.h"
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
File myFile;

uint8_t inter = 0;
int counter = 0;
int file_counter = 2;

void setup() {
  Serial.begin(9600);
  init_rtc(2);
  update_rtc();
  adxlInit();
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  myFile = SD.open("test.txt", FILE_WRITE);
  if(!myFile)
  {
    exit(1);
  }
}

void loop() {
  if(inter == 1)
  {
    String msg = string_date();
    int16_t x,y,z;
    adxlGetXYZ(&x,&y,&z);
    msg += " x:";
    msg += String(x);
    msg += " y:";
    msg += String(y);
    msg += " z:";
    msg += String(z);
    myFile.println(msg);
    Serial.println(msg);
    inter = 0;
    counter++;
    if(counter == 10)
    {
      counter = 0;
      myFile.close();
      String file = "test";
      file += file_counter;
      file += ".txt";
      myFile = SD.open(file, FILE_WRITE);
      file_counter++;
    }
  }

}

void interrupt()
{
  inter = 1;
}