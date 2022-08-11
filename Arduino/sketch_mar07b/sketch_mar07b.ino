#include "LedControl.h"     // incluye libreria LedControl

LedControl lc = LedControl(11, 13, 10, 1); // DIN CLK CS Id device

#define demora 250      // constante demora con valor de 250

byte flecha_arriba_1[8] = {   // array con primer cuadro de animacion de flecha
  B00000000,
  B00011000,
  B00111100,
  B01111110,
  B11011011,
  B00011000,
  B00011000,
  B00011000
};

byte flecha_vertical[8] = {
  B00001000,
  B00011000,
  B00110000,
  B01111111,
  B01111111,
  B00110000,
  B00011000,
  B00001000
};

void setup() {
  lc.shutdown(0, false);    // enciende la matriz
  lc.setIntensity(0, 1);    // establece brillo
  lc.clearDisplay(0);     // blanquea matriz
}

void loop() {
  for(byte j = 0; j < 8; j++){
    mostrar_1(j);
    delay(100);
  }
  lc.clearDisplay(0);
}

void mostrar_1(byte j) {    // funcion mostrar_1
  for (byte i = 0; i < 8; i++) {
    lc.setColumn(0, i, flecha_vertical[i]<<j);
  }
}
