#include <Wire.h>
#include "imagedata.h"
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
  Wire.begin();
  Serial.begin(9600);

  /*i2c_eeprom_write_byte(eepromAddress, diaAdd, 13); //dia
  delay(loadTime);
  i2c_eeprom_write_byte(eepromAddress, mesAdd, 3); //mes
  delay(loadTime);
  i2c_eeprom_write_byte(eepromAddress, contadorCumpleAdd, 0); //contador de cumpleaños
  delay(loadTime);
  i2c_eeprom_write_byte(eepromAddress, ultimaCancionAdd, 0); //ultima cancion
  delay(loadTime);

  byte data2[] = "Youre Beautiful James Blunt-Just Another Girl The Killers-I Try Macy Gray-Pay No Mind Madeon-She Will Be Loved Maroon 5-Perfect Ed Sheeran-Instant Crush Daft Punk-Que Vida la Mia Reik-No te Apartes de mi Vicentico-Marry You Bruno Mars-+";
  for (int i = 0; i < sizeof(data2); i++) {
    i2c_eeprom_write_byte(eepromAddress, i, data2[i]);
    delay(loadTime);
  }

  Serial.println("Escritura terminada");
  Serial.println("Lectura de prueba");
  Serial.print("Dia ");
  Serial.print(i2c_eeprom_read_byte(eepromAddress,diaAdd));
  Serial.print(" Mes ");
  Serial.print(i2c_eeprom_read_byte(eepromAddress,mesAdd));
  Serial.print(" Contador de cumple ");
  Serial.print(i2c_eeprom_read_byte(eepromAddress,contadorCumpleAdd));
  Serial.print(" addr de la ultima cancion ");
  Serial.println(i2c_eeprom_read_byte(eepromAddress,ultimaCancionAdd));
  int i = 0;
  char value = i2c_eeprom_read_byte(eepromAddress, i);
  tape = "";
  do {
    if (value == '-') {
      Serial.println(tape);
      tape = "";
    } else
      tape = tape + value;
    i++;
    value = i2c_eeprom_read_byte(eepromAddress, i);
  } while (value != '+');
  Serial.print(i);
  Serial.println("  lectura finalizada");
  while (true);*/
}

void imprimirFecha() {
  Serial.print("Fecha: ");
  Serial.print(dia);
  Serial.print("/");
  Serial.print(mes);
  Serial.print("/");
  Serial.print(anio);
  Serial.print("   ");
}

void leerDatosImportantes() {
  diaDato = i2c_eeprom_read_byte(eepromAddress, diaAdd);
  mesDato = i2c_eeprom_read_byte(eepromAddress, mesAdd);
  Serial.print(diaDato);
  Serial.print("/");
  Serial.print(mesDato);
  Serial.print(" ");
}

bool checaFecha() {
  if (dia == diaDato && mes == mesDato)
    return true;
  else
    return false;
}

void actualizaDatosImportantes(byte addr) {
  contadorCumpleDato++;
  i2c_eeprom_write_byte(eepromAddress, 237, contadorCumpleDato); //contador de cumpleaños
  delay(10);
  i2c_eeprom_write_byte(eepromAddress, 238, addr); //ultima cancion
  delay(10);
}

void llamaLaCancion() {
  contadorCumpleDato = i2c_eeprom_read_byte(eepromAddress, contadorCumpleAdd);
  ultimaCancionDato = i2c_eeprom_read_byte(eepromAddress, ultimaCancionAdd);
  Serial.print(" Contador cumple: ");
  Serial.print(contadorCumpleDato);
  Serial.print(" UltimaCancion ");
  Serial.print(ultimaCancionDato);
  Serial.print(" ultiCanOld ");
  char value = i2c_eeprom_read_byte(eepromAddress, ultimaCancionDato);
  tape = "";
  while (value != '-') {
    tape = tape + value;
    ultimaCancionDato++;
    value = i2c_eeprom_read_byte(eepromAddress, ultimaCancionDato);
  }
  Serial.print(ultimaCancionDato);
  Serial.print(" ");
  ultimaCancionDato += 1;
  Serial.print(" ultiCanNew ");
  Serial.print(ultimaCancionDato);
  Serial.print(" ");
  actualizaDatosImportantes(ultimaCancionDato);
}

void loop() {
  imprimirFecha();
  leerDatosImportantes();
  if (checaFecha() && contadorCumpleDato < 10) {
    Serial.print("Si: ");
    llamaLaCancion();
    Serial.println(tape);
  } else
    Serial.println("No");


  delay(500);
  if (dia == 15)
    dia = 11;
  else
    dia++;
}
