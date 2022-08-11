#include <SPI.h>
#include <Wire.h>
#include <TimerOne.h>
#include "epd2in9.h"
#include "imagedata.h"
#include "RTClib.h"

#define sizeImage 4736
#define lastImageAdd 28422
#define fechaCumple 28423
#define fechaActual 28425
#define fechaExtra  28428

#define intervalTimeCheck 1000

//variables globales
int currentImage = 0;
unsigned char buff[16];
bool connectionActive = false;

//variables timer
byte timerCount = 0;
unsigned long imageLifeTime = 0;

//variables RTC
RTC_DS1307 RTC;
int anio = 2000;
uint8_t second, minute, hour, wday, day, month, year;

//variables EEPROM
int mult = 0;
long index = 0;
byte ultimaImagen = 0;
const int eepromAddress = 0x50;
uint8_t memoryArray[6] = {0, 1, 2, 3, 4, 5};

//variables pantalla
Epd epd;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  if (epd.Init(lut_full_update) != 0) {
    Serial.print("e-Paper init failed");
    return;
  }

  //Timer1.initialize(500000);        //500ms
  //Timer1.attachInterrupt(timeInt);

  connectionActive = true; //Esta variable debe de ser desactivada por el timer despues de terminar la comunicacion
  for (int i = 0; i < sizeof(memoryArray); i++) {
    memoryArray[i] = memoryArray[i] + 128;
  }
  guardaUltimaImagen();
  writeMemoryArray();
}

/*void timeInt(void)
  {
  timerCount++;
  imageLifeTime++;
  if (timerCount > 39) {
    timerCount = 0;
    leeRTC();
    leerFechaExtra(byte localidad);
    if (comparaFecha() == true) {
      if (imageLifeTime > 172800) {

      }
    }
  }
  }*/

void loop() {
  imprimeSiguienteImagenEnPantalla();
  for (int i = 0; i < 10; i++)
    delay(1000);
}

byte guardaBuffMemoria() {
  byte temp = buscaEspacioMemoria();
  if (temp < 6) {
    for (index = mult * sizeof(buff) + temp * sizeImage; index < sizeof(buff) * (mult + 1) + temp * sizeImage; index++) { //ver Formulas.xlsx para ver como funciona esto
      i2c_eeprom_write_byte(eepromAddress, index, buff[index - (16 * mult + temp * sizeImage)]);
    }
    mult++;
    if (mult > 295) {
      index = 0;                            //Despues de mandar e la pc debe responder con
      mult = 0;                             //la fecha en la que se debe desplegar la imagen
      actualizaEspaciosMemoria(temp, 1);
      return 'e';
    }
    else
      return 'u';
  }
}

void imprimeSiguienteImagenEnPantalla() {
  limpiaDisplay();
  ultimaImagen = buscaSiguienteImagen();
  Serial.print("UltimaImagen: ");
  Serial.println(ultimaImagen);
  if (ultimaImagen < 6) {
    currentImage = ultimaImagen * sizeImage;

    epd.ReadEEPROMSetFrameMemory(currentImage, eepromAddress);
    epd.DisplayFrame();
    epd.ReadEEPROMSetFrameMemory(currentImage, eepromAddress);
    epd.DisplayFrame();

    actualizaEspaciosMemoria(ultimaImagen, 0);
    guardaUltimaImagen();
  } else {
    imagenCarga1();
  }
}

void actualizaEspaciosMemoria(byte localidad, byte estado) {  //
  readMemoryArray();
  bitWrite(memoryArray[localidad], 7, estado);
  writeMemoryArray();
}

byte buscaEspacioMemoria() {                      //Regresa la primera localidad en memoria lista para ser reescrita
  readMemoryArray();
  byte i = 0, temp = 0;
  bool retorno = false;
  leeUltimaImagen();
  ultimaImagen++;
  if (ultimaImagen > 5)
    i = 0;
  else
    i = ultimaImagen;
  temp = i;

  while (bitRead(memoryArray[i], 7) == 1) {
    i++;
    if (i == 6 && retorno == false) {
      i = 0;
      retorno = true;
    } if (i == temp && retorno == true) {
      i = 6;
      break;
    }
  }

  if (i < 6)
    if (bitRead(memoryArray[i], 7) == 0)
      return (memoryArray[i] & 0x7F);
    else
      return 6;
  else
    return 6;

}

byte buscaSiguienteImagen() {                     //Busca la siguiente imagen disponible en memoria
  readMemoryArray();
  leeUltimaImagen();
  for (byte i = ultimaImagen; i < sizeof(memoryArray); i++)
    if (bitRead(memoryArray[i], 7) == 1)
      return (memoryArray[i] & 0x7F);

  for (byte i = 0; i < ultimaImagen; i++)
    if (bitRead(memoryArray[i], 7) == 1)
      return (memoryArray[i] & 0x7F);
    else
      return 6;
  return 6;
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
  i2c_eeprom_write_byte(eepromAddress, lastImageAdd, ultimaImagen);
}

void leeUltimaImagen() {
  ultimaImagen = i2c_eeprom_read_byte(eepromAddress, lastImageAdd);
}

void readMemoryArray() {
  for (int i = 6 * sizeImage; i < sizeof(memoryArray) +  6 * sizeImage; i++)
    memoryArray[i -  (6 * sizeImage)] = i2c_eeprom_read_byte(eepromAddress, i);
}

void writeMemoryArray() {
  for (int i =  6 * sizeImage; i < sizeof(memoryArray) + 6 * sizeImage; i++)
    i2c_eeprom_write_byte(eepromAddress, i, memoryArray[i -  (6 * sizeImage)]);
}

void leerFechaExtra(byte localidad) {
  for (int i = fechaExtra + (localidad * 3); i < (fechaExtra + (localidad * 3)) + 2; i++)
    buff[i - (fechaExtra + (localidad * 3))] = i2c_eeprom_read_byte(eepromAddress, i);
}

void guardaFechaExtra() {
  byte temp = buscaEspacioMemoria();
  i2c_eeprom_write_byte(eepromAddress, fechaExtra + (temp * 3), buff[0]);
  i2c_eeprom_write_byte(eepromAddress, fechaExtra + (temp * 3), buff[1]);
  i2c_eeprom_write_byte(eepromAddress, fechaExtra + (temp * 3), buff[2]);
}

bool comparaFecha() {
  if (buff[0] == day && buff[1] == month && buff[2] == year)
    return true;
  else
    return false;
}

void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, byte data) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
  delay(4);
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

void limpiaDisplay() {
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
}
