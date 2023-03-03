#include <RTClib.h>
#include <RH_ASK.h>
#include <SPI.h>

//CONSTANTS
//ADXL345 Registers
#define OFSX 0x1E
#define OFSY 0x1F
#define OFSZ 0x20
#define BW_RATE 0x2C
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37

//I2C Hardware ID
#define ADXL_ADD 0x53

//FUNCTION DECLARATIONS
//I2C
//Writes a byte word to register at address with unique dev id
void i2cWrite(uint8_t dev, uint8_t address, uint8_t word);
//Reads num bytes starting from pointed address to sufficiently large buffer from unique dev id
void i2cRead(uint8_t dev, uint8_t address, uint8_t num, uint8_t buffer[]);

//ADXL345
//startup routine for ADXL345
void adxlInit();
//gets raw sensor measurements with offset and writes to provided 16-bit x,y,z variables
//Use SF: ~0.004m/s^2 for standard measure
void adxlGetXYZ(int16_t* x, int16_t* y, int16_t* z);
//formats and prints via serial xyz triple for plotting/debugging
void adxlPrintXYZ(int16_t x, int16_t y, int16_t z);
//offsets ADXL axis sensor readouts for calibration
void adxlSetOffset(int8_t x, int8_t y, int8_t z);
//reads currently employed 3-axis offset values and writes to provided 8-bit x,y,z variables
void adxlGetOffset(int8_t* x, int8_t* y, int8_t* z);
//reads and prints offsets currently employed
void adxlPrintOffset();
//reads and prints ADXL measurements at ~20Hz
void adxlTest();

//Initialize RTC. Return 1 if successful, 0 otherwise
//Sets ALARM_PIN to an interrupt connected to a function called "interupt"
int init_rtc(const uint8_t ALARM_PIN);

/*Update RTC
Asks User for date to update RTC
year (4 digit), month(1-12), day(1-31), hour(0-23), minute(0-59), second(0-59)
*/
void update_rtc(void);