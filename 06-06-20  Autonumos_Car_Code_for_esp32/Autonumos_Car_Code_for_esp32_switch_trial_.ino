


/*Autonomous RC Car
//NodeMCU SETUP:::
///////////////////////////////////////////////////////////////////////////
===========================================================================
GPS               ==2,4
Compass           ==22,21
UltraSonic sensor ==35,34,39,36
Motor Driver      ==27,26,25,33,32,23
GPS Switch        == 18

=========================================================================
/////////////////////////////////////////////////////////////////////////
*/ 
//Library files
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <TinyGPS++.h>          
#include <Wire.h>
#include <analogWrite.h>
#include <Arduino.h>
#include<HardwareSerial.h>
#include <MechaQMC5883.h>



//******************************************************************************************************                                                                  
// GPS Variables & Setup
HardwareSerial S2(2);
static const int RXPin = 2, TXPin = 4;
TinyGPSPlus gps;
int GPS_Course;                                                    // variable to hold the gps's determined course to destination
int Number_of_SATS;                                                // variable to hold the number of satellites acquired

//******************************************************************************************************
// Setup Motor Driver
int turn_Speed = 175;                                              // motor speed when using the compass to turn left and right
int mtr_Spd = 250;                                                 // motor speed when moving forward and reverse

#define CHANNEL 4

uint8_t mac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};

struct __attribute__((packed)) DataStruct {
    //char text[32];
    int x;
    int y;
    unsigned long time;
};

DataStruct myData;



//******************************************************************************************************
// CompaSerial Variables & Setup
MechaQMC5883 qmc;

int desired_heading;                                               // initialize variable - stores value for the new desired heading
int compass_heading;                                               // initialize variable - stores value calculated from compass readings
int compass_dev = 5;                                               // the amount of deviation that is allowed in the compass heading - Adjust as Needed
                                                                   // setting this variable too low will cause the robot to continuously pivot left and right
                                                                   // setting this variable too high will cause the robot to veer off course

int Heading_A;                                                     // variable to store compass heading
int Heading_B;                                                     // variable to store compass heading in Opposite direction
int pass = 0;                                                      // variable to store which paSerial the robot is on

//******************************************************************************************************
// Ping Sensor for Collision Avoidance

boolean pingOn = false;                                            // Turn Collision detection On or Off
int trigPin1 = 35;    // Trigger
int echoPin1 = 34;    // Echo
int trigPin2 = 39;    // Trigger
int echoPin2 = 36;    // Echo
long durationL, cmL;
long durationR, cmR;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;                                  // Store last time Ping was updated
const long interval = 200;                                         // Ping the Distance every X miliseconds

//*****************************************************************************************************
// GPS Locations

unsigned long Distance_To_Home;                                    // variable for storing the distance to destination

double Destination_LAT=0.0;                                          // variable for storing the destination Latitude - Only Programmed for 5 waypoint
double Destination_LON=0.0;                                          // variable for storing the destination Longitude - up to 50 waypoints

int increment = 0;

//*****************************************************************************************************
//Web Pge Server Connection
 String c;
  char cs[37];
 
const char* ssid = "iBall-Baton-1";
const char* password = "*************";

char host[]= "192.168.1.105";

double lati1;
double logi1;
double lati2;
double logi2;

//*****************************************************************************************************
// Extras

#define autopilot 18
 void  gesturecontroll();
 void getGPS();    
 void getCompass();  
 void Forward();
void Forward_Meter();
void Reverse();
void LeftTurn();
void RightTurn();
void SlowLeftTurn();
void SlowRightTurn();
void StopCar();
void setWaypoint();
 void move();
 void Startup();
 void goWaypoint();
 void clearWaypoints();

 int blueToothVal=0;
  
 int flag=0;
 int button;
               WiFiClient client;


void setup() 
{    Serial.begin(115200);                                            // Serial 0 is for communication with the computer
     Serial.println("Welcome");

//Motors////////////////////////////////////////////////////////////////////////////////////
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(23, OUTPUT);
pinMode(autopilot, INPUT);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 button = digitalRead(autopilot);
 if (button == HIGH)
 {   flag=15;
     Serial.println("Manual Control initated");
     Serial.println("ESPNow/Basic/Slave Example");
     //Set device in AP mode to begin with
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

 else{   
        S2.begin(9600, SERIAL_8N1, RXPin, TXPin);                                             // Serial 2 is for GPS communication at 9600 baud - DO NOT MODIFY - Ublox Neo 6m 
           flag=0;
          // pinMode(LED_BUILTIN, OUTPUT);                                   // An LED indicator - Not Used
           Serial.println("Autonmous Mode Initiated...");
          WiFi.begin(ssid, password);
       
        while (WiFi.status() != WL_CONNECTED) {
          delay(1000);
          Serial.println("Connecting to WiFi..");
        }
       
        Serial.println("Connected to the WiFi network");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
// Ping Sensor////////////////////////////////////////////////////////////////////////////////////

        pinMode(trigPin1, OUTPUT);
        pinMode(echoPin1, INPUT);
        pinMode(trigPin2, OUTPUT);
        pinMode(echoPin2, INPUT);
// CompaSerial////////////////////////////////////////////////////////////////////////////////////
         Wire.begin();                                                    // Join I2C bus used for the HMC5883L compass
        //compass.init();
          qmc.init();
         Serial.println("Compass setting done");
         //Serial.print("Collision Avoidance ON");

            //Coordinates();
            Startup(); 
            getGPS();                                                        // Update the GPS location
            getCompass(); 
     }              
}  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  memcpy(&myData, data, sizeof(myData));
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.println(myData.x); 
  Serial.println(myData.y);
  Serial.println("");
  Serial.println();
  move();
  Serial.println();
} 

//********************************************************************************************************
// Main Loop

void loop()
{  button = digitalRead(autopilot);
    if (button == HIGH && flag==15)
    {      
      } 
     else
    {  
              
            client.connect(host,2455);
           
            if (client) {
                          Serial.println("Connected to Server");
                        
                            while (client.connected())
                            {
                              while (client.available()>0) 
                              {
                                
                                c = client.readStringUntil('\0');
                                 if(c.length()!=0 && c.length()>5)
                                    Coordinates();
                                
                                else if(c.length()!=0 && c.length()<5)
                                   { Serial.println("STOP");
                                    StopCar();
                                    blueToothVal=5;
                                   }
                              }
                             delay(10);
                            }
                         
                            //client.stop();
                           // Serial.println("Client disconnected");
                            
                 }
         
    }    
   
}



/*Autonomous RC Car
//NodeMCU SETUP:::
///////////////////////////////////////////////////////////////////////////
===========================================================================
GPS               ==2,4
Compass           ==22,21
UltraSonic sensor ==35,34,39,36
Motor Driver      ==27,26,25,33,32,23
GPS Switch        == 18

=========================================================================
/////////////////////////////////////////////////////////////////////////
*/ 
//Library files
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <TinyGPS++.h>          
#include <Wire.h>
#include <analogWrite.h>
#include <Arduino.h>
#include<HardwareSerial.h>
#include <MechaQMC5883.h>



//******************************************************************************************************                                                                  
// GPS Variables & Setup
HardwareSerial S2(2);
static const int RXPin = 2, TXPin = 4;
TinyGPSPlus gps;
int GPS_Course;                                                    // variable to hold the gps's determined course to destination
int Number_of_SATS;                                                // variable to hold the number of satellites acquired

//******************************************************************************************************
// Setup Motor Driver
int turn_Speed = 175;                                              // motor speed when using the compass to turn left and right
int mtr_Spd = 250;                                                 // motor speed when moving forward and reverse

#define CHANNEL 4

uint8_t mac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};

struct __attribute__((packed)) DataStruct {
    //char text[32];
    int x;
    int y;
    unsigned long time;
};

DataStruct myData;



//******************************************************************************************************
// CompaSerial Variables & Setup
MechaQMC5883 qmc;

int desired_heading;                                               // initialize variable - stores value for the new desired heading
int compass_heading;                                               // initialize variable - stores value calculated from compass readings
int compass_dev = 5;                                               // the amount of deviation that is allowed in the compass heading - Adjust as Needed
                                                                   // setting this variable too low will cause the robot to continuously pivot left and right
                                                                   // setting this variable too high will cause the robot to veer off course

int Heading_A;                                                     // variable to store compass heading
int Heading_B;                                                     // variable to store compass heading in Opposite direction
int pass = 0;                                                      // variable to store which paSerial the robot is on

//******************************************************************************************************
// Ping Sensor for Collision Avoidance

boolean pingOn = false;                                            // Turn Collision detection On or Off
int trigPin1 = 35;    // Trigger
int echoPin1 = 34;    // Echo
int trigPin2 = 39;    // Trigger
int echoPin2 = 36;    // Echo
long durationL, cmL;
long durationR, cmR;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;                                  // Store last time Ping was updated
const long interval = 200;                                         // Ping the Distance every X miliseconds

//*****************************************************************************************************
// GPS Locations

unsigned long Distance_To_Home;                                    // variable for storing the distance to destination

int ac =0;                                                         // GPS array counter
int wpCount = 0;                                                   // GPS waypoint counter
double Home_LATarray[2];                                          // variable for storing the destination Latitude - Only Programmed for 5 waypoint
double Home_LONarray[2];                                          // variable for storing the destination Longitude - up to 50 waypoints

int increment = 0;

//*****************************************************************************************************
//Web Pge Server Connection
 String c;
  char cs[37];
 
const char* ssid = "iBall-Baton-1";
const char* password = "epposheriakkitharam";

char host[]= "192.168.1.14";

double lati1;
double logi1;
double lati2;
double logi2;

//*****************************************************************************************************
// Extras

#define autopilot 18
 void  gesturecontroll();
 void getGPS();    
 void getCompass();  
 void Forward();
void Forward_Meter();
void Reverse();
void LeftTurn();
void RightTurn();
void SlowLeftTurn();
void SlowRightTurn();
void StopCar();
void setWaypoint();
 void move();
 void Startup();
 void goWaypoint();
 void clearWaypoints();

 int blueToothVal=0;
  
 int flag=0;
 int button;


void setup() 
{    Serial.begin(115200);                                            // Serial 0 is for communication with the computer
     Serial.println("Welcome");

//Motors////////////////////////////////////////////////////////////////////////////////////
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(23, OUTPUT);
pinMode(autopilot, INPUT);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 button = digitalRead(autopilot);
 if (button == HIGH)
 {   flag=15;
     Serial.println("Manual Control initated");
     Serial.println("ESPNow/Basic/Slave Example");
     //Set device in AP mode to begin with
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

 else{   
        S2.begin(9600, SERIAL_8N1, RXPin, TXPin);                                             // Serial 2 is for GPS communication at 9600 baud - DO NOT MODIFY - Ublox Neo 6m 
           flag=0;
          // pinMode(LED_BUILTIN, OUTPUT);                                   // An LED indicator - Not Used
           Serial.println("Autonmous Mode Initiated...");
          WiFi.begin(ssid, password);
       
        while (WiFi.status() != WL_CONNECTED) {
          delay(1000);
          Serial.println("Connecting to WiFi..");
        }
       
        Serial.println("Connected to the WiFi network");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
// Ping Sensor////////////////////////////////////////////////////////////////////////////////////

        pinMode(trigPin1, OUTPUT);
        pinMode(echoPin1, INPUT);
        pinMode(trigPin2, OUTPUT);
        pinMode(echoPin2, INPUT);
// CompaSerial////////////////////////////////////////////////////////////////////////////////////
         Wire.begin();                                                    // Join I2C bus used for the HMC5883L compass
        //compass.init();
          qmc.init();
         Serial.println("Compass setting done");
         Serial.print("Collision Avoidance ON");

            //Coordinates();
            Startup(); 
            getGPS();                                                        // Update the GPS location
            getCompass(); 
     }              
}  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  memcpy(&myData, data, sizeof(myData));
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); 
  Serial.println(myData.x); 
  Serial.println(myData.y);
  Serial.println("");
  Serial.println();
  move();
  Serial.println();
} 

//********************************************************************************************************
// Main Loop

void loop()
{  button = digitalRead(autopilot);
    if (button == HIGH && flag==15)
    {      
      } 
     else
    {  
              
               WiFiClient client;
            client.connect(host,2455);
           
            if (client) {
                          Serial.println("Connected to Server");
                        
                            while (client.connected())
                            {
                              while (client.available()>0) 
                              {
                                
                                c = client.readStringUntil('\0');
                                 if(c.length()!=0 && c.length()>5)
                                    Coordinates();
                                
                                else if(c.length()!=0 && c.length()<5)
                                   { Serial.println("STOP");
                                    StopCar();
                                    blueToothVal=5;
                                   }
                              }
                             delay(10);
                            }
                         
                            client.stop();
                            Serial.println("Client disconnected");
                            
               
                       }
                       Startup();
   // getGPS();                                                        // Update the GPS location
    //getCompass();                                                    // Update the CompaSerial Heading
    //Ping();                                                          
    
    }    
   
}
