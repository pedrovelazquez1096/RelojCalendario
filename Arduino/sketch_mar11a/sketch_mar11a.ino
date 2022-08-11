#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
//matriz
//Vcc - Vcc
//Gnd - Gnd
//Din - Mosi (Pin 11)
//Cs  - SS (Pin 10)
//Clk - Sck (Pin 13)
//memoria
//Vcc - 5V
//GND - GND
//SDA - A4 pin 5
//SCL - A5 pin 6

const int eepromAddress = 0x50;
const int wait = 100; // In milliseconds
const int spacer = 1;
const int width = 5 + spacer; // The fon width is 5 pixels
const int pinCS = 10;
const int numberOfHorizontalDisplays = 1;
const int numberOfVerticalDisplays = 1;

String tape;  //text
uint8_t second, minute, hour, wday, day, month, year;


Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);


void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, byte data) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page(int deviceaddress, unsigned int eeaddresspage, byte* data, byte length) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  byte c;
  for (c = 0; c < length; c++)
    Wire.write(data[c]);
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

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer(int deviceaddress, unsigned int eeaddress, byte *buffer, int length) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, length);
  int c = 0;
  for (c = 0; c < length; c++)
    if (Wire.available()) buffer[c] = Wire.read();
}

void imprimirEnPantalla() {
  for (int i = 0; i < width * tape.length() + matrix.width() - 1 - spacer; i++) {
    matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    while (x + width - spacer >= 0 && letter >= 0) {
      if (letter < tape.length())
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);

      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display

    delay(wait);
  }
}

void print_time() {
  Serial.print("Fecha: ");
  Serial.print(day);
  Serial.print('/');
  Serial.print(month);
  Serial.print('/');
  Serial.print(year);

  Serial.print("  Hora: ");
  Serial.print(hour);
  Serial.print(':');
  Serial.print(minute);
  Serial.print(':');
  Serial.print(second);

  Serial.println();
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
  year = bcd2bin(Wire.read());

  // Recibir los datos del registro de control en la dirección 0x07
  uint8_t ctrl = Wire.read();

  // Operacion satisfactoria, retornamos verdadero
  return true;
}

void getTime() {
  if (read_ds1307())
    print_time();
  else
    Serial.println("No se detecta el DS1307, revisar conexiones");
}

void setup() {
  Wire.begin();
  Serial.begin(9600);

  char somedata[] = "Esto es una frase";

  i2c_eeprom_write_page(eepromAddress, 0, (byte *)somedata, sizeof(somedata));

  delay(10);

  Serial.println(F("Escritura completada"));

  matrix.setIntensity(1); // Use a value between 0 and 15 for brightness

  // Adjust to your own needs
  matrix.setPosition(0, 0, 0); // The first display is at <0, 0>

  matrix.setRotation(0, 0);    // Display is position upside down
}

void loop() {
  int addr = 0;
  byte value = i2c_eeprom_read_byte(eepromAddress, addr);
  tape = "";
  while (value != 0) {
    tape = tape + String((char)value);
    addr++;
    value = i2c_eeprom_read_byte(0x50, addr);
  }
  Serial.println("Imprimiendo lo leido en la matriz");
  Serial.println(tape);
  getTime();
  tape = "";
  tape = "Fecha:" + String(day) + "/" + String(month) + "/20" + String(year) + " Hora: " + String(hour) + ":" + String(minute) + ":" + String(second);
  imprimirEnPantalla();
}
