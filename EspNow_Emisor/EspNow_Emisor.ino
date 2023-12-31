//Bibliotecas
#include "esp_now.h"
#include "WiFi.h"

//Direcion MAC del receptor
uint8_t broadcastAddress[] = {0xFC, 0xF5, 0xC4, 0x98, 0xA4, 0xBD};

//Estructura del mensaje a enviar
typedef struct struct_message {
  String d;
} struct_message;

//Creamos una variable para almacenar los valores de los mensajes
struct_message myData;

//Funcion de devolucion de llamada - OnDataSent()
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nEstado del ultimo paquete enviado\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Envio exitoso" : "Envio fallido");
}

void setup() {
  // Iniciamos Monitor Serie 
  Serial.begin(115200);

  //Configuracion como Estacion WiFi
  WiFi.mode(WIFI_STA);

  //Inicializamos el protocolo ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error de inicializacion del protocolo ESP-NOW");
    return;
  }

  //Registramos la funcion de devolucion de llamada
  esp_now_register_send_cb(OnDataSent);

  //Agregar dispositivo del mismo nivel
    //Para enviar datos a otra placa (el receptor), debe emparejarlo como un par.
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
    // Agregamos un par
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("No se puedo agregar el par");
    return;
  }
}

void loop() {
  delay(2000);
  //Asignamos los valores a las variables
    myData.d = "Hello";
    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}
