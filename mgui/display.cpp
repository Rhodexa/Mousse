#include "display.h"
#include <SPI.h>

#define GLCD_CS_PIN   5   // Chip Select Pin for the GLCD (adjust as needed)
#define GLCD_DC_PIN   17  // Data/Command Pin (adjust as needed)
#define GLCD_RST_PIN  16  // Reset Pin (adjust as needed)

// SPI Settings for ESP32
SPIClass spi = SPIClass(VSPI); // Use VSPI bus for ESP32

void Display::writeData(uint8_t data){
  digitalWrite(GLCD_CS_PIN, LOW);    // Select GLCD
  digitalWrite(GLCD_DC_PIN, HIGH);   // Data mode
  spi.transfer(data);                // Transfer data
  digitalWrite(GLCD_CS_PIN, HIGH);   // Deselect GLCD
}

void Display::writeCmd(uint8_t data){
  digitalWrite(GLCD_CS_PIN, LOW);    // Select GLCD
  digitalWrite(GLCD_DC_PIN, LOW);    // Command mode
  spi.transfer(data);                // Transfer command
  digitalWrite(GLCD_CS_PIN, HIGH);   // Deselect GLCD
}

void Display::init()
{
  // Initialize SPI with clock speed, bit order, and SPI mode
  spi.begin();
  spi.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // 1 MHz clock

  pinMode(GLCD_CS_PIN, OUTPUT);
  pinMode(GLCD_DC_PIN, OUTPUT);
  pinMode(GLCD_RST_PIN, OUTPUT);
  
  digitalWrite(GLCD_RST_PIN, LOW);   // Reset the display
  delay(10);
  digitalWrite(GLCD_RST_PIN, HIGH);
  delay(10);
  
  writeCmd(0xE2); // Reset
  delay(10);
  writeCmd(0xA2); // Bias
  writeCmd(0xA1); // Bias
  writeCmd(0xC0); // Out Mode
  writeCmd(0x25); // Voltage Reg
  writeCmd(0x81); // Contrast
  writeCmd(0x17); // Contrast
  writeCmd(0x2F); // Set everything on
  writeCmd(0xA4); // Normal
  writeCmd(0x40); // Start Line
  writeCmd(0xB0); // Start Page
  writeCmd(0x10); // Column Address
  writeCmd(0x00); // Column Address
  writeCmd(0xAF); // Display On

  // Display initialization done, now print the logo (if any)
  for (uint8_t y = 0; y < 8; y++){
    writeCmd(0xB0 | y);
    writeCmd(0x10);
    writeCmd(0x00);
    for(uint8_t x = 0; x < 128; x++){
      writeData(0);
    }
  }
}

void Display::print(String text, uint8_t x, uint8_t y, int s) {
  if (x > 127) return;
  if (y > 7) return;
  writeCmd(0xB0 | y);
  writeCmd(0x10 | (x >> 4));
  writeCmd(0x00 | (x & 0x0F));
  
  for (int i = 0; i < text.length(); i++) {
    for(uint8_t line = 0; line < 6; line++)
    {
      if(s > 0) writeData(Tile::ascii[text[i]*6 + line] >> s);
      else writeData(Tile::ascii[text[i]*6 + line] << (s*-1));
    }
  }
}

void Display::clearLine(uint8_t y){
  if (y > 7) return;
  writeCmd(0xB0 | y);
  writeCmd(0x10);
  writeCmd(0x00);
  for (int i = 0; i < 128; i++) {
    writeData(0);
  }
}

void Display::clearDisplay(){
  for (uint8_t y = 0; y < 8; y++){
    writeCmd(0xB0 | y);
    writeCmd(0x10);
    writeCmd(0x00);
    for(uint8_t x = 0; x < 128; x++){
      writeData(0);
    }
  }
}

void Display::stampBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t x_pos, uint8_t y_pos){
  if (x_pos > 127) return;
  if (y_pos > 7) return;
  
  for (uint8_t y = 0; y < height; y++){
    writeCmd(0xB0 | ((y_pos + height) & 0x0F));
    writeCmd(0x10 | (x_pos >> 4));
    writeCmd(0x00 | (x_pos & 0x0F));
    for(uint8_t x = 0; x < width; x++){
      writeData(bitmap[y*width + x]);
    }
  }
}

void Display::tile(const uint8_t *tile, uint8_t offset, uint8_t width) {
  for(uint8_t x = 0; x < width; x++){
    writeData(tile[x + offset*width]);
  }
}

void Display::space(uint8_t width){
  for (uint8_t i = 0; i < width; i++){
    writeData(0);
  }
}
