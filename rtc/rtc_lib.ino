#include "rf.h"
#include <RTClib.h>
#include <SPI.h>

RTC_DS3231 rtc;

//adxl raw sensor readouts 16-bit signed values
int16_t adxlXYZ[3];

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

int init_rtc(const uint8_t ALARM_PIN)
{
  rtc.begin();  
  pinMode(ALARM_PIN, INPUT);
  digitalWrite(ALARM_PIN, HIGH);
  attachInterrupt(digitalPinToInterrupt(ALARM_PIN), interrupt, RISING);
  rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
}

void update_rtc()
{
  const char txt[6][8] = { "year", "month", "day",
                            "hour", "minute", "seconds"};
  String str = "";
  uint16_t newDate[6];

  while (Serial.available()) {
    Serial.read();
  }
  for (uint8_t i = 0; i < 6; i++) {

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


String string_date()
{
  String time = "";
  DateTime rtc_time = rtc.now();
  time += String(rtc_time.hour());
  time += ":";
  time += String(rtc_time.minute());
  time += ":";
  time += String(rtc_time.second());
  time += " ";
  time += String(rtc_time.day());
  time += "/";
  time += String(rtc_time.month());
  time += "/";
  time += String(rtc_time.year());
  return time;
}