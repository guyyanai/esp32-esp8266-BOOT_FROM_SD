/*
The SD boot (in this case) works automatically in the setup void (therefore right after the code starts).
Happy Coding,
Guy Yanai.
If there is any problems with the code don't hesitate opening an error on github.
*/
#include <Arduino.h>
#include "SD_Boot.h"
#define SD_CS 5

String fileName = "YOUR_UPDATE_FILE_NAME.bin";

SD_Boot boot = SD_Boot(SD_CS);
void updateStatus(int status) {
   if (status == 1) {
      Serial.println("Hurray! update was successful");
   }else{
      Serial.println("Bad Luck, there was an error, maybe try again");
   }
}

void setup() {
   Serial.begin(115200);
   Serial.println("Welcome to the SD-Update example!");
   digitalWrite(2, LOW);
   delay(100);
   digitalWrite(2, HIGH);
   delay(100);
   digitalWrite(2, LOW);
   delay(100);
   digitalWrite(2, HIGH);
   boot.update(fileName, updateStatus);
   digitalWrite(2, LOW);    
}

void loop() {
}
