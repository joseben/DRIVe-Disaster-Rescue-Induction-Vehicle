/**
   ESPNOW - Basic communication - Slave
   Date: 26th September 2017
   Author: Arvind Ravulavaru <https://github.com/arvindr21>
   Purpose: ESPNow Communication between a Master ESP32 and a Slave ESP32
   Description: This sketch consists of the code for the Slave module.
   Resources: (A bit outdated)
   a. https://espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
   b. http://www.esploradores.com/practica-6-conexion-esp-now/

   << This Device Slave >>

   Flow: Master
   Step 1 : ESPNow Init on Master and set it in STA mode
   Step 2 : Start scanning for Slave ESP32 (we have added a prefix of `slave` to the SSID of slave for an easy setup)
   Step 3 : Once found, add Slave as peer
   Step 4 : Register for send callback
   Step 5 : Start Transmitting data from Master to Slave

   Flow: Slave
   Step 1 : ESPNow Init on Slave
   Step 2 : Update the SSID of Slave with a prefix of `slave`
   Step 3 : Set Slave in AP mode
   Step 4 : Register for receive callback and wait for data
   Step 5 : Once data arrives, print it in the serial monitor

   Note: Master and Slave have been defined to easily understand the setup.
         Based on the ESPNOW API, there is no concept of Master and Slave.
         Any devices can act as master or salve.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define CHANNEL 4

uint8_t mac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};

struct __attribute__((packed)) DataStruct {
    //char text[32];
    int x;
    int y;
    unsigned long time;
};

DataStruct myData;
// Init ESP Now with fallback


void setup() {
  Serial.begin(115200);
  Serial.println("ESPNow/Basic/Slave Example");
  //Set device in AP mode to begin with
  // This is the mac address of the Slave in AP Mode
 WiFi.mode(WIFI_AP);
     // configure device AP mode
     // This is the mac address of the Slave in AP Mode
     esp_wifi_set_mac(ESP_IF_WIFI_STA, &mac[0]);


     Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
     // Init ESPNow with a fallback logic
     if (esp_now_init()!=0)
      {
          Serial.println("*** ESP_Now init failed");
          while(true) {};
       }

       // Once ESPNow is successfully Init, we will register for recv CB to
      // get recv packer info.
       esp_now_register_recv_cb(OnDataRecv);
      Serial.print("Aheloiioi");
}

// callback when data is recv from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  memcpy(&myData, data, sizeof(myData));
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println(myData.x); Serial.println(myData.y);
  Serial.println("");
}

void loop() {
  // Chill 
 
}
