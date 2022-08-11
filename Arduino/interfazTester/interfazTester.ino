byte fechayhora[6];
byte bytein[2];
bool holis = false;
void setup() {
  Serial.begin(9600);

}

void loop() {
  if (Serial.available()) {
    if (!holis) {
      Serial.readBytes(bytein, 2);
      if (bytein[0] == 107 && bytein[1] == 107) {
        Serial.write('k');
        holis = true;
      }
    }
    else
      Serial.readBytes(fechayhora,6);
    }
}
