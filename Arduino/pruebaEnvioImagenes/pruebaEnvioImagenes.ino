#include <SPI.h>
#include <Wire.h>
#include <TimerOne.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"

#define sizeImage 4736

//variables globales
unsigned char buff[16];
//unsigned char llave[16] = {'P','e','d','r','o','E','d','u','a','r','d','o','V','e','G','o'};
unsigned long currentImage = 0;
byte bytein, espaciosMemoria = 0b11000000;
bool fechaHora = false, imagen = false, imageLeftInside = false, connectionActive = false;

//variables RTC
int anio = 2000;
uint8_t second, minute, hour, wday, day, month, year;

//variables EEPROM
int mult = 0;
long index = 0;
const int eepromAddress = 0x50;

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
      //Serial.println("llave");
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
  if (imagenesPorEnviar() != 0) {
    if (mult <= 294) {
      for (index = mult * sizeof(buff) + buscaEspacioMemoria() * sizeImage; index < sizeof(buff) * (mult + 1) + buscaEspacioMemoria() * sizeImage; index++) { //ver Formulas.xlsx para ver como funciona esto
        i2c_eeprom_write_byte(eepromAddress, index, buff[index - (16 * mult + buscaEspacioMemoria()*sizeImage)]);
        delay(5);
      }
      mult++;
      return 'u';
    }
    else {
      mult = 0;
      index = 0;
      actualizaEspaciosMemoria(buscaEspacioMemoria(), 1);
      return 'e';
    }
  } else {
    return 'k';
  }
}

void actualizaEspaciosMemoria(byte localidad, byte estado) {
  bitWrite(espaciosMemoria, localidad, estado);
}

byte buscaEspacioMemoria() {//Regresa la primera localidad en memoria lista para ser reescrita, si no hay espacio regresa 6
  byte i = 0;
  if (imagenesPorEnviar() != 0)
    while (i < 6) {
      if (bitRead(espaciosMemoria, i) == 0) {
        break;
      }
      else
        i++;
    }
  else
    i = 6;
  return i;
}

byte imagenesPorEnviar() {    //Regresa la cantidad de imagenes que faltan en memoria
  byte contador = 0;
  for (byte i = 0; i < 6; i++)
    if (bitRead(espaciosMemoria, i) == 0)
      contador++;
  return contador;
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

byte buscaSiguienteImagen() {
  byte i = 0;
  while (i < 6) {
    if (bitRead(espaciosMemoria, i) == 1) {
      break;
    }
    else
      i++;
  }
  return i;
}

void imprimeSiguienteImagenEnPantalla() {
  limpiaDisplay();
  limpiaDisplay();
  if (buscaSiguienteImagen() < 6) {
    currentImage = buscaSiguienteImagen() * sizeImage * 8;
    epd.ReadEEPROMSetFrameMemory(currentImage, eepromAddress);
    epd.DisplayFrame();
    epd.ReadEEPROMSetFrameMemory(currentImage, eepromAddress);
    epd.DisplayFrame();
    actualizaEspaciosMemoria(currentImage / 37888, 0);
    if (currentImage == 189440)
      currentImage = 0;
    else
      currentImage = currentImage + 37888;
  }
}

void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, byte data) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
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
  second = buff[0];
  minute = buff[1];
  hour = buff[2];
  day = buff[3];
  month = buff[4];
  year = buff[5];
  write_ds1307();
}

uint8_t bcd2bin(uint8_t bcd) {
  // Convertir decenas y luego unidades a un numero binario
  return (bcd / 16 * 10) + (bcd % 16);
}

uint8_t bin2bcd(uint8_t bin) {
  return (bin / 10 * 16) + (bin % 10);
}

bool write_ds1307() {
  // Iniciar el intercambio de información con el DS1307 (0x68)
  Wire.beginTransmission(0x68);

  // Escribir la dirección del registro segundero
  Wire.write(0x00);

  // Escribir valores en los registros, nos aseguramos que el bit clock halt
  // en el registro del segundero este desactivado (esto hace que el reloj funcione)
  Wire.write(bin2bcd(second & 0x7F)); // <--- Esto hace que el reloj comience a trabajar
  Wire.write(bin2bcd(minute));
  Wire.write(bin2bcd(hour));
  Wire.write(bin2bcd(wday));
  Wire.write(bin2bcd(day));
  Wire.write(bin2bcd(month));
  Wire.write(bin2bcd(year));

  // Terminamos la escritura y verificamos si el DS1307 respondio
  // Si la escritura se llevo a cabo el metodo endTransmission retorna 0
  if (Wire.endTransmission() != 0)
    return false;

  // Retornar verdadero si se escribio con exito
  return true;
}

bool read_ds1307() {
  // Iniciar el intercambio de información con el DS1307 (0xD0)
  Wire.beginTransmission(0x68);

  // Escribir la dirección del segundero
  Wire.write(0x00);

  // Terminamos la escritura y verificamos si el DS1307 respondio
  // Si la escritura se llevo a cabo el metodo endTransmission retorna 0
  if (Wire.endTransmission() != 0)
    return false;

  // Si el DS1307 esta presente, comenzar la lectura de 8 bytes
  Wire.requestFrom(0x68, 8);

  // Recibimos el byte del registro 0x00 y lo convertimos a binario
  second = bcd2bin(Wire.read());
  minute = bcd2bin(Wire.read()); // Continuamos recibiendo cada uno de los registros
  hour = bcd2bin(Wire.read());
  wday = bcd2bin(Wire.read());
  day = bcd2bin(Wire.read());
  month = bcd2bin(Wire.read());
  year = bcd2bin(Wire.read()) + anio;

  // Recibir los datos del registro de control en la dirección 0x07
  uint8_t ctrl = Wire.read();

  // Operacion satisfactoria, retornamos verdadero
  return true;
}
