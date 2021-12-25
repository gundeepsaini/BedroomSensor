


void Config_TempSensor_DS18B20()
{
  uint8_t address[] = {40, 250, 31, 218, 4, 0, 0, 52};
  uint8_t response = 0;

  response = ds.select(address);

  if(!response) 
  {
    Serial.println("Device not found!");
  }
}



void get_Temp_Humidity()
{
  //delay(DHT.getMinimumSamplingPeriod());

  DHT_temperature = DHT.getTemperature();
  Humidity = DHT.getHumidity();
  Temp = ds.getTempC();

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
