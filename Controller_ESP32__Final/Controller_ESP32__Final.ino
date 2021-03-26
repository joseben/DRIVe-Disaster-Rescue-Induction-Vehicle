#include <ESP8266WiFi.h>
#include <espnow.h>

#define MUX_A D4
#define MUX_B D3
#define MUX_C D2
#define ANALOG_INPUT A0
#define CHANNEL 4

extern "C" {
}
uint8_t remoteMac[] = {0x24, 0x6F, 0x28, 0xB6, 0x24, 0x49};

struct __attribute__((packed)) DataStruct {
    //char text[32];
    int x;
    int y;
    unsigned long time;
};

DataStruct myData;

unsigned long lastSentMillis;
unsigned long sendIntervalMillis = 1000;
unsigned long sentMicros;
unsigned long ackMicros;
int xAxis;
int yAxis;
int zAxis;

void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init()==0) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}


/*void sendData() {
  if (millis() - lastSentMillis >= sendIntervalMillis) {
        lastSentMillis += sendIntervalMillis;
        myData.time = millis();
        uint8_t bs[sizeof(myData)];
        memcpy(bs, &myData, sizeof(myData));
        sentMicros = micros();
        esp_now_send(NULL, bs, sizeof(myData)); // NULL means send to all peers
        Serial.println("sent data");
        Serial.println(myData.x);
        Serial.println(myData.y);
    }
}*/
void sendData() {
  if (millis() - lastSentMillis >= sendIntervalMillis) {
        lastSentMillis += sendIntervalMillis;
        myData.time = millis();
        esp_now_send(NULL, (uint8_t *)&myData, sizeof(myData)); // NULL means send to all peers
        Serial.println("sent data");
        Serial.println(myData.x);
        Serial.println(myData.y);
    }
}

void sendCallBackFunction(uint8_t* mac, uint8_t sendStatus) {
    ackMicros = micros();
    Serial.print("Trip micros "); Serial.println(ackMicros - sentMicros);
    Serial.printf("Send status = %i", sendStatus);
    Serial.println();
    Serial.println();
}
void setup() {
    Serial.begin(115200); Serial.println();
    Serial.println("Starting EspnowController.ino");

    WiFi.mode(WIFI_STA); // Station mode for esp-now controller
    WiFi.disconnect();

    Serial.printf("This mac: %s, ", WiFi.macAddress().c_str());
    Serial.printf("slave mac: %02x%02x%02x%02x%02x%02x", remoteMac[0], remoteMac[1], remoteMac[2], remoteMac[3], remoteMac[4], remoteMac[5]);

    Serial.printf(", channel: %i\n",CHANNEL);
    InitESPNow();
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(remoteMac, ESP_NOW_ROLE_SLAVE, CHANNEL, NULL, 0);

    esp_now_register_send_cb(sendCallBackFunction);

    Serial.print("Message "); 
    
    pinMode(MUX_A, OUTPUT);
    pinMode(MUX_B, OUTPUT);     
    pinMode(MUX_C, OUTPUT);
    Serial.println("Setup finished");

    }
    
void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}
void loop() {
 
    changeMux(LOW, LOW, LOW);
    xAxis = analogRead(ANALOG_INPUT); //Value of the sensor connected to  pin 0 of IC
    changeMux(LOW, LOW, HIGH);
    yAxis = analogRead(ANALOG_INPUT); //Value of the sensor connected to  pin 1 of IC
    changeMux(LOW, HIGH, LOW);
    zAxis = analogRead(ANALOG_INPUT); //Value of the sensor connected to  pin 2 of IC
    changeMux(LOW, HIGH, LOW);
    
    myData.x= xAxis;
    myData.y= yAxis;
    sendData();
    delay(500);
}


















 
