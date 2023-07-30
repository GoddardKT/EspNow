#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  Serial.println();
  delay(3000);
  Serial.print("La direccion MAC de la placa es: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  delay(3000);
  Serial.print("La direccion MAC de la placa es: ");
  Serial.println(WiFi.macAddress());
}
