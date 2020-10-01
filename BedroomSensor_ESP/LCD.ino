

void LCD_Update()
{	
	if(millis()/1000 - lastLCDChange > 2)
	{		
		switch(DisplayCycle)
		{
			case 0:	
				DisplayCycle ++;
				LCD_DisplayTemp();
				break;
			case 1:	
				DisplayCycle ++;
				LCD_DisplayHumidity();
				break;
			case 2:	
				DisplayCycle = 0;
				LCD_DisplayTemp_City();
				break;
		}

	 lastLCDChange = millis()/1000;
	}
}



void LCD_Backlight_handle()
{
	
  if(	LCD_TimeBasedCtrl == 0 &&
  		LCD_Status == 1 && 
  		LCD_Always_ON == 0 && 
     	millis()/1000 - LCD_TMR_Start > LCD_TMR_SP )
  {
  		LCD_backlight_Ctrl(0);
  }

  if(LCD_TimeBasedCtrl)
  {
  	Time_NTP_updateVar();
  	int hr_now  	= time_now.hr;
  	int mins_now  	= time_now.min;

  	if(hr_now >= LCD_StartTime_hr &&
  	   mins_now >= LCD_StartTime_mins &&
  	   hr_now <= LCD_StopTime_hr &&
  	   mins_now <= LCD_StopTime_mins)
  	{
  		LCD_backlight_Ctrl(2);	
  	}
  	else
  		LCD_backlight_Ctrl(0);
  }
}



void LCD_backlight_Ctrl(int CMD)
{
  switch(CMD)
  {
  	case 0:
  		lcd.noBacklight();
     	LCD_TMR_Start = 0;
     	LCD_Status = 0;
     	LCD_Always_ON = 0;
     	break;

     	// 1 - On with auto off timer
    case 1:
		lcd.backlight();
    	LCD_Status = 1;
  		LCD_TMR_Start = millis()/1000;
  		LCD_Always_ON = 0;
  		break;

  		// 2 - Always on
  	case 2: 
		lcd.backlight();
    	LCD_Status = 1;
  		LCD_TMR_Start = millis()/1000;
  		LCD_Always_ON = 1; 
  		break; 
  }
}




void LCD_DisplayTemp()
{
	// Big Font position is in multiples of 3
	lcd.clear();
	lcd.setCursor(0,0);
	
	bool IsNegative = 0;
	
	if(Temp == -127)
		Temp = 0;

	if(Temp < 0)
	 {
	  	Temp = Temp *-1;
	  	IsNegative = 1;
	 }

	int t1 = int(Temp)/10;
	int t2 = int(Temp) - (t1*10);
	int t3 = (int(Temp *10) % 100) % 10;

	if(0)
	{
		if(t1 != 0)
			BigFont_print(t1, 0);
		BigFont_print(t2, 3);
		lcd.setCursor(6,1);
		lcd.print(".");
		BigFont_print(t3, 7);
		customC(11);
	}
	else
	{	
		lcd.setCursor(0,0);
		lcd.print("Temp");
		lcd.setCursor(0,1);
		lcd.print("Room ");

		if(IsNegative)
			{
				lcd.setCursor(5,0);
				lcd.write(4);
			}
		if(t1 != 0)
		{
			BigFont_print(t1, 6);
			BigFont_print(t2, 10);
		}
		else
			BigFont_print(t2, 10);

		lcd.setCursor(14,0);
		lcd.print((char)223);
		lcd.print("C");
		
		
	}
}


void LCD_DisplayHumidity()
{
	// Big Font position is in multiples of 3
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("RH %");
	lcd.setCursor(0,1);
	lcd.print("Room ");

	int h1 = int(Humidity)/10;
	int h2 = int(Humidity) - (h1*10);


	if(1)
	{
		if(h1 != 0)
			BigFont_print(h1, 9);
		BigFont_print(h2, 13);
	}
	else
	{
		if(h1 != 0)
			BigFont_print(h1, 9);
		BigFont_print(h2, 12);
	}
}



void LCD_DisplayTemp_City()
{
	// Big Font position is in multiples of 3
	lcd.clear();
	lcd.setCursor(0,0);

	float LCD_Temp_City = City_Temperature;
	
	bool IsNegative = 0;
	if(LCD_Temp_City < 0)
	 {
	  	LCD_Temp_City = LCD_Temp_City *-1;
	  	IsNegative = 1;
	 }

	int t1 = int(LCD_Temp_City)/10;
	int t2 = int(LCD_Temp_City) - (t1*10);
	int t3 = (int(LCD_Temp_City *10) % 100) % 10;


	if(0)
	{
		if(t1 != 0)
			BigFont_print(t1, 0);
		BigFont_print(t2, 3);
		lcd.setCursor(6,1);
		lcd.print(".");
		BigFont_print(t3, 7);
		customC(11);
	}
	else
	{
		if(City_Temperature_OK)
		{
			if(IsNegative)
				{
					lcd.setCursor(0,0);
					lcd.write(4);
				}
			if(t1 == 0)
				BigFont_print(t2, 1);
			else
			{
				BigFont_print(t1, 1);
				BigFont_print(t2, 5);
			}
		}
		else
		{
			lcd.setCursor(2,0);
			lcd.write(4);
		}

		lcd.setCursor(10,0);
		lcd.print((char)223);
		lcd.print("C");
		lcd.setCursor(9,1);
		lcd.print("Outside");
	}
}
