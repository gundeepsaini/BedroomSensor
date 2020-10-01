/*

https	://home.openweathermap.org/api_keys

*/



void get_City_Weather() 
{
	char APIserver[] = "api.openweathermap.org";     
	int status = WL_IDLE_STATUS;
	
	WiFiClient client; 

 	if (client.connect(APIserver, 80)) 
 	 { 
   		Serial.println("connected to api.openweathermap.org");          		
	   
     client.print("GET /data/2.5/weather?zip="); 
	   client.print(SECRET_MyLocation4); 
	   client.print("&appid=");
     client.print(SECRET_OpenWeatherMap_API_Key4);
	   client.println("&units=metric"); 
	   client.println("Host: api.openweathermap.org"); 
	   client.println("Connection: close"); 
	   client.println(); 
 	 }
 	else 
 	 { 
   		Serial.println("unable to connect"); 
 	 } 
 	
while(client.connected())
  {
    if(1) //client.available())
    {
      String line = client.readStringUntil('\r');
      //Serial.print(line);

      if(line.length() > 0)
      {
      	int t1 = line.indexOf("temp");    	  
    	  if(t1 > 0)
      		{
      			String subline = line.substring(t1+6, t1+6+10); //Extracts without the add/subtract: temp":1.86,"
            int t2 = subline.indexOf(",");
            if(t2 > 0)
            {
              subline = subline.substring(0, t2); //Extracts numerals
              float temp = subline.toFloat();
              if(temp < 100 && temp > -100)
                  {
                    City_Temperature = temp;
                    City_Temperature_OK = true;
                  }

        			//Serial.print("City Temp:");
        			Serial.println(City_Temperature);
              //Serial.println(subline);
            }
      		}

        // also extract humidity: e.g "humidity":49},"visibility":10000
          t1 = line.indexOf("humidity");          
          if(t1 > 0)
          {
            String subline = line.substring(t1+10, t1+10+10);                        
            int t2 = subline.indexOf(",");
            if(t2 > 0)
            {
              subline = subline.substring(0, t2); //Extracts numerals
              int temp_H = int(subline.toFloat());
               if(temp_H <= 100 && temp_H >= 0)
                  {
                    City_Humidity = temp_H;
                    City_Humidity_OK = true;
                  }
            
              //Serial.print("City humidity:");
              Serial.println(City_Humidity);
              //Serial.println(subline);
            }
          }
      }
    } 
    else 
    {
      // No data yet, wait a bit
      delay(10);
    }
  }
} 
