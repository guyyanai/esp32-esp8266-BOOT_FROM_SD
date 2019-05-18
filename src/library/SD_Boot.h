#ifndef SD_BOOT
#define SD_BOOT

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <Update.h>
#include "SD.h"

class SD_Boot {

    public:
        void update(String fileName = "firmware.bin", void (*completion)(int status) = NULL);
        SD_Boot(int SD_CS = 5, void (*completion)(int status) = NULL);
    private:
        void (*_completion)(int status) = NULL;
        void _updateFromFS(fs::FS &fs, String fileName);
        void _rebootEspWithReason(String reason);
        void _performUpdate(Stream &updateSource, size_t updateSize);

};


#endif#ifndef SD_BOOT
#define SD_BOOT

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <Update.h>
#include "SD.h"

class SD_Boot {

    public:
        void update(String fileName = "firmware.bin", void (*completion)(int status) = NULL);
        SD_Boot(int SD_CS = 5, void (*completion)(int status) = NULL);
    private:
        void (*_completion)(int status) = NULL;
        void _updateFromFS(fs::FS &fs, String fileName);
        void _rebootEspWithReason(String reason);
        void _performUpdate(Stream &updateSource, size_t updateSize);

};


#endif
