#include <SPI.h>
#include <Wire.h>
#include <TimerOne.h>
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

//variables memoria
const int memAddr = 0x50;
int anio = 2000, loadTime = 5;
unsigned int diaAdd = 240, mesAdd = 239, contadorCumpleAdd = 237, ultimaCancionAdd = 238;
byte diaDato, mesDato, contadorCumpleDato, ultimaCancionDato = 0, siguienteCancionDato;

//variables matriz
const int spacer = 1;
const int width = 5 + spacer; // The fon width is 5 pixels
const int pinCS = 10;
const int numberOfHorizontalDisplays = 1;
const int numberOfVerticalDisplays = 1;
int wait = 100;
String tape;

//variables RTC
uint8_t second, minute, hour, wday, day, month, year;
byte fechayhora[6];
byte bytein[2];
bool holis = false;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

void setup() {
  Wire.begin();
  Serial.begin(9600);
  matrix.setIntensity(1); // Use a value between 0 and 15 for brightness
  matrix.setPosition(0, 0, 0); // The first display is at <0, 0>
  matrix.setRotation(0, 0);    // Display is position upside down

  Timer1.initialize(500000);
  Timer1.attachInterrupt(resetVariables);

  wday = 1;
  /*i2c_eeprom_write_byte(memAddr, diaAdd, 13); //dia
    delay(loadTime);
    i2c_eeprom_write_byte(memAddr, mesAdd, 3); //mes
    delay(loadTime);
    i2c_eeprom_write_byte(memAddr, contadorCumpleAdd, 0); //contador de cumpleaños
    delay(loadTime);
    i2c_eeprom_write_byte(memAddr, ultimaCancionAdd, 0); //ultima cancion
    delay(loadTime);

    byte data2[] = "Youre Beautiful James Blunt-Just Another Girl The Killers-I Try Macy Gray-Pay No Mind Madeon-She Will Be Loved Maroon 5-Perfect Ed Sheeran-Instant Crush Daft Punk-Que Vida la Mia Reik-No te Apartes de mi Vicentico-Marry You Bruno Mars-+";
    for (int i = 0; i < sizeof(data2); i++) {
    i2c_eeprom_write_byte(memAddr, i, data2[i]);
    delay(loadTime);
    }

    Serial.println("Escritura terminada");
    Serial.println("Lectura de prueba");
    Serial.print("Dia ");
    Serial.print(i2c_eeprom_read_byte(memAddr,diaAdd));
    Serial.print(" Mes ");
    Serial.print(i2c_eeprom_read_byte(memAddr,mesAdd));
    Serial.print(" Contador de cumple ");
    Serial.print(i2c_eeprom_read_byte(memAddr,contadorCumpleAdd));
    Serial.print(" addr de la ultima cancion ");
    Serial.println(i2c_eeprom_read_byte(memAddr,ultimaCancionAdd));
    int i = 0;
    char value = i2c_eeprom_read_byte(memAddr, i);
    tape = "";
    do {
    if (value == '-') {
      Serial.println(tape);
      tape = "";
    } else
      tape = tape + value;
    i++;
    value = i2c_eeprom_read_byte(memAddr, i);
    } while (value != '+');
    Serial.print(i);
    Serial.println("  lectura finalizada");
    while (true);*/
}

void loop() {
  if (Serial.available()) {
    if (!holis) {
      Serial.readBytes(bytein, 2);
      if (bytein[0] == 107 && bytein[1] == 107) {
        Serial.write('k');
        holis = true;
      }
    } else {
      Serial.readBytes(fechayhora, 6);
      second = fechayhora[0];
      minute = fechayhora[1];
      hour = fechayhora[2];
      day = fechayhora[3];
      month = fechayhora[4];
      year = fechayhora[5];
      write_ds1307();
    }
  }

  /*simulador de tiempo
    delay(500);
    if (day == 15)
    day = 11;
    else
    day++;
  */
}

void resetVariables() {
  holis = false;
  imprimirFecha();
  if (getTime()) {
    leerDatosImportantes();
    if (checaFecha() && contadorCumpleDato < 10) {
      Serial.print("Si: ");
      llamaLaCancion();
      Serial.println(tape);
    } else
      Serial.println("No");
  }
}

bool getTime() {
  return read_ds1307();
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

void imprimirFecha() {
  Serial.print("Fecha: ");
  Serial.print(day);
  Serial.print("/");
  Serial.print(month);
  Serial.print("/");
  Serial.print(year);
  Serial.print("   ");
}

void leerDatosImportantes() {
  diaDato = i2c_eeprom_read_byte(memAddr, diaAdd);
  mesDato = i2c_eeprom_read_byte(memAddr, mesAdd);
  Serial.print(diaDato);
  Serial.print("/");
  Serial.print(mesDato);
  Serial.print(" ");
}

bool checaFecha() {
  if (day == diaDato && month == mesDato)
    return true;
  else
    return false;
}

void actualizaDatosImportantes(byte addr) {
  contadorCumpleDato++;
  i2c_eeprom_write_byte(memAddr, 237, contadorCumpleDato); //contador de cumpleaños
  delay(10);
  i2c_eeprom_write_byte(memAddr, 238, addr); //ultima cancion
  delay(10);
}

void llamaLaCancion() {
  contadorCumpleDato = i2c_eeprom_read_byte(memAddr, contadorCumpleAdd);
  ultimaCancionDato = i2c_eeprom_read_byte(memAddr, ultimaCancionAdd);
  Serial.print(" Contador cumple: ");
  Serial.print(contadorCumpleDato);
  Serial.print(" UltimaCancion ");
  Serial.print(ultimaCancionDato);
  Serial.print(" ultiCanOld ");
  char value = i2c_eeprom_read_byte(memAddr, ultimaCancionDato);
  tape = "";
  while (value != '-') {
    tape = tape + value;
    ultimaCancionDato++;
    value = i2c_eeprom_read_byte(memAddr, ultimaCancionDato);
  }
  Serial.print(ultimaCancionDato);
  Serial.print(" ");
  ultimaCancionDato += 1;
  Serial.print(" ultiCanNew ");
  Serial.print(ultimaCancionDato);
  Serial.print(" ");
  actualizaDatosImportantes(ultimaCancionDato);
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
