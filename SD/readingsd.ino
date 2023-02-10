#include <SPI.h>
#include <SD.h>
File myFile;


void setup() {
  // put your setup code here, to run once:
  //Reading sd
  Serial.begin(9600);
                while (!Serial) {
                ; // wait for serial port to connect. Needed for native USB port only
                }
                Serial.print("Initializing SD card...");
                if (!SD.begin(10)) {
                Serial.println("initialization failed!");
                while (1);
                }
                Serial.println("initialization done.");
                // open the file for reading:
                myFile = SD.open("test.txt");
                if (myFile) {
                Serial.println("test.txt:");
                // read from the file until there's nothing else in it:
                while (myFile.available()) {
                Serial.write(myFile.read());
                }
                // close the file:
                myFile.close();
                } else {
                // if the file didn't open, print an error:
                Serial.println("error opening test.txt");
                }

}

void loop() {
  // put your main code here, to run repeatedly:

}
