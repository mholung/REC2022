#include <SPI.h>
#include <SD.h>
File myFile;

void setup() {
  // put your setup code here, to run once:
  // Deleting
   Serial.begin(9600);
   while (!Serial) {
                    ; // wait for serial port to connect. Needed for native USB port only
                    }
   Serial.println("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("Initialization done.");
  myFile = SD.open("test.txt");

 
Serial.println("Removing test.txt...");
  SD.remove("test.txt"); // remove is a function in SD library to delete a file
  if (SD.exists("test.txt")) { //If the file still exist display message exist
    Serial.println("The test.txt exists.");
  } else { //If the file was successfully deleted display message that the file was already deleted.
    Serial.println("The test.txt doesn't exist.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
