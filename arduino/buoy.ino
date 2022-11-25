#include <Wire.h> //I2C
#include <RTClib.h>
#include <RH_ASK.h>
#include <SPI.h>
#include "buoy.h"

//adxl raw sensor readouts 16-bit signed values
int16_t adxlXYZ[3];
//driver for RF
RH_ASK driver;
RTC_DS3231 rtc;

//I2C
//writes a word to a given register address for a specified hardware dev (I2C)
void i2cWrite(uint8_t dev, uint8_t address, uint8_t word) {
  Wire.beginTransmission(dev);  // start transmission to I2C device and take control of bus
  Wire.write(address); // send register address
  Wire.write(word); // send data/command to write
  Wire.endTransmission(); //frees I2C bus
}

//reads num words starting from pointed address from dev to provivded buffer (I2C)
//Warning: Assumes size of buffer >= num
void i2cRead(uint8_t dev, uint8_t address, uint8_t num, uint8_t buffer[])
{
  Wire.beginTransmission(dev);  // start transmission to I2C device
  Wire.write(address);          // send register address
  Wire.endTransmission();
  Wire.requestFrom(dev, num);  // request for read from pointed address

  uint8_t i = 0;
  while (Wire.available())  //reads from wire buffer byte by byte until empty
  {
    buffer[i++] = Wire.read();
  }
}

//ADXL345
//startup sequence for ADXL345
void adxlInit()
{
  i2cWrite(ADXL_ADD, DATA_FORMAT, 0x0B); //full res, full range
  i2cWrite(ADXL_ADD, POWER_CTL, 0x00); //resets register
  i2cWrite(ADXL_ADD, POWER_CTL, 0x08); //starts measurement
}

//get signed 16-bit raw acceleration measurements for each axis
void adxlGetXYZ(int16_t* x, int16_t* y, int16_t* z)
{
  uint8_t xyz[6];
  i2cRead(ADXL_ADD, DATAX0, 6, xyz);
  *x = (xyz[1] << 8) + (xyz[0]);
  *y = (xyz[3] << 8) + (xyz[2]);
  *z = (xyz[5] << 8) + (xyz[4]);
}

//prints (and plots) the current global variable values for accelerometer readouts
void adxlPrintXYZ(int16_t x, int16_t y, int16_t z)
{
  Serial.print("x:");
  Serial.print(x);
  Serial.print(' ');
  Serial.print("y:");
  Serial.print(y);
  Serial.print(' ');
  Serial.print("z:");
  Serial.print(z);
  Serial.println("");
}

//set calibration offsets for each axis signed 8-bit ints
void adxlSetOffset(int8_t x, int8_t y, int8_t z) 
{
  i2cWrite(ADXL_ADD, OFSX, x);
  i2cWrite(ADXL_ADD, OFSY, y);
  i2cWrite(ADXL_ADD, OFSZ, z);
}

//read calibration offsets
void adxlGetOffset(int8_t* x, int8_t* y, int8_t* z) 
{
  i2cRead(ADXL_ADD, OFSX, 1, x);
  i2cRead(ADXL_ADD, OFSY, 1, y);
  i2cRead(ADXL_ADD, OFSZ, 1, z);
}

//prints the current offsets for ADXL axis xyz in hex format
void adxlPrintOffset()
{
  int8_t offsetXYZ[3];
  adxlGetOffset(&offsetXYZ[0], &offsetXYZ[1], &offsetXYZ[2]);
  Serial.print("x: ");
  Serial.print(offsetXYZ[0], HEX);
  Serial.print(" y: ");
  Serial.print(offsetXYZ[1], HEX);
  Serial.print(" z: ");
  Serial.print(offsetXYZ[2], HEX);
  Serial.println("");
}

//reads and prints ADXL readouts sub 20Hz frequency
void adxlTest()
{
  adxlGetXYZ(&adxlXYZ[0],&adxlXYZ[1],&adxlXYZ[2]);
  adxlPrintXYZ(adxlXYZ[0],adxlXYZ[1],adxlXYZ[2]);
  //delay(50);
}

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
    pinMode(ALARM_PIN, INPUT_PULLUP);
    rtc.writeSqwPinMode(DS3231_OFF);
    rtc.disableAlarm(1);
    rtc.clearAlarm(1);
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

void setup()
{
  Serial.begin(9600);  //Initiates serial communications via USB
  Wire.begin(); //initiates I2C
  adxlInit();
  //adxlSetOffset(0, 0, 0); //sensor offset calibration
  //adxlPrintOffset();
}

void loop() 
{
  adxlTest();
  print_date();
  delay(1000);
}
