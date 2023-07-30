#include <ESP8266WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();
  delay(3000);
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){
  delay(3000);
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
