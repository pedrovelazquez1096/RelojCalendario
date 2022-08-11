#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
int prueba = 61680;
int msb,lsb;
void setup () {
  Wire.begin(); // Inicia el puerto I2C
  RTC.begin(); // Inicia la comunicaci¢n con el RTC
  RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));//año mes dia hora minutos segundos
  Serial.begin(9600);

  Serial.println(prueba,BIN);

  msb = prueba>>8;
  Serial.println(msb,BIN);
  
  lsb = (int)(prueba & 0xFF);
  Serial.println(lsb,BIN);
}
void loop() {


  /*DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC

    Serial.print(now.year(), DEC); // A§o
    Serial.print('/');
    Serial.print(now.month(), DEC); // Mes
    Serial.print('/');
    Serial.print(now.day(), DEC); // Dia
    Serial.print(' ');
    Serial.print(now.hour(), DEC); // Horas
    Serial.print(':');

    Serial.print(now.minute(), DEC); // Minutos
    Serial.print(':');
    Serial.print(now.second(), DEC); // Segundos
    Serial.println();*/
}
