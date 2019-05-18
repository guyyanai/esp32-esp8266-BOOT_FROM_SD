#include <SD_Boot.h>

SD_Boot::SD_Boot(int SD_CS, void (*completion)(int status)) {
    Serial.println("Initalizing SD");
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

    if (!SD.begin(SD_CS)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;
    }

    Serial.println("SD Card initalized");
    _completion = completion;
}

void SD_Boot::update(String fileName, void (*completion)(int status)) {
    File firmware = SD.open("/" + fileName);
    Serial.println("Verifiyng update file " + fileName);
    if(!firmware) {
        Serial.println("File doens't exist");
        _completion(0);
    }else{
        Serial.println("Found firmware");
        _updateFromFS(SD, fileName);
    }
    firmware.close();
}

void SD_Boot::_updateFromFS(fs::FS &fs, String fileName) {
    File updateBin = fs.open("/" + fileName);
    if (updateBin) {
        if(updateBin.isDirectory()){
            Serial.println("Error, " + fileName + " is not a file");
            updateBin.close();
            _completion(0);
            return;
        }

    size_t updateSize = updateBin.size();

    if (updateSize > 0) {
        Serial.println("Try to start update");
        _performUpdate(updateBin, updateSize);
    }else {
        _completion(0);
        Serial.println("Error, file is empty");
    }

    updateBin.close();
    _completion(1);
    _rebootEspWithReason("finished update");   
    }else{
        Serial.println("Could not load " + fileName + " from sd root");
        _completion(0);
    }
}

void SD_Boot::_rebootEspWithReason(String reason) {
    Serial.println(reason);
    delay(1000);
    ESP.restart();
}

void SD_Boot::_performUpdate(Stream &updateSource, size_t updateSize) {
    if (Update.begin(updateSize)) {      
        size_t written = Update.writeStream(updateSource);
        if (written == updateSize) {
            Serial.println("Written : " + String(written) + " successfully");
        }else{
            Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
            _completion(0);
        } 
        if (Update.end()) {
            Serial.println("OTA done!");
            if (Update.isFinished()) {
                Serial.println("Update successfully completed. Rebooting.");
            }else{
                Serial.println("Update not finished? Something went wrong!");
                _completion(0);
            }
        }else {
            Serial.println("Error Occurred. Error #: " + String(Update.getError()));
            _completion(0);
        }
    }else{
        Serial.println("Not enough space to begin OTA");
        _completion(0);
    }
}
