#include <esp_now.h>
#include <WiFi.h>

// MAC del Receptor
uint8_t broadcastAddress[] = {0x78, 0xE3, 0x6D, 0x18, 0xB7, 0xD0};

// Define variables to store BME280 readings to be sent
int T_enviar=2;

// Define variables to store incoming readings
int R_recibir=0;

//Almacena manesaje de estado de transmision
String success;

//Estructura de datos
typedef struct struct_message {
  int X;
} struct_message;

// Create a struct_message para enviar
struct_message Enviar;

// Create a struct_message para recibir
struct_message Recibir;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&Recibir, incomingData, sizeof(Recibir));
  Serial.print("Bytes received: ");
  Serial.println(len);
  R_recibir=Recibir.X;
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // Set values to send
  Enviar.X=T_enviar;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Enviar, sizeof(Enviar));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  Serial.println(R_recibir);
  Serial.println(T_enviar);
  delay(2000);
}
