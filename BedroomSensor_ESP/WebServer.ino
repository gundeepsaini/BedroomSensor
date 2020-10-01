

void WebServer_Config()
{
  
  server.onNotFound(handleNotFound);
  server.on("/",Webpage_Home1);
  server.on("/webpage1",Webpage_Home1);
  server.on("/OTA",Webpage_OTA);
  server.on("/restart",Webpage_restart);
  server.on("/LCD_backlight_TMR", Webpage_LCDbacklight_TMR);
  server.on("/LCD_backlight_ON",  Webpage_LCDbacklight_ON);
  server.on("/LCD_backlight_OFF", Webpage_LCDbacklight_OFF);
  
  server.begin();  
}

void Webpage_Home1()
{
  Prep_webpage1();
  server.send ( 200, "text/html", webpage1 );
  delay(10);
  webpage1 = "";
}

void Webpage_OTA()
{
  OTA_Mode = 1;
  server.send(200,"text/plain","OTA mode active now");
}

void Webpage_LCDbacklight_TMR()
{
  LCD_TimeBasedCtrl = 0;
  Blynk.virtualWrite(V4, LCD_TimeBasedCtrl);
  LCD_backlight_Ctrl(1);
  server.send(200,"text/plain","Success!");
}

void Webpage_LCDbacklight_ON()
{
  LCD_TimeBasedCtrl = 0;
  Blynk.virtualWrite(V4, LCD_TimeBasedCtrl);
  LCD_backlight_Ctrl(2);
  server.send(200,"text/plain","Success!");
}

void Webpage_LCDbacklight_OFF()
{
  LCD_TimeBasedCtrl = 0;
  Blynk.virtualWrite(V4, LCD_TimeBasedCtrl);
  LCD_backlight_Ctrl(0);
  server.send(200,"text/plain","Success!");
}


void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found! Send one of these: /OTA, /restart, /webpage1, /LCD_backlight_TMR, /LCD_backlight_ON, /LCD_backlight_OFF");
}

void Webpage_restart()
{
  server.send(200,"text/plain","Restarting ESP");
  Serial.println("Restart requested from webserver");
  delay(1000);
  Restart_ESP();
}

