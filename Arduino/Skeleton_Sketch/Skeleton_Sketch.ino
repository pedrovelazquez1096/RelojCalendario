#include <SPI.h>
#include <Wire.h>
#include <TimerOne.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "RTClib.h"

#define sizeImage 4736

//variables globales
byte bytein;
unsigned char buff[16];
//unsigned char llave[16] = {'P','e','d','r','o','E','d','u','a','r','d','o','V','e','G','o'};
unsigned long currentImage = 0;
bool connectionActive = false;

//variables RTC
RTC_DS1307 RTC;
int anio = 2000;
uint8_t second, minute, hour, wday, day, month, year;

//variables EEPROM
int mult = 0;
long index = 0;
byte ultimaImagen = 0;
const int eepromAddress = 0x50;
uint8_t memoryArray[54] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53};
//ultimaImagen esta en 255798
//memoryArray esta de 255744 a 255797

//variables pantalla
Epd epd;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  if (epd.Init(lut_full_update) != 0) {
    Serial.print("e-Paper init failed");
    return;
  }

  connectionActive = true; //Esta variable debe de ser desactivada por el timer despues de terminar la comunicacion

  limpiaDisplay();
  limpiaDisplay();
  imagenCarga1();
}

void loop() {
  if (Serial.available()) {
    Serial.readBytes(buff, 16);
    if (buff[0] == 'P' && buff[1] == 'e' && buff[2] == 'd' &&
        buff[3] == 'r' && buff[4] == 'o' && buff[5] == 'E' &&
        buff[6] == 'd' && buff[7] == 'u' && buff[8] == 'a' &&
        buff[9] == 'r' && buff[10] == 'd' && buff[11] == 'o' &&
        buff[12] == 'V' && buff[13] == 'e' && buff[14] == 'G' && buff[15] == 'o') {
      limpiaDisplay();
      imagenCarga1();
      Serial.write('k');
    } else if (buff[6] == 'F' && buff[7] == 'e' && buff[8] == 'c' &&
               buff[9] == 'h' && buff[10] == 'a' && buff[11] == 'H' &&
               buff[12] == 'o' && buff[13] == 'r' && buff[14] == 'a' &&
               buff[15] == 'P' && buff[5] < 80) {
      actualizaRTC();
      limpiaDisplay();
      imagenCarga2();
      Serial.write(imagenesPorEnviar());
    } else if (buff[0] == 'P' && buff[1] == 'e' && buff[2] == 'd' &&
               buff[3] == 'r' && buff[4] == 'o' && buff[5] == 'E' &&
               buff[6] == 'd' && buff[7] == 'u' && buff[8] == 'a' &&
               buff[9] == 'r' && buff[10] == 'd' && buff[11] == 'o' &&
               buff[12] == 'G' && buff[13] == 'o' && buff[14] == 'V' && buff[15] == 'e') {
      imprimeSiguienteImagenEnPantalla();
    } else {
      Serial.write(guardaBuffMemoria());
    }
  }
}

byte guardaBuffMemoria() {
  byte temp = buscaEspacioMemoria();
  if (temp < 54) {
    for (index = mult * sizeof(buff) + temp * sizeImage; index < sizeof(buff) * (mult + 1) + temp * sizeImage; index++) { //ver Formulas.xlsx para ver como funciona esto
      i2c_eeprom_write_byte(eepromAddress, index, buff[index - (16 * mult + temp * sizeImage)]);
    }
    mult++;
    if (mult > 295) {
      Serial.print('e');
      mult = 0;
      index = 0;
      actualizaEspaciosMemoria(temp, 1);
    }
    else
      Serial.print('u');
  }
}

void imprimeSiguienteImagenEnPantalla() {
  limpiaDisplay();
  limpiaDisplay();

  ultimaImagen = buscaSiguienteImagen();
  if (ultimaImagen < 54) {
    currentImage = ultimaImagen * sizeImage;

    epd.ReadEEPROMSetFrameMemory(currentImage, eepromAddress);
    epd.DisplayFrame();
    epd.ReadEEPROMSetFrameMemory(currentImage, eepromAddress);
    epd.DisplayFrame();

    actualizaEspaciosMemoria(currentImage / 4736, 0);
    guardaUltimaImagen();
  }
}

void actualizaEspaciosMemoria(byte localidad, byte estado) {  //
  bitWrite(memoryArray[localidad], 7, estado);
  writeMemoryArray();
}

byte buscaEspacioMemoria() {                      //Regresa la primera localidad en memoria lista para ser reescrita
  readMemoryArray();
  byte i = 0;
  bool retorno = false;
  i = buscaSiguienteImagen();

  while (bitRead(memoryArray[i], 7) == 1) {
    i++;
    if (i == 54 && retorno == false) {
      i = 0;
      retorno = true;
    } if (i == 54 && retorno == true) {
      break;
    }
  }

  if (bitRead(memoryArray[i], 7) == 0 && i < 54)
    return (memoryArray[i] & 0x7F);
  else
    return 54;
}

byte buscaSiguienteImagen() {                     //Busca la siguiente imagen disponible en memoria
  readMemoryArray();
  leeUltimaImagen();
  for (byte i = ultimaImagen + 1; i < sizeof(memoryArray); i++)
    if (bitRead(memoryArray[i], 7) == 1)
      return (memoryArray[i] & 0x7F);
    else
      for (byte i = 0; i < ultimaImagen; i++)
        if (bitRead(memoryArray[i], 7) == 1)
          return (memoryArray[i] & 0x7F);
        else
          return 54;
}

byte imagenesPorEnviar() {                        //Regresa la cantidad de imagenes que faltan en memoria, indicado por el MSB si es 1 no leido si es 0 leida
  byte contador = 0;
  readMemoryArray();
  for (byte i = 0; i < sizeof(memoryArray); i++)
    if (bitRead(memoryArray[i], 7) == 0)
      contador++;
  return contador;
}

void guardaUltimaImagen() {
  i2c_eeprom_write_byte(eepromAddress, 255798, ultimaImagen);
}

void leeUltimaImagen() {
  ultimaImagen = i2c_eeprom_read_byte(eepromAddress, 255798);
}

void readMemoryArray() {
  for (long i = 255744; i < sizeof(memoryArray); i++)
    memoryArray[i - 255744] = i2c_eeprom_read_byte(eepromAddress, i);
}

void writeMemoryArray() {
  for (long i = 255744; i < sizeof(memoryArray); i++)
    i2c_eeprom_write_byte(eepromAddress, i, memoryArray[i - 255744]);
}

void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, byte data) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  delay(4);
  Wire.endTransmission();
}

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

void actualizaRTC() {
  RTC.adjust(DateTime(buff[5] + 2000, buff[4], buff[3], buff[2], buff[1], buff[0])); //año mes dia hora minutos segundos
}

void leeRTC() {
  DateTime now = RTC.now();

  second = now.second();
  minute = now.minute();
  hour = now.hour();
  day = now.day();
  month = now.month();
  year = now.year() - 2000;
}

void imagenCarga1() {
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
}

void imagenCarga2() {
  epd.SetFrameMemory(IMAGE_DATA2);
  epd.DisplayFrame();
}

void limpiaDisplay() {
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
}
