#include "rf.h"
#include <RTClib.h>
#include <RH_ASK.h>
#include <SPI.h>

int inter = 0;

void setup() {
  Serial.begin(9600);
  init_rtc(2);
  update_rtc();
}

void loop() {
  if(inter == 1)
  {
    print_date();
    inter = 0;
  }
}

void interrupt()
{
  inter = 1;
}