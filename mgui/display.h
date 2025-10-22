#ifndef DISPLAY_H
#define DISPLAY_H

/*
    Update to be compatible with ESP32!
    The display pinout needs to be updated according to the ESP32 configuration.
*/

#include <Arduino.h>
#include <cstdint>
#include <SPI.h>
#include "tiles.h"

// Display pinout for ESP32 (change according to your setup):
/*
  1 /CS   -> GPIO5 (Chip Select Pin)
  2 /RST  -> GPIO16 (Reset Pin)
  3 A0    -> GPIO17 (Data/Command Pin)
  4 CLK   -> VSPI SCK (GPIO18 or your preferred SPI clock pin)
  5 MOSI  -> VSPI MOSI (GPIO23 or your preferred SPI MOSI pin)
  6 3v3  
  7 GND  
*/

namespace Display{
  // Function Declarations:
  void writeData(uint8_t data);
  void writeCmd(uint8_t data);
  void init();
  void print(String text, uint8_t x, uint8_t y, int s);
  void clearLine(uint8_t y);
  void clearDisplay();
  void stampBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t x_pos, uint8_t y_pos);
  void tile(const uint8_t *tile, uint8_t offset, uint8_t width);
  void space(uint8_t width);

  // In-line functions for cursor management:
  inline void cursorX(uint8_t x)
  {
    writeCmd(0x10 | (x >> 4)); // Send high nibble
    writeCmd(0x00 | (x & 0x0F)); // Send low nibble
  }

  inline void cursorY(uint8_t y)
  {
    writeCmd(0xB0 | y);  // Set page (vertical positioning)
  }
}

#endif
