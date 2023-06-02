
// DST and Time Zones https://randomnerdtutorials.com/esp32-ntp-timezones-daylight-saving/
// https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
// https://mikaelpatel.github.io/Arduino-RTC/de/df7/time_8h_source.html

/*

int8_t 	tm_sec
 	Seconds [0-60]. More...
int8_t 	tm_min
 	Minutes [0-59]. More...
int8_t 	tm_hour
 	Hours [0-23]. More...
int8_t 	tm_mday
 	Day in Month [1-31]. More...
int8_t 	tm_wday
 	Days since Sunday [0-6]. More...
int8_t 	tm_mon
 	0-11 Months since January [0-11]. More...
int16_t 	tm_year
 	Years since 1900. More..
int16_t 	tm_yday
 	days since January 1 [0-365]. More... 
int16_t 	tm_isdst
 	Daylight Saving Time flag [-1/0/1]. More...

*/



#include "time.h"


// Timezone Info: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define TimeZone "CET-1CEST,M3.5.0,M10.5.0/3"

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  
  // Convert to HH:MM:SS
  char locTime[9];
  sprintf(locTime, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  // Convert to HH:MM:SS
  char locdate[9];
    sprintf(locdate, "%02d.%02d.%02d", timeinfo.tm_year-100, timeinfo.tm_mon+1, timeinfo.tm_mday);

  Serial.print(locdate);
  Serial.print(" ");
  Serial.println(locTime);
}


void Config_Time()
{
  struct tm timeinfo;

  Serial.println("Setting up time");
  configTime(0, 0, NTP_Server_local);    // First connect to NTP server, with 0 TZ offset
  
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.printf("  Setting Timezone to ", TimeZone);
  setenv("TZ", TimeZone, 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();

  printLocalTime();

}





bool Time_NTP_isValid()
{    
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("  Failed to obtain time");
    return 0;
  }

  return 1;
}


void Time_NTP_Update()
{
  printLocalTime();
}













