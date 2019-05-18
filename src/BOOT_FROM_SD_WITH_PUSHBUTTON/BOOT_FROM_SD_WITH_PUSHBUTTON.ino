/*
The SD boot (in this case) works only if the you pushes a button right after the code starts and hold it for a ccertian delay, in this case 10 seconds.
Happy Coding,
Guy Yanai.
If there is any problems with the code don't hesitate opening an error on github.
*/
#include <Update.h>
#include <FS.h>
#include "SD.h"
#include <SPI.h>
#include <Arduino.h>

#define SD_CS 5

const int PushButton = 4;
String fileName = "YOUR_FILE_NAME.bin";

void rebootEspWithReason(String reason){
    Serial.println(reason);
    delay(1000);
    ESP.restart();
}

void performUpdate(Stream &updateSource, size_t updateSize) {
   if (Update.begin(updateSize)) {      
      size_t written = Update.writeStream(updateSource);
      if (written == updateSize) {
         Serial.println("Written : " + String(written) + " successfully");
      }
      else {
         Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
      }
      if (Update.end()) {
         Serial.println("OTA done!");
         if (Update.isFinished()) {
            Serial.println("Update successfully completed. Rebooting.");
         }
         else {
            Serial.println("Update not finished? Something went wrong!");
         }
      }
      else {
         Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }

   }
   else
   {
      Serial.println("Not enough space to begin OTA");
   }
}

void updateFromFS(fs::FS &fs) {
   File updateBin = fs.open("/" + fileName);
   if (updateBin) {
      if(updateBin.isDirectory()){
         Serial.println("Error, update.bin is not a file");
         updateBin.close();
         return;
      }

      size_t updateSize = updateBin.size();

      if (updateSize > 0) {
         Serial.println("Try to start update");
         performUpdate(updateBin, updateSize);
      }
      else {
         Serial.println("Error, file is empty");
      }

      updateBin.close();
      rebootEspWithReason("finished update");   
   }
   else {
      Serial.println("Could not load firm.bin from sd root");
   }
}

void setup() {
   Serial.begin(115200);
   Serial.println("Welcome to the SD-Update example!");
    SD.begin(SD_CS);  
    if(!SD.begin(SD_CS)) {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    if(cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }
    Serial.println("Initializing SD card...");
    if (!SD.begin(SD_CS)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;
    }
   pinMode(2, OUTPUT);
   delay(500);
   digitalWrite(2, HIGH);
   delay(10000);
   int Push_button_state = digitalRead(PushButton);
   if (Push_button_state == HIGH) {
      digitalWrite(2, LOW);
      delay(100);
      digitalWrite(2, HIGH);
      delay(100);
      digitalWrite(2, LOW);
      delay(100);
      digitalWrite(2, HIGH);
      File firmware = SD.open("/" + fileName);
      if(!firmware) {
         Serial.println("File doens't exist");
      }else{
         Serial.println("Found firmware");
         updateFromFS(SD);
      }
      firmware.close();
   }
   digitalWrite(2, LOW);    
}

void loop() {
}