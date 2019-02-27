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

const int buzzer = 12;
const int buttonMon = 11;
const int buttonTue = 10;
const int buttonWed = 9;
const int buttonThu = 8;
const int buttonFri = 7;
const int buttonSat = 6;
const int buttonSun = 5;
const int lightSetting1 = 4;
const int lightSetting2 = 3;
const int lightSetting3 = 2;
const int lightControl = A0;

String comMsg = "";                     // Income from HC-05, DEL or ADD
String dayOfWeek = "";                  // Day of Week
String clockData = "";                  // Data of clock
String minData = "";                    // Data of minute

String clocksMon[10];                   // Clocks of Monday
String clocksTue[10];                   // Clocks of Tuesday
String clocksWed[10];                   // Clocks of Wednesday
String clocksThu[10];                   // Clocks of Thursday
String clocksFri[10];                   // Clocks of Friday
String clocksSat[10];                   // Clocks of Saturday
String clocksSun[10];                   // Clocks of Sunday

String minMon[10];                   // Minute of Monday
String minTue[10];                   // Minute of Tuesday
String minWed[10];                   // Minute of Wednesday
String minThu[10];                   // Minute of Thursday
String minFri[10];                   // Minute of Friday
String minSat[10];                   // Minute of Saturday
String minSun[10];                   // Minute of Sunday

int numberOfAlarms[7] = {0, 0, 0, 0, 0, 0, 0};  // Number of alarms set on Week
int alarmStatus[7] = {0, 0, 0, 0, 0, 0, 0};  // Number of alarms set on Week

int delayPeriod = 500;
int buttonState = 0;

boolean stringComplete = false;         // When serial message finished
boolean exist = false;
boolean alarmOn = false;

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
  pinMode(buzzer, OUTPUT);
  pinMode(buttonMon, INPUT);
  pinMode(buttonTue, INPUT);
  pinMode(buttonWed, INPUT);
  pinMode(buttonThu, INPUT);
  pinMode(buttonFri, INPUT);
  pinMode(buttonSat, INPUT);
  pinMode(buttonSun, INPUT);
  pinMode(lightSetting1, OUTPUT);
  pinMode(lightSetting2, OUTPUT);
  pinMode(lightSetting3, OUTPUT);
  pinMode(lightControl, OUTPUT);

  digitalWrite(buzzer, HIGH);
  digitalWrite(lightSetting1, LOW);
  digitalWrite(lightSetting2, LOW);
  digitalWrite(lightSetting3, LOW);
  digitalWrite(lightControl, LOW);
}

void loop()
{
  RTC.getTime();

  // Check Alarm
  if (RTC.dow == 1)
  {
    checkAlarm(clocksMon, minMon, 0, RTC.hour, RTC.minute, RTC.second);
  }
  else if (RTC.dow == 2)
  {
    checkAlarm(clocksTue, minTue, 1, RTC.hour, RTC.minute, RTC.second);
  }
  else if (RTC.dow == 3)
  {
    checkAlarm(clocksWed, minWed, 2, RTC.hour, RTC.minute, RTC.second);
  }
  else if (RTC.dow == 4)
  {
    checkAlarm(clocksThu, minThu, 3, RTC.hour, RTC.minute, RTC.second);
  }
  else if (RTC.dow == 5)
  {
    checkAlarm(clocksFri, minFri, 4, RTC.hour, RTC.minute, RTC.second);
  }
  else if (RTC.dow == 6)
  {
    checkAlarm(clocksSat, minSat, 5, RTC.hour, RTC.minute, RTC.second);
  }
  else if (RTC.dow == 7)
  {
    checkAlarm(clocksSun, minSun, 6, RTC.hour, RTC.minute, RTC.second);
  }

  //Rise alarm
  riseAlarm();

  // Add or delete alarm time
  if (stringComplete)
  {
    if (dayOfWeek == "MON")
    {
      alarmSettings(clocksMon, minMon, 0);
    }
    else if (dayOfWeek == "TUE")
    {
      alarmSettings(clocksTue, minTue, 1);
    }
    else if (dayOfWeek == "WED")
    {
      alarmSettings(clocksWed, minWed, 2);
    }
    else if (dayOfWeek == "THU")
    {
      alarmSettings(clocksThu, minThu, 3);
    }
    else if (dayOfWeek == "FRI")
    {
      alarmSettings(clocksFri, minFri, 4);
    }
    else if (dayOfWeek == "SAT")
    {
      alarmSettings(clocksSat, minSat, 5);
    }
    else if (dayOfWeek == "SUN")
    {
      alarmSettings(clocksSun, minSun, 6);
    }

    comMsg = "";
    dayOfWeek = "";
    clockData = "";
    minData = "";
    stringComplete = false;
  }
}

// Reading the Serial Input
void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    minData += inChar;
    if (inChar == '-')
    {
      comMsg = minData.substring(0, (minData.length() - 1));
      minData = "";
    }
    if (inChar == ';')
    {
      dayOfWeek = minData.substring(0, (minData.length() - 1));
      minData = "";
    }
    if (inChar == ':')
    {
      clockData = minData.substring(0, (minData.length() - 1));
      minData = "";
    }
    if (inChar == '!')
    {
      stringComplete = true;
      minData = minData.substring(0, (minData.length() - 1));
    }
  }
}

// Alarm settings
void alarmSettings(String hourArray[], String minArray[], int dayNumber)
{
  exist = false;
  // TODO: ARRAY LIMIT REACHED ERROR v2
  if (comMsg == "ADD")
  {
    if (numberOfAlarms[dayNumber] == 0) {
      hourArray[0] = clockData;
      minArray[0] = minData;
    }
    else
    {
      for (int i = 0; i < numberOfAlarms[dayNumber]; i++)
      {
        if (hourArray[numberOfAlarms[i]] == clockData )
        {
          if (minArray[numberOfAlarms[i]] == minData)
          {
            exist = true;
            digitalWrite(buzzer, LOW); // Buzzer
            delay(1000);
            digitalWrite(buzzer, HIGH); // Buzzer
            delay(1000);
            digitalWrite(buzzer, LOW); // Buzzer
            delay(1000);
          }
        }
      }
      if (!exist)
      {
        hourArray[numberOfAlarms[dayNumber]] = clockData;
        minArray[numberOfAlarms[dayNumber]] = minData;
        numberOfAlarms[dayNumber] = numberOfAlarms[dayNumber] + 1;
      }
    }
  }
  else if (comMsg == "DEL")
  {
    if (numberOfAlarms[dayNumber] != 0)
    {
      for (int i = 0; i < numberOfAlarms[dayNumber]; i++)
      {
        if (hourArray[numberOfAlarms[i]] == clockData )
        {
          if (minArray[numberOfAlarms[i]] == minData )
          {
            hourArray[numberOfAlarms[i]] = "";
            minArray[numberOfAlarms[i]] = "";
            numberOfAlarms[i] = numberOfAlarms[i] - 1;
            break;
          }
        }
      }
      // TODO: DELETE NOTHING ERROR v2
    }
    // TODO: NOTHING TO DELETE ERROR v2
  }
}

// Check Alarm
void checkAlarm(String hourArray[], String minArray[], int dayNumber, int currentHour, int currentMin, int currentSec)
{
  if (numberOfAlarms[dayNumber] > 0)
  {
    for (int i = 0; i < numberOfAlarms[dayNumber]; i++)
    {
      if (minArray[i].toInt() == currentMin)
      {
        if (hourArray[i].toInt() == currentHour)
        {
          if (20 <= currentSec <= 40)
          {
            alarmStatus[dayNumber] = 1;
            break;
          }
        }
      }
    }
  }
}

void riseAlarm()
{
  if (alarmStatus[0] == 1)
  {
    // Give alarm on Monday
    digitalWrite(lightSetting1, LOW); // Lights 0
    digitalWrite(lightSetting2, LOW);
    digitalWrite(lightSetting3, LOW);

    while (true)
    {
      buttonState = digitalRead(buttonMon);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[0] == 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(10000);
        break;
      }
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
    }
  }
  else if (alarmStatus[1] == 1)
  {
    // Give alarm on Tuesday
    digitalWrite(lightSetting1, HIGH); // Lights 1
    digitalWrite(lightSetting2, LOW);
    digitalWrite(lightSetting3, LOW);

    while (true)
    {
      buttonState = digitalRead(buttonTue);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[1] == 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(10000);
        break;
      }
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
    }
  }
  else if (alarmStatus[2] == 1)
  {
    // Give alarm on Wednesday
    digitalWrite(lightSetting1, LOW); // Lights 2
    digitalWrite(lightSetting2, HIGH);
    digitalWrite(lightSetting3, LOW);

    while (true)
    {
      buttonState = digitalRead(buttonWed);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[2] == 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(10000);
        break;
      }
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
    }
  }
  else if (alarmStatus[3] == 1)
  {
    // Give alarm on Thursday
    digitalWrite(lightSetting1, HIGH); // Lights 3
    digitalWrite(lightSetting2, HIGH);
    digitalWrite(lightSetting3, LOW);

    while (true)
    {
      buttonState = digitalRead(buttonThu);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[3] == 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(10000);
        break;
      }
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
    }
  }
  else if (alarmStatus[4] == 1)
  {
    // Give alarm on Friday
    digitalWrite(lightSetting1, LOW); // Lights 4
    digitalWrite(lightSetting2, LOW);
    digitalWrite(lightSetting3, HIGH);

    while (true)
    {
      buttonState = digitalRead(buttonFri);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[4] == 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(10000);
        break;
      }
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
    }
  }
  else if (alarmStatus[5] == 1)
  {
    // Give alarm on Saturday
    digitalWrite(lightSetting1, HIGH); // Lights 5
    digitalWrite(lightSetting2, LOW);
    digitalWrite(lightSetting3, HIGH);

    while (true)
    {
      buttonState = digitalRead(buttonSat);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[5] == 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(10000);
        break;
      }
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
    }
  }
  else if (alarmStatus[6] == 1)
  {
    // Give alarm on Sunday
    digitalWrite(lightSetting1, LOW); // Lights 6
    digitalWrite(lightSetting2, HIGH);
    digitalWrite(lightSetting3, HIGH);

    while (true)
    {
      buttonState = digitalRead(buttonSun);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[0] == 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(10000);
        break;
      }
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
    }
  }
}

// END
