#include <Arduino.h>
#include <SPI.h>
#include "epd_3in.h"
#include "cover_image.png.h"

// ESP32-S3 SPI defaults: MOSI 11, MISO 13, SCK 12
EPD display(/* CS */ 16, /* DC */ 8, /* RESET */ 18, /* BUSY */ 17);

void setup() {
  Serial.begin(115200);
  display.init();
  Serial.println(F("Init done"));

}

void loop() {
  display.init();
  display.display(cover_image_png);

  delay(10000); // Set a minimum time between refreshes

  Serial.println("Press any key to clear...");
  while(!Serial.available()) {delay(5);}
  while(Serial.available()) {Serial.read();}

  display.init();
  display.clear(EPD::WHITE);

  delay(10000);

  Serial.println("Press any key to draw bitmap...");
  while(!Serial.available()) {delay(5);}
  while(Serial.available()) {Serial.read();}
}
