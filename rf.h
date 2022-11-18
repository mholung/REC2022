//Initializes The RF module. Returns 1 if initialized sucessfully, 0 otherwise
int rf_init(void)

// Sends a String Message Over RF (Transmitter should be connected to pin 12)
void send_msg(char* msg)

/* Receives A Message From the RF Module 
(Receiver should be connected to pin 11)
Takes a char array and it's size
Stores the string message in the array and returns the size of the message
Returns -1 if messsage could not be received
*/
int receive_msg(char msg[], int size)

//Initialize RTC. Return 1 if successful, 0 otherwise
int init_rtc(void)

/*Update RTC
Asks User for date to update RTC
year (4 digit), month(1-12), day(1-31), hour(0-23), minute(0-59), second(0-59)
*/
void update_rtc(int year, int month, int day, int hour, int minute, int second)

// Gets current time from RTC in DateTime struct.
DateTime get_time(void)


//Sets Recurring Alarm Interrupt from RTC. The alarm will happen in secs seconds
void set_rtc_alarm(int secs)