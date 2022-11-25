//Include radiohead ASK library

#include <RH_ASK.h>
#include <SPI.h> //RH lib depends on SPI lib even if not directly used
#include "DHT.h"

//Create ASK object
RH_ASK rf_driver;
// ^ by creating ASK object, we can use special fxns found in the RH ASK lib

#define DHTPIN 8
#define DHTTYPE DHT11
//create dht object
DHT dht(DHTPIN, DHTTYPE);

float f = 0;
unsigned int switchCount=0;
unsigned int switchVar=0;
bool turnON = HIGH;
bool turnOFF = LOW;


void setup()
{
  //initialize ASK object
  rf_driver.init();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
   if (!rf_driver.init()){
         Serial.println("init failed");
}
dht.begin();

}

void loop(){
  
  switchVar = switchCount%2;
  
      Serial.println("**************************************");
  switch(switchVar) {
    
    case 0:
    switchCount = 1;
      setLED(LED_BUILTIN,turnOFF);
        float t = dht.readTemperature();
  //we prepare a msg to tx by storing a string in a character pointer named msg
  char buffer1[18];
  dtostrf(t,6,2,buffer1);
  char bufferTemp[12] = "Temp:";
Serial.println();
//  Serial.print("Entered case 0: Temp case: ");
//  Serial.print(buffer1);
//  Serial.println();
  strcat(bufferTemp, buffer1);
Serial.print("Msg to transmit (Temp.): ");
Serial.print(bufferTemp);
  //const char *msg = h;
  //Note: string msg should not exceed 27 characters for better performance
  //Note: You need to count the # of characters in tx'd msg (rxr needs this #)
  char *msg = bufferTemp;

 //the msg is tx'd using .send() fxn
            //.send() has 2 parameters: array of data, #ofbytes (length of data)
  rf_driver.send((uint8_t *)msg, strlen(msg));

  //send() fxn is usually followed by this waitPacketSent() fxn which waits until
  //any prev. tx packet is finished being tx'd
  rf_driver.waitPacketSent();
  setLED(LED_BUILTIN,turnON);
    delay(400);
  //break;
  
    case 1:
    switchCount = 0;
      setLED(LED_BUILTIN,turnOFF);
          float h = dht.readHumidity();
  char buffer2[10];
     dtostrf(h, 6, 2, buffer2);
     char bufferHumidity[18] = "Humd:";
     strcat(bufferHumidity, buffer2);
     char *msg2 = bufferHumidity;
          Serial.println();
//     Serial.print("Entered case 1: Humidity case: ");
//     Serial.print(buffer2);
//          Serial.println();
  Serial.print("Msg to transmit (Humidity): ");
  Serial.print(bufferHumidity);
  Serial.println();
     rf_driver.send((uint8_t *)msg2, strlen(msg2));
       //send() fxn is usually followed by this waitPacketSent() fxn which waits until
  //any prev. tx packet is finished being tx'd
  rf_driver.waitPacketSent();
  setLED(LED_BUILTIN,turnON);
      delay(400);
  break;
  }
  delay(125);
}

void setLED(int pin, bool setReset){
  digitalWrite(pin, setReset);  
}
