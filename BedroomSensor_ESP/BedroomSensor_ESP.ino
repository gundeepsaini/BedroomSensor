/*-----------------------------------------

            Bedroom Sensor Module

Date: Jan 2020 (v0)
Device: ESP8266 NodeMcU

Note:
  - Reset ESP after upload, disable IPv6, 
      reset adapter, allow port in firewall
  - send "OTA" on blynk terminal to enter dedicated mode
    or navigate to "ip/OTA" for OTA through web portal
  
To-Do:
  - 

Changes:
  - v20 | 14.03.2020
    Updated code and moved to Github
    Shifted personal config data to Secret.h file
    Added City Humidity as additional parameter
    Changed time handling to include DST effects (day light saving)    

  - v15 | 09.03.2020
    Updated MQTT protocol to be auto-discovered by Home Assistant

  - v14 | 08.03.2020
    Added MQTT protocol to communicate with RPi

  - v13 | 27.02.2020
    Forgot to add changes from RPi files
    Added support for blink and server
    removed fetching from other ESP servers - module serves as client only

  - v12 | 27.02.2020
    Use of Big Font on LCD
    Removed data fetch from other sensors

  - v11 | 19.01.2020
    Ongoing changes

------------------------------------------- */


/* ------------- LIB ------------------------------ */
#include "Secrets.h"
#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "DHTesp.h"
#include <DallasTemperature.h>
#include <ESP8266WebServer.h>
#include <EasyButton.h>
#include <BlynkSimpleEsp8266.h>


/* ------------- CONFIG VAR ------------------------------ */
unsigned long looptime_Fast = 0;    // in secs
unsigned long looptime_Mid1 = 2;    // in secs
unsigned long looptime_Mid2 = 10;   // in secs
unsigned long looptime_Slow       = 1 * (60);      // in mins
unsigned long looptime_VerySlow   = 5 * (60);      // in mins

int LCD_TMR_SP = 60;
#define SDA_Pin     0 
#define SCK_Pin     5
#define Button_Pin  4
#define TEMP_Pin    13
#define DHT_Pin     12


/* ------------- VAR ------------------------------ */
const char* ssid             = SECRET_WIFI_SSID2;
const char* pass             = SECRET_WIFI_PASS2;
const char* DeviceHostName   = SECRET_Device_Name4;
const char* OTA_Password     = SECRET_Device_OTA_PASS; 
unsigned long lastrun_fast, lastrun_Mid1;
unsigned long lastrun_Mid2, lastrun_slow, lastrun_Veryslow;
unsigned long lastrun_30mins,lastrun_OTA, LastNTP_Update;
bool OTA_Mode=0;
String NameBuffer[10];
float DataBuffer[10];
float Temp, Humidity;
unsigned long lastLCDChange;
int DisplayCycle;
String webpage1="";
bool LCD_Always_ON, LCD_Status, LCD_TimeBasedCtrl;
float City_Temperature;
int City_Humidity;
int LCD_StartTime_hr, LCD_StartTime_mins;
int LCD_StopTime_hr, LCD_StopTime_mins;
bool City_Temperature_OK = false;
bool City_Humidity_OK = false;
float DHT_temperature;
unsigned long LCD_TMR_Start=0; 


//(lcd_Addr,En,Rw,Rs,d4,d5,d6,d7,backlighPin,t_backlighPol)
LiquidCrystal_I2C lcd(0x27,5,6,7,4,3,2,1,0,POSITIVE);
WiFiUDP ntpUDP;
DHTesp DHT;
OneWire oneWire(TEMP_Pin);
DallasTemperature TempSensor(&oneWire);
ESP8266WebServer server(80);
EasyButton button(Button_Pin);



void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);
  Serial.println(DeviceHostName);

  Wire.begin(SDA_Pin, SCK_Pin);
  lcd.begin(16,2);
  LCD_backlight_Ctrl(0);
  BigFont_Config();
  lcd.clear();
  lcd.print(DeviceHostName);

  wifi_station_set_hostname(DeviceHostName);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Wifi Error! Rebooting in 30s...");
    delay(30 *1000);
    digitalWrite(LED_BUILTIN, HIGH);
    Restart_ESP();
  }
  
  Serial.print("Connected to ");
  Serial.print(WiFi.SSID());
  Serial.print(", IP Address: ");
  Serial.println(WiFi.localIP());
  
  lcd.setCursor(0,1);
  lcd.print("Wifi");

  Time_NTP_Config();  

  DHT.setup(DHT_Pin, DHTesp::DHT11);
  TempSensor.begin();
  button.begin();
  button.onPressed(onButtonPressed);
  
  Blynk_Config();
  OTA_Config();
  WebServer_Config();
  MQTT_config();
  //LCD_backlight_Ctrl(1);
  get_City_Weather();
  
  lcd.clear();
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Ready");
}


void loop() 
{
  // Always
  Always_loop();

  // Fast Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_fast > looptime_Fast) || lastrun_fast ==0))
  {
    lastrun_fast = millis()/1000;
    Fast_Loop();
  }

  // Mid1 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid1 > looptime_Mid1) || lastrun_Mid1 ==0))
  {
    lastrun_Mid1 = millis()/1000;
    Mid1_Loop();
  }

  // Mid2 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid2 > looptime_Mid2) || lastrun_Mid2 ==0))
  {
    lastrun_Mid2 = millis()/1000;
    Mid2_Loop();
  }

  // Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_slow > looptime_Slow) || lastrun_slow ==0))
  {
    lastrun_slow = millis()/1000;
    Slow_Loop();
  }

    // Very Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Veryslow > looptime_VerySlow) || lastrun_Veryslow ==0))
  {
    lastrun_Veryslow = millis()/1000;
    VerySlow_Loop();
  }
}



void Restart_ESP()
{
  Serial.println("Restarting ESP");
  ESP.restart();
  delay(1000);
  while(1);
}
