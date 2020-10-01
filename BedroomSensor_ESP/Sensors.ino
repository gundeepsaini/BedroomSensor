

void get_Temp_Humidity()
{
  //delay(DHT.getMinimumSamplingPeriod());

  DHT_temperature = DHT.getTemperature();
  Humidity = DHT.getHumidity();
  TempSensor.requestTemperatures();
  Temp = TempSensor.getTempCByIndex(0);

  if(Temp < -50)
    {
      Temp = DHT_temperature;
      Serial.println("Temp Sensor failed!");
    }
  //Serial.print("%\t");
  //Serial.print(Temp, 1);
  //Serial.println("°C\t");
}


void onButtonPressed() 
{
  Serial.println("ButtonPress!");
  
  if(LCD_Status)
    LCD_backlight_Ctrl(0);
  else
    LCD_backlight_Ctrl(1);
}
