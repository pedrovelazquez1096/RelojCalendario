/*
   Modified by Navaneeth Krishnan, For Elementz Engineers Guild Pvt Ltd
*/
#include <SPI.h>
#include <Wire.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"

#define COLORED     1
#define UNCOLORED   0
const int eepromAddress = 0x50;
unsigned int i = 0;
unsigned char contador = 0;
unsigned int tamanio = 4736;
//unsigned char image[1024];
//Paint paint(image, 0, 0);
Epd epd;

byte i2c_eeprom_read_byte(int deviceaddress, unsigned int eeaddress) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(INPUT, 3);
  Serial.begin(9600);
  if (epd.Init(lut_full_update) != 0) {
    Serial.print("e-Paper init failed");
    return;
  }
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  //epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  //epd.DisplayFrame();

  //epd.SetFrameMemory(IMAGE_DATA);
  //epd.DisplayFrame();
  //epd.SetFrameMemory(IMAGE_DATA);
  //epd.DisplayFrame();
  
    paint.SetRotate(ROTATE_90);
    paint.SetWidth(20);
    paint.SetHeight(30);

    paint.Clear(COLORED);
    paint.DrawStringAt(0, 0, "DEMO", &Font12, UNCOLORED);
    epd.SetFrameMemory(paint.GetImage(), 75, 250, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame();
}
void loop() {/*
  if (digitalRead(3) == HIGH) {
    for (i = tamanio * contador; i < tamanio * (contador + 1); i++) {
      IMAGE_DATA[i - tamanio * contador] = i2c_eeprom_read_byte(eepromAddress, i);
    }
    epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    epd.DisplayFrame();
    epd.SetFrameMemory(IMAGE_DATA);
    epd.DisplayFrame();
    contador++;
    if(contador > 2)
      contador = 0;
  }*/
}
