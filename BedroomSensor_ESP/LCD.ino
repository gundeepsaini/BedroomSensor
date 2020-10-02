

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


bool Light_Time_Set;

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
  	   mins_now <= LCD_StopTime_mins &&
  	   !Light_Time_Set)
  	{
  		LCD_backlight_Ctrl(2);
  		Light_Time_Set = 1;	
  	}
  	else
  		LCD_backlight_Ctrl(0);
  		Light_Time_Set = 0;
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



void LCD_DisplayTemp_Room_City()
{
	
	// Negative Check
	bool IN_Temp_Negative = 0;
	bool OUT_Temp_Negative = 0;

	int IN_Temp = Temp;
	int OUT_Temp = City_Temperature;
	
	if(IN_Temp < 0)
	{
		IN_Temp = IN_Temp * -1;
		IN_Temp_Negative = 1;
	}
	if(OUT_Temp < 0)
	{
		OUT_Temp = OUT_Temp * -1;
		OUT_Temp_Negative = 1;
	}

	int IN_t1 =  int(IN_Temp) / 10;
	int IN_t2 =  int(IN_Temp) - (IN_t1 * 10);
	int IN_t3 = (int(IN_Temp * 10) % 100) % 10;

	int OUT_t1 =  int(OUT_Temp) / 10;
	int OUT_t2 =  int(OUT_Temp) - (OUT_t1 * 10);
	int OUT_t3 = (int(OUT_Temp * 10) % 100) % 10;


	// Big Font position is in multiples of 3
	lcd.clear();
	lcd.setCursor(0,0);
	
	// --- --- - --- --- ---
	//  t1  t2        t1  t2

	// --- --- - --- --- ---
	//   -  t2         -  t2


	if(OUT_Temp_Negative)
	{
		lcd.setCursor(0,0);
		lcd.write(4);
	}
	else
	{	BigFont_print(OUT_t1, 0); }
	BigFont_print(OUT_t2, 3);		


	lcd.setCursor(8,0);
	lcd.print((char)223);
	lcd.print("C");


	if(IN_Temp_Negative)
	{
		lcd.setCursor(10,0);
		lcd.write(4);
	}
	else
	{	BigFont_print(IN_t1, 10); }
	BigFont_print(IN_t2, 13);

}


void LCD_DisplayText_Temp()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("OUT");

	lcd.setCursor(6,1);
	lcd.print("Temp");

	lcd.setCursor(14,0);
	lcd.print("IN");
}



void LCD_DisplayText_Humidity()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("TEMP");

	lcd.setCursor(8,1);
	lcd.print("HUMIDITY");
}


void LCD_DisplayHumidity_Time()
{
		
	int IN_h1 = int(Humidity)/10;
	int IN_h2 = int(Humidity) - (IN_h1*10);

	Time_NTP_updateVar();

	int time_hr = time_now.hr;
	if(time_hr > 12)
		time_hr = time_hr - 12;

	int time1 = int(time_hr)/10;
	int time2 = int(time_hr) - (time1*10);
	int time3 = int(time_now.min)/10;
	int time4 = int(time_now.min) - (time3*10);

	lcd.clear();
	lcd.setCursor(0,0);
	if(time1 > 0)
		lcd.print("1");
	BigFont_print(time2, 1);
	lcd.setCursor(4,1);	
	lcd.print(":");
	BigFont_print(time3, 5);
	BigFont_print(time3, 8);
	

	lcd.setCursor(13,0);
	lcd.print("%");
	lcd.print(int(Humidity));
	
}


int LCD_cycle = 0;

void LCD_Update_2()
{		
	{		
		switch(LCD_cycle)
		{
			case 0:	
				LCD_DisplayText_Temp();
				break;
			case 1:	
				LCD_DisplayTemp_Room_City();
				break;
			case 2:					
				LCD_DisplayText_Humidity()
				break;
			case 3:	
				LCD_DisplayHumidity_Time();
				break;
		}

		if(LCD_cycle >= 3)
			LCD_cycle = 0;
		else
			LCD_cycle++;
	}
}

