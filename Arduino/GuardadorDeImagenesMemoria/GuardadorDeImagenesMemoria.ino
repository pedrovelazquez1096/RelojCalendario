#include <Wire.h>
//#include "imagedata.h"
//#include <avr/pgmspace.h>
//memoria - arduino
//1-3 Address
//4 GND
//5 SDA - A4
//6 SCL - A5
//7 WP write Protection - GND
//8 VCC
const int eepromAddress = 0x50;
String tape;  //text
byte dia = 11, mes = 3;
int anio = 2020, loadTime = 5;
unsigned int diaAdd = 240, mesAdd = 239, contadorCumpleAdd = 237, ultimaCancionAdd = 238;
byte diaDato, mesDato, contadorCumpleDato, ultimaCancionDato = 0, siguienteCancionDato;

void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, byte data) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}
unsigned char buff[16];
long index = 0;
int mult = 0;

long tiempo;
void setup() {
  Wire.begin();
  Serial.begin(115200);
  /*for (unsigned long i = 0; i < 256000; i++)
  {
    i2c_eeprom_write_byte(eepromAddress, i, 0x00);
    delay(5);
    if (i % 16 == 0) {
      Serial.println(i);
    } else {
      Serial.print(i);
      Serial.print(",");
    }
  }*/
}

void loop() {
    if (Serial.available()) {
      //Serial.read();
      Serial.readBytes(buff, 16);
      for (index = mult * sizeof(buff); index < sizeof(buff) * (mult + 1); index++) {
        i2c_eeprom_write_byte(eepromAddress, index, buff[index - (16 * mult)]);
        delay(5);
      }
      mult++;
      if (mult > 295)
        Serial.print('e');
      else
        Serial.print('k');
    }
}
