#include <Wire.h>
//memoria
//Vcc - 5V
//GND - GND
//SDA - A4 pin 5
//SCL - A5 pin 6
const int eepromAddress = 0x50;
String tape = "qqqwweeerrrtttyyyuuuiiiooopppaaasssdddfffggghhhjjjkkklllzzzxxxcccvvvbbbnnnmmm";  //text

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

void setup() {
  Wire.begin();
  Serial.begin(9600);

  char somedata[] = "Esto es una frase-";
  i2c_eeprom_write_page(eepromAddress, 0, (byte *)somedata, sizeof(somedata));
  Serial.println("Escritura terminada");

  Serial.println("Iniciando lectura");
  delay(100);
  int addr = 0;
  char value = i2c_eeprom_read_byte(eepromAddress, addr);
  tape = "";
  while (value != '-') {
    tape = tape + value;
    addr++;
    value = i2c_eeprom_read_byte(0x50, addr);
  }
  
  Serial.print(tape);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
