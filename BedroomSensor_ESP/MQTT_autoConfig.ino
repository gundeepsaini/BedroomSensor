

/******************** LIB **************************************/
#define MQTT_MAX_PACKET_SIZE 1024  // < ----- Change in lib: This is because the defaul maxium length is 128b. So just go to PubSubClient.h and change #define MQTT_MAX_PACKET_SIZE 128 to #define MQTT_MAX_PACKET_SIZE 1024
#include "src/PubSubClient/PubSubClient.h"
#include <ArduinoJson.h>


/********************* Var *************************************/
WiFiClient espClient;
PubSubClient client(espClient);

long lastReconnectAttempt = 0;
float lastTemp, lastHumidity, lastCity_Temperature;

/******************** Config **************************************/
const char* mqtt_server     = SECRET_MQTT_Server;
const char* mqtt_user       = SECRET_MQTT_User;
const char* mqtt_password   = SECRET_MQTT_Pass;


#define MQTT_CONFIG_TEMP      "homeassistant/sensor/Bedroom/Temp/config"
#define MQTT_CONFIG_HUMIDITY  "homeassistant/sensor/Bedroom/Humidity/config"
#define MQTT_CONFIG_TEMP_CITY "homeassistant/sensor/Bedroom/TempCity/config"
#define MQTT_CONFIG_HUMI_CITY "homeassistant/sensor/Bedroom/HumiCity/config"
#define MQTT_TOPIC_STATE      "homeassistant/sensor/Bedroom/state"


/**************** External Functions ************************************/

void MQTT_config()
{
	client.setServer(mqtt_server, SECRET_MQTT_Port);
 	client.setCallback(MQTT_MessageRecd_callback);
}


void MQTT_loop()
{
	if (!client.connected())
    	MQTT_reconnect();              
  
  	client.loop();
}


void MQTT_publish()
{   
 
    if(1) // || TempChanged || HumidityChanged || TempCityChanged) // disabled the change parameter flag
    {
      // Use arduinojson.org/v6/assistant to compute the capacity.
      const size_t capacity = JSON_OBJECT_SIZE(7);
      DynamicJsonDocument doc(capacity);
      
      doc["temperature"] = String(Temp);
      doc["humidity"] = String(int(Humidity));
      
      if(City_Temperature_OK)        
        {
          doc["tempCity"] = String(City_Temperature);  
          doc["humiCity"] = String(City_Humidity);        
        }
      else
        {
          doc["tempCity"] = "NULL";
          doc["humiCity"] = "NULL";
        }
      
      lastTemp = Temp;
      lastHumidity = Humidity;
      lastCity_Temperature = City_Temperature;

      char data[256];
      serializeJson(doc, data, sizeof(data));
      client.publish(MQTT_TOPIC_STATE, data, true);
      Serial.println(data);
    }
    else
      Serial.println("Value change not material");
}


/**************** Internal Functions ************************************/

void MQTT_reconnect() 
{
  if (millis()/1000 - lastReconnectAttempt > 30 || lastReconnectAttempt == 0) 
  {
      Serial.println("MQTT reconnecting");
      if (client.connect(DeviceHostName, mqtt_user, mqtt_password)) 
      //if(client.connect(DeviceHostName, mqtt_user, mqtt_password, MQTT_TOPIC_LWT, willQoS, willRetain, MQTT_LWT_MESSAGE))  
      {
        //client.publish(MQTT_TOPIC_LWT, "Online", true);

        //MQTT_publish_config_Temp();
        //MQTT_publish_config_Humidity();
        //MQTT_publish_config_TempCity();
        //MQTT_publish_config_Humi_City();
        
        Serial.println("MQTT connected");
      }
      lastReconnectAttempt = millis()/1000;
  }
}




void MQTT_MessageRecd_callback(char* topic, byte* payload, unsigned int length) 
{
 
}



void MQTT_publish_config_Temp() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(9); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["dev_cla"] = "temperature";
  root["name"] = "Bedroom Temp";
  root["stat_t"] = MQTT_TOPIC_STATE;
  //root["avty_t"] = MQTT_TOPIC_LWT;
  root["unit_of_meas"] = "°C";
  root["val_tpl"] = "{{value_json.temperature}}";
  root["pl_avail"] = "online";
  root["pl_not_avail"] = "offline";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_TEMP, message, true);
  //Serial.println(message);
}

void MQTT_publish_config_TempCity() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(7); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["dev_cla"] = "temperature";
  root["name"] = "City Temp";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "°C";
  root["val_tpl"] = "{{value_json.tempCity}}";
  root["pl_avail"] = "online";
  root["pl_not_avail"] = "offline";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_TEMP_CITY, message, true);
  //Serial.println(message);
}

void MQTT_publish_config_Humidity() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(7); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["dev_cla"] = "humidity";
  root["name"] = "Bedroom Humidity";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "%";
  root["val_tpl"] = "{{value_json.humidity}}";
  root["pl_avail"] = "online";
  root["pl_not_avail"] = "offline";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_HUMIDITY, message, true);
  //Serial.println(message);
}


void MQTT_publish_config_Humi_City() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(7); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["dev_cla"] = "humidity";
  root["name"] = "City Humidity";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "%";
  root["val_tpl"] = "{{value_json.humiCity}}";
  root["pl_avail"] = "online";
  root["pl_not_avail"] = "offline";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_HUMI_CITY, message, true);
  //Serial.println(message);
}


bool MQTT_hasValueChanged(float newValue, float prevValue, float maxDiff) 
{
  return !isnan(newValue) && (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}
