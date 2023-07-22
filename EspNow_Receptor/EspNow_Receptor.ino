//Bibliotecas
#include "esp_now.h"
#include "WiFi.h"

//Estructura para recibir los mensajes
typedef struct struct_message {
  int id;
  int x;
  int y;
} struct_message;

//Creamos una variable para almacenar los mensajes recibidos
struct_message myData;

//Creamos una estructura para cada placa
struct_message board1;
struct_message board2;
struct_message board3;

//Creamos una matriz que contenga todas las estructuras de las placas
struct_message boardsStruct[3] = {board1, board2, board3};

//Funcion de devolucion de llamada - OnDataSent()
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  //Obtenemos la direccion MAC de la placa transmisora
  char macStr[18];
  Serial.print("Paquete recibido de: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);

  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Numero de placa: %u: %u bytes\n", myData.id, len);
  //Actualice las estructuras con los nuevos datos entrantes
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  Serial.printf("Valor de x: %d \n", boardsStruct[myData.id-1].x);
  Serial.printf("Valor de y:: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
}

void setup() {
  //Inicializamos el Monitor Serie
  Serial.begin(115200);

  //Configuracion como Estacion WiFi
  WiFi.mode(WIFI_STA);

  //Inicializamos el protocolo ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error de inicializacion del protocolo ESP-NOW");
    return;
  }

  //Registramos la funcion de devolucion de llamada
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
