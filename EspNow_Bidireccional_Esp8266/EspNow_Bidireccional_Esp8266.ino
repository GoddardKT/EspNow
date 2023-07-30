#include <ESP8266WiFi.h>
#include <espnow.h>

//Mac receptor
uint8_t broadcastAddress[] = {0x0C, 0xB8, 0x15, 0xD7, 0x22, 0xB0};

//Variables internad para enviar y recibir
int T_enviar=2;
int R_recibir=0;

//Variable para almacenar si el envío de datos fue exitoso
String success;

//Estructura de valoriables a usar en el protocolo
typedef struct struct_message {
  int X;
} struct_message;

//Variable sde tipo struct_message para enviar por protocolo ESPNow
struct_message Enviar;

//Variable sde tipo struct_message para recibir por protocolo ESPNow
struct_message Recibir;


//Funcion para enviar por protocolo ESPNow
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Estado de envio de ultimo paquete: ");
  if (sendStatus == 0){
    Serial.println("Paquete enviado");
  }
  else{
    Serial.println("Paquete no enviado");
  }
}

//Funcion para recibir
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&Recibir, incomingData, sizeof(Recibir));
  Serial.print("Bytes received: ");
  Serial.println(len);
  R_recibir=Recibir.X;
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Inicia ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
    Enviar.X=T_enviar;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &Enviar, sizeof(Enviar));

    // Print incoming readings
    Serial.println(R_recibir);
    Serial.println(T_enviar);
    delay(2000);
}
