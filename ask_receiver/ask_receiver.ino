
#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver;

int LEDpin = 7;
bool turnON = HIGH;
bool turnOFF = LOW;

void setup() {
  rf_driver.init();
  pinMode(LEDpin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);
 if (!rf_driver.init()){
         Serial.println("init failed");
} 
}



void loop() {
  //we create buffer size, same as tx'd msg length, aka 11 for "Hello World"
  uint8_t buf[11]; //var. called buf that holds 11 elements

  uint8_t buflen = sizeof(buf);

if (rf_driver.recv(buf, &buflen))
{
  Serial.print("Message Received: ");
  Serial.println((char*)buf);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  
  if(buf[0] == 'T'){ //only true if tx'd msg is Temp msg
    setLED(LEDpin, turnON);
    setLED(LED_BUILTIN, turnOFF);
  }
  else if(buf[0] == 'H'){
    setLED(LED_BUILTIN, turnON);
    setLED(LEDpin, turnOFF);
  }else Serial.println("Error message: Neither Temp or Humidity transmission");
  //toggleLED(LEDpin);
  //Serial.println(digitalRead(LEDpin));
    }
}
void setLED(int pin, bool setReset){
  digitalWrite(pin, setReset);  
}
