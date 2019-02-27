// ##################################################################################
// #
// # H. Volkan Kasikaralar
// # Medicine Box
// # 
// # 26.02.2019
// #
// ##################################################################################

#include <Wire.h>
#include <DS1307new.h>

uint16_t startAddr = 0x0000;            // Ram Start Address
uint16_t lastAddr;                      // Ram Clock Adress
uint16_t TimeIsSet = 0xaa55;            // Is Clock Set?

String comMsg = "";                     // Income from HC-05, DEL or ADD
String dayOfWeek = "";                  // Day of Week
String clockData = "";                  // Data of clock

String clocksMon[10];                   // Clocks of Monday
String clocksTue[10];                   // Clocks of Tuesday
String clocksWed[10];                   // Clocks of Wednesday
String clocksThu[10];                   // Clocks of Thursday
String clocksFri[10];                   // Clocks of Friday
String clocksSat[10];                   // Clocks of Saturday
String clocksSun[10];                   // Clocks of Sunday

int numberOfAlarms[7] = {0, 0, 0, 0, 0, 0, 0};              // Number of alarms set on Week

boolean stringComplete = false;         // When serial message finished
boolean exist = false;

void setup()
{
  Serial.begin(38400);

  RTC.setRAM(0, (uint8_t *)&startAddr, sizeof(uint16_t));
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));

  // If not set then set
  if (TimeIsSet != 0xaa55)
  {
    RTC.stopClock();

    RTC.fillByYMD(2019, 2, 26);
    RTC.fillByHMS(22, 53, 0);

    RTC.setTime();
    TimeIsSet = 0xaa55;
    RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
    RTC.startClock();
  }
  else
  {
    RTC.getTime();
  }
}

void loop()
{
  RTC.getTime();

  // TODO: GET DAY OF WEEK v1
  // TODO: CHECK MINUTE v1
  // TODO: CHECK HOUR v1
  // TODO: ALARMING v1

  // Add or delete alarm time
  if (stringComplete)
  {
    if (dayOfWeek == "MON")
    {
      alarmSettings(clocksMon, 0);
    }
    else if (dayOfWeek == "TUE")
    {
      alarmSettings(clocksTue, 1);
    }
    else if (dayOfWeek == "WED")
    {
      alarmSettings(clocksWed, 2);
    }
    else if (dayOfWeek == "THU")
    {
      alarmSettings(clocksThu, 3);
    }
    else if (dayOfWeek == "FRI")
    {
      alarmSettings(clocksFri, 4);
    }
    else if (dayOfWeek == "SAT")
    {
      alarmSettings(clocksSat, 5);
    }
    else if (dayOfWeek == "SUN")
    {
      alarmSettings(clocksSun, 6);
    }

    comMsg = "";
    dayOfWeek = "";
    clockData = "";
    stringComplete = false;
  }
}

// Reading the Serial Input
void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    clockData += inChar;
    if (inChar == '-')
    {
      comMsg = clockData.substring(0, (clockData.length() - 1));
      clockData = "";
    }
    if (inChar == ';')
    {
      dayOfWeek = clockData.substring(0, (clockData.length() - 1));
      clockData = "";
    }
    if (inChar == '!')
    {
      stringComplete = true;
      clockData = clockData.substring(0, (clockData.length() - 1));
    }
  }
}

// Alarm settings
void alarmSettings(String dayArray[], int dayNumber)
{
  exist = false;
  // TODO: ARRAY LIMIT REACHED ERROR v2
  if (comMsg == "ADD")
  {
    if (numberOfAlarms[dayNumber] == 0) {
      dayArray[0] = clockData;
    }
    else
    {
      for (int i = 0; i < numberOfAlarms[dayNumber]; i++)
      {
        if (dayArray[numberOfAlarms[i]] == clockData )
        {
          exist = true;
          // TODO: BUZZER FAIL v1
        }
      }
      if (!exist)
      {
        dayArray[numberOfAlarms[dayNumber]] = clockData;
        numberOfAlarms[dayNumber] = numberOfAlarms[dayNumber] + 1;
      }
    }
  }
  else if (comMsg == "DEL")
  {
    if (numberOfAlarms[dayNumber] != 0) {
      for (int i = 0; i < numberOfAlarms[dayNumber]; i++)
      {
        if (dayArray[numberOfAlarms[i]] == clockData )
        {
          dayArray[numberOfAlarms[i]] = "";
          numberOfAlarms[i] = numberOfAlarms[i] - 1;
          break;
        }
      }
      // TODO: DELETE NOTTING ERROR v2
    }
    // TODO: NOTTING TO DELETE ERROR v2
  }
}

// END
