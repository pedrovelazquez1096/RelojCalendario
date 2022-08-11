#include <SPI.h>
#include <Wire.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"
const int eepromAddress = 0x50;

#define sizeImage 4736
#define lastImageAdd 28422



Epd epd;

byte bytein;
unsigned char buff[16];
long index = 0;
int mult = 0;
uint8_t memoryArray[6] = {0, 1, 2, 3, 4, 5};
int currentImage = 0;
byte ultimaImagen = 0;

char buffers[20] = " ";
char* formato = "dato:%i i:%i";

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Iniciando");
  if (epd.Init(lut_full_update) != 0) {
    Serial.print("e-Paper init failed");
    return;
  }

}

void loop() {
  if (Serial.available()) {
    byte i = Serial.parseInt();
    limpiaDisplay();
    epd.CreateLoadingBar(IMAGE_DATA, i);
    epd.DisplayFrame();
  }
  /*
    if (Serial.available()) {
      switch (Serial.read()) {
        case '1':
          imprimeSiguienteImagenEnPantalla();
          Serial.println("---------------------------------------");
          break;
        case '2':
          guardaBuffMemoria();
          Serial.println("---------------------------------------");
          break;
        case '3':
          Serial.println("Imagenes por enviar: ");
          Serial.println(imagenesPorEnviar());
          Serial.println("---------------------------------------");
          break;
      }
    }*/
}

byte guardaBuffMemoria() {
  Serial.println(">>guardaBuffMemoria()");
  byte temp = buscaEspacioMemoria();
  Serial.print("buscaEspacioMemoria: ");
  Serial.println(temp);
  /*
    if (temp < 6) {
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
  */
  actualizaEspaciosMemoria(temp, 1);
  Serial.println(">>guardaBuffMemoria()");
}

void imprimeSiguienteImagenEnPantalla() {
  //limpiaDisplay();
  Serial.println(">>imprimeSiguienteImagenEnPantalla()");

  ultimaImagen = buscaSiguienteImagen();
  Serial.print("ultimaImagen: ");
  Serial.println(ultimaImagen);
  if (ultimaImagen < 6) {
    currentImage = ultimaImagen * sizeImage;
    Serial.print("currentImage: ");
    Serial.println(currentImage);
    /*
      epd.ReadEEPROMSetFrameMemory(currentImage, eepromAddress);
      epd.DisplayFrame();
      epd.ReadEEPROMSetFrameMemory(currentImage, eepromAddress);
      epd.DisplayFrame();
    */
    actualizaEspaciosMemoria(ultimaImagen, 0);
    guardaUltimaImagen();
  } else {
    Serial.println("Ya no hay imagenes disponibles");
  }
  Serial.println("<<imprimeSiguienteImagenEnPantalla()");
}

void actualizaEspaciosMemoria(byte localidad, byte estado) {
  Serial.println(">>actualizaEspaciosMemoria()");
  readMemoryArray();
  bitWrite(memoryArray[localidad], 7, estado);
  writeMemoryArray();
  Serial.println("<<actualizaEspaciosMemoria()");
}

byte buscaEspacioMemoria() {                      //Regresa la primera localidad en memoria lista para ser reescrita
  Serial.println(">>buscaEspacioMemoria()");
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
  Serial.print("buscaSiguienteImagen apartir de: ");
  Serial.println(i);
  while (bitRead(memoryArray[i], 7) == 1) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(memoryArray[i], BIN);
    i++;
    if (i == 6 && retorno == false) {
      i = 0;
      retorno = true;
      Serial.println("De i a fin no encontre, reinicio i");
    } if (i == temp && retorno == true) {
      Serial.println("De inicio a i no encontre, break, i = 7");
      i = 6;
      break;
    }
  }
  Serial.print("Sali del while i: ");
  Serial.println(i);
  if (i < 6)
    if (bitRead(memoryArray[i], 7) == 0) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(memoryArray[i], BIN);
      Serial.println("<<buscaEspacioMemoria()");
      return (memoryArray[i] & 0x7F);
    }
    else {
      Serial.println("no encontre espacio");
      Serial.println("<<buscaEspacioMemoria()");
      return 6;
    }
  else
  {
    Serial.println("i >= 7");
    Serial.println("<<buscaEspacioMemoria()");
    return 6;
  }
}

byte buscaSiguienteImagen() {                     //Busca la siguiente imagen disponible en memoria
  Serial.println(">>buscaSiguienteImagen()");
  readMemoryArray();
  leeUltimaImagen();
  for (byte i = ultimaImagen; i < sizeof(memoryArray); i++)
    if (bitRead(memoryArray[i], 7) == 1) {
      Serial.print("de ultimaImagen hasta el final: ");
      Serial.println(memoryArray[i] & 0x7F, BIN);
      Serial.println("<<buscaSiguienteImagen()");
      return (memoryArray[i] & 0x7F);
    }

  for (byte i = 0; i < ultimaImagen; i++)
    if (bitRead(memoryArray[i], 7) == 1) {
      Serial.print("del inicio hasta ultimaImagen: ");
      Serial.println(memoryArray[i] & 0x7F, BIN);
      Serial.println("<<buscaSiguienteImagen()");
      return (memoryArray[i] & 0x7F);
    }
    else {
      Serial.println("No encontre siguienteImagen");
      Serial.println("<<buscaSiguienteImagen()");
      return 6;
    }
  return 6;
}

byte imagenesPorEnviar() {                        //Regresa la cantidad de imagenes que faltan en memoria, indicado por el MSB si es 1 no leido si es 0 leida
  Serial.println(">>imagenesPorEnviar()");
  byte contador = 0;
  readMemoryArray();
  for (byte i = 0; i < sizeof(memoryArray); i++)
    if (bitRead(memoryArray[i], 7) == 0)
      contador++;
  Serial.print("Imagenes por enviar: ");
  Serial.println(contador);
  Serial.println("<<imagenesPorEnviar()");
  return contador;
}

void guardaUltimaImagen() {
  Serial.println(">>guardaUltimaImagen()");
  i2c_eeprom_write_byte(eepromAddress, lastImageAdd, ultimaImagen);
  Serial.println("<<guardaUltimaImagen()");
}

void leeUltimaImagen() {
  Serial.println(">>leeUltimaImagen()");
  ultimaImagen = i2c_eeprom_read_byte(eepromAddress, lastImageAdd);
  Serial.print("ultimaImagen: ");
  Serial.println(ultimaImagen);
  Serial.println("<<leeUltimaImagen()");
}

void readMemoryArray() {
  Serial.println(">>readMemoryArray()");
  for (int i = 6 * sizeImage; i < sizeof(memoryArray) +  6 * sizeImage; i++) {
    memoryArray[i -  (6 * sizeImage)] = i2c_eeprom_read_byte(eepromAddress, i);
    Serial.println(memoryArray[i -  (6 * sizeImage)], BIN);
  }
  Serial.println("<<readMemoryArray()");
}

void writeMemoryArray() {
  Serial.println(">>writeMemoryArray()");
  for (int i =  6 * sizeImage; i < sizeof(memoryArray) + 6 * sizeImage; i++)
  {
    Serial.println(memoryArray[i -  (6 * sizeImage)], BIN);
    i2c_eeprom_write_byte(eepromAddress, i, memoryArray[i -  (6 * sizeImage)]);
  }
  Serial.println("<<writeMemoryArray()");
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

void imagenCarga1() {
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
}

void limpiaDisplay() {
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
}
