

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{   
  Blynk.run();
  button.read();
  server.handleClient();
  MQTT_loop();
}



void Mid1_Loop()
{  
  get_Temp_Humidity();
  LCD_Update_2();
}


void Mid2_Loop()
{
   LCD_Backlight_handle();
}



void Slow_Loop()
{  
  MQTT_publish();
  RunOnce_30mins();
  
  
}


void VerySlow_Loop()
{
  if(millis()/1000 > 7200)   // every 2 hours
    //Restart_ESP();
  
  Time_NTP_Update();  
  get_City_Weather();
  Blynk_Graph_DataPush();
}


void RunOnce_30mins()
{
	if(millis()/1000 - lastrun_30mins > 30 * 60 || lastrun_30mins == 0)
	{
		lastrun_30mins = millis()/1000;

    Time_NTP_Update();
		    
	}
}
