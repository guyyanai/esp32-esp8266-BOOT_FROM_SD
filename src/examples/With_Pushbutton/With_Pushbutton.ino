/*
The SD boot (in this case) works only if the you pushes a button right after the code starts and hold it for a ccertian delay, in this case 10 seconds.
Happy Coding,
Guy Yanai.
If there is any problems with the code don't hesitate opening an error on github.
*/
#include <Arduino.h>
#include <SD_Boot.h>
#define SD_CS 5

const int PushButton = 4;
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
      boot.update(fileName, updateStatus);
   }
   digitalWrite(2, LOW);    
}

void loop() {
}
