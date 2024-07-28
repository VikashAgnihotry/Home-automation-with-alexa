//IOT Home automation project by Vikash Kumar
#ifdef ARDUINO_ARCH_ESP32
#define REMOTEXY_MODE__ESP32CORE_WIFI_CLOUD
#include <WiFiClient.h>
#include <WiFi.h>
#else
#include <ESP32WiFi.h>
#endif
#include <Espalexa.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "Moto"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "e08f05e2f9f5e83ecaa42d291f3f5135"
#define REMOTEXY_ACCESS_PASSWORD "1234"


#include <RemoteXY.h>

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 70 bytes
  { 255,3,0,0,0,63,0,17,0,0,0,24,1,106,200,1,1,3,0,2,
  29,11,44,22,1,2,26,31,24,79,78,0,79,70,70,0,2,30,75,44,
  22,1,2,26,31,31,79,78,0,79,70,70,0,2,31,147,44,22,1,2,
  26,31,31,79,78,0,79,70,70,0 };
  
 
struct {

    // input variables
  uint8_t switch_01; // =1 if switch ON and =0 if OFF
  uint8_t switch_02; // =1 if switch ON and =0 if OFF
  uint8_t switch_03; // =1 if switch ON and =0 if OFF

   
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)
 
#define PIN_SWITCH_01 6   //led pins
#define PIN_SWITCH_02 7
#define PIN_SWITCH_03 8

boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);

String Device_1_Name = "a";    //assigning names to the leds
String Device_2_Name = "b";
String Device_3_Name = "c";

boolean wifiConnected = false;

Espalexa espalexa;

void setup() 
{
 
  RemoteXY_Init (); 
  
  pinMode (PIN_SWITCH_01, OUTPUT);
  pinMode (PIN_SWITCH_02, OUTPUT);
  pinMode (PIN_SWITCH_03, OUTPUT);
  
  // TODO you setup code
   Serial.begin(115200);
    wifiConnected = connectWifi();

  if (wifiConnected)
  {
    
    espalexa.addDevice(Device_1_Name, firstLightChanged); //devices are added here
    espalexa.addDevice(Device_2_Name, secondLightChanged);
    espalexa.addDevice(Device_3_Name, thirdLightChanged);

    espalexa.begin();
  }
  else
  {
    while (1)
    {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
 
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_SWITCH_01, (RemoteXY.switch_01==0)?LOW:HIGH);   // condition written to work according to mobile app
  digitalWrite(PIN_SWITCH_02, (RemoteXY.switch_02==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_03, (RemoteXY.switch_03==0)?LOW:HIGH);
  
    
  espalexa.loop();

  RemoteXY_delay(1);

}
//******Condition written for working according to Alexa home////
void firstLightChanged(uint8_t brightness)
{
  
  if (brightness == 255)
    {
      digitalWrite(PIN_SWITCH_01, HIGH);
      Serial.println("Device1 ON");
      RemoteXY.switch_01=1;
    }
  else
  {
    digitalWrite(PIN_SWITCH_01, LOW);
    Serial.println("Device1 OFF");
    RemoteXY.switch_01=0;
  }
}

void secondLightChanged(uint8_t brightness)
{
  
  if (brightness == 255)
    {
      digitalWrite(PIN_SWITCH_02, HIGH);
      Serial.println("Device2 ON");
      RemoteXY.switch_02=1;
    }
  else
  {
    digitalWrite(PIN_SWITCH_02, LOW);
    Serial.println("Device2 OFF");
    RemoteXY.switch_02=0;
  }
}

void thirdLightChanged(uint8_t brightness)
{
 
  if (brightness == 255)
    {
      digitalWrite(PIN_SWITCH_03, HIGH);
      Serial.println("Device3 ON");
      RemoteXY.switch_03=1;
    }
  else
  {
    digitalWrite(PIN_SWITCH_03, LOW);
    Serial.println("Device3 OFF");
    RemoteXY.switch_03=0;
  }
  
}


boolean connectWifi()
{
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(REMOTEXY_WIFI_SSID, REMOTEXY_WIFI_PASSWORD);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(REMOTEXY_WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}