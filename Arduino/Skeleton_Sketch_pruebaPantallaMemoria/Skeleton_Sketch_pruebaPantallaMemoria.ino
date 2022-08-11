#include <SPI.h>
#include <Wire.h>
#include <TimerOne.h>
#include "epd2in9.h"
#include "epdpaint.h"

#define sizeImage 4736

//variables globales
unsigned long currentImage = 0;
byte bytein, espaciosMemoria = 0;
bool fechaHora = false, imagen = false, imageLeftInside = false;

//variables RTC
int anio = 2000;
uint8_t second, minute, hour, wday, day, month, year;
byte fechayhora[6];

//variables EEPROM
int mult = 0;
long index = 0;
unsigned char buff[16];
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
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();


}

void loop() {
  if (Serial.available()) {

    if (imagen) {
      Serial.readBytes(buff, 16);
      Serial.write(guardaBuffMemoria());
    }

    if (fechaHora) {
      Serial.readBytes(fechayhora, 6);
      actualizaRTC();
      fechaHora = false;
    }
    
    if (fechaHora == false && imagen == false) {
      bytein = Serial.read();
      if (bytein == 'r') {
        fechaHora = true;
        imagen = false;
        Serial.write('k');
      } else if (bytein == 'i') {
        fechaHora = false;
        imagen = true;
      } else if (bytein == 'm') {
        fechaHora = false;
        imagen = false;
        Serial.write(imagenesPorEnviar());
      } else if (bytein == 'k') {
        Serial.write('k');
      } else if (bytein == 'p'){
        imprimeSiguienteImagenEnPantalla();
      }
    }
  }
}

byte guardaBuffMemoria() {
  if (buscaEspacioMemoria() != 6) {
    for (index = mult * sizeof(buff) + buscaEspacioMemoria() * sizeImage; index < sizeof(buff) * (mult + 1) + buscaEspacioMemoria() * sizeImage; index++) { //ver Formulas.xlsx para ver como funciona esto
      i2c_eeprom_write_byte(eepromAddress, index, buff[index - (16 * mult + buscaEspacioMemoria()*sizeImage)]);
      delay(5);
    }
    mult++;

    if (mult == 296) {
      return 'e';
      mult = 0;
      index = 0;
      actualizaEspaciosMemoria(buscaEspacioMemoria(), 1);
      imagen = false;
    }
    else
      return 'k';
  } else
    return 'e';
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

byte imagenesPorEnviar() {
  byte contador = 0;
  for (byte i = 0; i < 6; i++)
    if (bitRead(espaciosMemoria, i) == 0)
      contador++;
  return contador;
}

void imprimeSiguienteImagenEnPantalla() {
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
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
  second = fechayhora[0];
  minute = fechayhora[1];
  hour = fechayhora[2];
  day = fechayhora[3];
  month = fechayhora[4];
  year = fechayhora[5];
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
