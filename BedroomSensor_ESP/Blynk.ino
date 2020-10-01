

char BlynkAuth[] = SECRET_BLYNK_AUTH4;
WidgetTerminal terminal(V0);

/**********************************************************/
void Blynk_Config()
{
  Blynk.config(BlynkAuth);
  while (Blynk.connect(1000) == false);  
  lcd.print(" Blynk");  
  
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
}


void Blynk_Graph_DataPush()
{
  Blynk.virtualWrite(V1, Temp);
  Blynk.virtualWrite(V2, Humidity);
  if(City_Temperature_OK) 
    Blynk.virtualWrite(V3, City_Temperature);
  if(City_Humidity_OK) 
    Blynk.virtualWrite(V6, City_Humidity);
}


/**********************************************************/

BLYNK_WRITE(V0)
{
// Terminal
  String Data = param.asStr();
  if(Data == String("OTA")) 
      OTA_Mode = 1;
}

BLYNK_READ(V1)
{
  Blynk.virtualWrite(V1, Temp);
}

BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, Humidity);
}

BLYNK_READ(V3)
{
  Blynk.virtualWrite(V3, City_Temperature);
}

BLYNK_WRITE(V4)
{
  int input = param.asInt();
  if(input > 0)
    LCD_TimeBasedCtrl = 1;
  else
    LCD_TimeBasedCtrl = 0;
}

BLYNK_WRITE(V5)
{
  TimeInputParam t(param);
  
  if (t.hasStartTime() && t.hasStopTime())
  {
    LCD_StartTime_hr = t.getStartHour();
    LCD_StartTime_mins = t.getStartMinute();
    LCD_StopTime_hr = t.getStopHour();
    LCD_StopTime_mins = t.getStopMinute();
    
    Serial.print(LCD_StartTime_hr);
    Serial.print(":");
    Serial.print(LCD_StartTime_mins);
    Serial.print(" - ");
    Serial.print(LCD_StopTime_hr);
    Serial.print(":");
    Serial.println(LCD_StopTime_mins);
  }
}

BLYNK_READ(V6)
{
  Blynk.virtualWrite(V6, City_Humidity);
}