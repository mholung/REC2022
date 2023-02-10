#include "rf.h"
#include <RTClib.h>
#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver;
RTC_DS3231 rtc;

int rf_init(void)
{
    if(!driver.init())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void send_msg(char* msg)
{
    driver.send((uint8_t *)msg, strlen(msg) + 1);
    driver.waitPacketSent();
}


int init_rtc(const int ALARM_PIN)
{
  rtc.begin();  
  pinMode(ALARM_PIN, INPUT);
  digitalWrite(ALARM_PIN, HIGH);
  attachInterrupt(digitalPinToInterrupt(ALARM_PIN), interrupt, RISING);
  rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
}

void update_rtc()
{
  const char txt[6][15] = { "year [4-digit]", "month [1~12]", "day [1~31]",
                            "hours [0~23]", "minutes [0~59]", "seconds [0~59]"};
  String str = "";
  long newDate[6];

  while (Serial.available()) {
    Serial.read();
  }
  for (int i = 0; i < 6; i++) {

    Serial.print("Enter ");
    Serial.print(txt[i]);
    Serial.print(": ");

    while (!Serial.available()) {
      ;
    }
    str = Serial.readString();
    newDate[i] = str.toInt();
    Serial.println(newDate[i]);
  }
  rtc.adjust(DateTime(newDate[0], newDate[1], newDate[2], newDate[3], newDate[4], newDate[5]));
}

DateTime get_time(void)
{
    return rtc.now();
}


void print_date()
{
  const char dayInWords[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

  const char monthInWords[13][4] = {" ", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
                                         "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

  DateTime rtcTime = rtc.now();

  int ss = rtcTime.second();
  int mm = rtcTime.minute();
  int hh = rtcTime.twelveHour();
  int DD = rtcTime.dayOfTheWeek();
  int dd = rtcTime.day();

  int MM = rtcTime.month();
  int yyyy = rtcTime.year();
  if (dd < 10) Serial.print("0");
  Serial.print(dd);
  Serial.print("-");
  Serial.print(monthInWords[MM]);
  Serial.print("-");
  Serial.print(yyyy);

  Serial.print("  ");
  Serial.print(dayInWords[DD]);
  Serial.print("  ");


  if (hh < 10) Serial.print("0");
  Serial.print(hh);
  Serial.print(':');

  if (mm < 10) Serial.print("0");
  Serial.print(mm);
  Serial.print(':');

  if (ss < 10) Serial.print("0");
  Serial.print(ss);

  if (rtcTime.isPM()) Serial.println(" PM");
  else Serial.println(" AM");
}