#include <RTClib.h>
#include <RH_ASK.h>
#include <SPI.h>


//Initializes The RF module. Returns 0 if initialized sucessfully, 1 otherwise
int rf_init(void);

// Sends a String Message Over RF (Transmitter should be connected to pin 12)
void send_msg(char* msg);

/* Receives A Message From the RF Module 
(Receiver should be connected to pin 11)
Takes a char array and it's size
Stores the string message in the array and returns the size of the message
Returns -1 if messsage could not be received
*/
int receive_msg(char msg[], int size);

//Initialize RTC. Return 1 if successful, 0 otherwise
//Sets ALARM_PIN to an interrupt connected to a function called "interupt"
int init_rtc(const int ALARM_PIN);

/*Update RTC
Asks User for date to update RTC
year (4 digit), month(1-12), day(1-31), hour(0-23), minute(0-59), second(0-59)
*/
void update_rtc(void);

// Gets current time from RTC in DateTime struct.
DateTime get_time(void);

void print_date();
