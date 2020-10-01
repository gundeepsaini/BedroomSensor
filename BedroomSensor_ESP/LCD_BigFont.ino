
int testvar = 0;

byte LT2[8] =
{
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111
};
byte LT1[8] =
{
 B00111,
 B01111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111
};

byte UB[8] =
{
 B11111,
 B11111,
 B11111,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000
};
byte RT[8] =
{
 B11100,
 B11110,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111
};
byte LL[8] =
{
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B01111,
 B00111
};
byte LB[8] =
{
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B11111,
 B11111,
 B11111
};
byte LR[8] =
{
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11110,
 B11100
};
byte UMB[8] =
{
 B11111,
 B11111,
 B11111,
 B00000,
 B00000,
 B00000,
 B11111,
 B11111
};
byte LMB[8] =
{
 B11111,
 B00000,
 B00000,
 B00000,
 B00000,
 B11111,
 B11111,
 B11111
};


void BigFont_Config()
{
 lcd.createChar(0,LT1);
 lcd.createChar(1,UB);
 lcd.createChar(2,RT);
 lcd.createChar(3,LL);
 lcd.createChar(4,LB);
 lcd.createChar(5,LR);
 lcd.createChar(6,UMB);
 lcd.createChar(7,LMB);
 lcd.clear();
}


void BigFont_print(int data, int pos)
{
	lcd.setCursor(0,0);
	switch(data)
	{
		case 0: custom0(pos); break;
		case 1: custom1(pos); break;
		case 2: custom2(pos); break;
		case 3: custom3(pos); break;
		case 4: custom4(pos); break;
		case 5: custom5(pos); break;
		case 6: custom6(pos); break;
		case 7: custom7(pos); break;
		case 8: custom8(pos); break;
		case 9: custom9(pos); break;
	}
}



void custom0(int pos)
{ 
// uses segments to build the number 0
 lcd.setCursor(pos, 0); // set cursor to column 0, line 0 (first row)
 lcd.write(0);  // call each segment to create
 lcd.write(1);  // top half of the number
 lcd.write(2);
 lcd.setCursor(pos, 1); // set cursor to colum 0, line 1 (second row)
 lcd.write(3);  // call each segment to create
 lcd.write(4);  // bottom half of the number
 lcd.write(5);
}

void custom1(int pos)
{
 lcd.setCursor(pos+1,0);
 //lcd.write(1);
 lcd.write(2);
 lcd.setCursor(pos+1,1);
 lcd.write(5);
}

void custom2(int pos)
{
 lcd.setCursor(pos,0);
 lcd.write(6);
 lcd.write(6);
 lcd.write(2);
 lcd.setCursor(pos, 1);
 lcd.write(3);
 lcd.write(7);
 lcd.write(7);
}

void custom3(int pos)
{
 lcd.setCursor(pos,0);
 lcd.write(6);
 lcd.write(6);
 lcd.write(2);
 lcd.setCursor(pos, 1);
 lcd.write(7);
 lcd.write(7);
 lcd.write(5);
}

void custom4(int pos)
{
 lcd.setCursor(pos,0);
 lcd.write(3);
 lcd.write(4);
 lcd.write(2);
 lcd.setCursor(pos+2, 1);
 lcd.write(5);
}

void custom5(int pos)
{
 lcd.setCursor(pos,0);
 lcd.write(0);
 lcd.write(6);
 lcd.write(6);
 lcd.setCursor(pos, 1);
 lcd.write(7);
 lcd.write(7);
 lcd.write(5);
}

void custom6(int pos)
{
 lcd.setCursor(pos,0);
 lcd.write(0);
 lcd.write(6);
 lcd.write(6);
 lcd.setCursor(pos, 1);
 lcd.write(3);
 lcd.write(7);
 lcd.write(5);
}

void custom7(int pos)
{
 lcd.setCursor(pos,0);
 lcd.write(1);
 lcd.write(1);
 lcd.write(2);
 lcd.setCursor(pos+1, 1);
 lcd.write(0);
}

void custom8(int pos)
{
 lcd.setCursor(pos,0);
 lcd.write(0);
 lcd.write(6);
 lcd.write(2);
 lcd.setCursor(pos, 1);
 lcd.write(3);
 lcd.write(7);
 lcd.write(5);
}

void custom9(int pos)
{
 lcd.setCursor(pos,0);
 lcd.write(0);
 lcd.write(6);
 lcd.write(2);
 lcd.setCursor(pos+2, 1);
 lcd.write(5);
}

void clearnumber(int pos)
{ // clears the area the custom number is displayed in
lcd.setCursor(0,0);
lcd.print("   ");
lcd.setCursor(0,1);
lcd.print("   ");
}

void customC(int pos)
{
 lcd.setCursor(pos,0);
 lcd.print("O");
 //lcd.write(1);
 lcd.setCursor(pos+1,0);
 lcd.write(0);
 lcd.write(1);
 lcd.write(1);
 lcd.setCursor(pos+1,1);
 lcd.write(3);
 lcd.write(4);
 lcd.write(4);
}
