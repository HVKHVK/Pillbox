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

char comMsg = "";                   // Income from HC-05, DEL or ADD
int dayOfWeek;                      // Day of Week
int clockData = 0;                  // Data of clock
int minData = 0;                    // Data of minute
String input = "";

int clocksMon[3];                   // Clocks of Monday
int clocksTue[3];                   // Clocks of Tuesday
int clocksWed[3];                   // Clocks of Wednesday
int clocksThu[3];                   // Clocks of Thursday
int clocksFri[3];                   // Clocks of Friday
int clocksSat[3];                   // Clocks of Saturday
int clocksSun[3];                   // Clocks of Sunday

int minMon[3];                   // Minute of Monday
int minTue[3];                   // Minute of Tuesday
int minWed[3];                   // Minute of Wednesday
int minThu[3];                   // Minute of Thursday
int minFri[3];                   // Minute of Friday
int minSat[3];                   // Minute of Saturday
int minSun[3];                   // Minute of Sunday

int numberOfAlarms[7] = {0, 0, 0, 0, 0, 0, 0};  // Number of alarms set on Week
int alarmStatus[7] = {0, 0, 0, 0, 0, 0, 0};  // Number of alarms set on Week

int delayPeriod = 250;
int buttonState = 0;

boolean stringComplete = false;         // When serial message finished
boolean exist = false;
boolean alarmOn = false;

void setup()
{
  Serial.begin(38400);
  input.reserve(500);

  RTC.setRAM(0, (uint8_t *)&startAddr, sizeof(uint16_t));
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));

  // If not set then set
  if (TimeIsSet != 0xaa55)
  {
    RTC.stopClock();

    RTC.fillByYMD(2019, 3, 1);
    RTC.fillByHMS(22, 38, 20);

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

  int currentHour = RTC.hour;
  int currentMin = RTC.minute;
  int currentSec = RTC.second;
  // Check Alarm

  if (RTC.dow == 1)
  {
    if (numberOfAlarms[0] > 0)
    {
      for (int i = 0; i < numberOfAlarms[0]; i++)
      {
        if (minMon[i] == currentMin)
        {
          if (clocksMon[i] == currentHour)
          {
            if (40 <= currentSec <= 50)
            {
              alarmStatus[0] = 1;
              break;
            }
          }
        }
      }
    }
  }
  else if (RTC.dow == 2)
  {
    if (numberOfAlarms[1] > 0)
    {
      for (int i = 0; i < numberOfAlarms[1]; i++)
      {
        if (minTue[i] == currentMin)
        {
          if (clocksTue[i] == currentHour)
          {
            if (40 <= currentSec <= 50)
            {
              alarmStatus[1] = 1;
              break;
            }
          }
        }
      }
    }
  }
  else if (RTC.dow == 3)
  {
    if (numberOfAlarms[2] > 0)
    {
      for (int i = 0; i < numberOfAlarms[2]; i++)
      {
        if (minWed[i] == currentMin)
        {
          if (clocksWed[i] == currentHour)
          {
            if (40 <= currentSec <= 50)
            {
              alarmStatus[2] = 1;
              break;
            }
          }
        }
      }
    }
  }
  else if (RTC.dow == 4)
  {
    if (numberOfAlarms[3] > 0)
    {
      for (int i = 0; i < numberOfAlarms[3]; i++)
      {
        if (minThu[i] == currentMin)
        {
          if (clocksThu[i] == currentHour)
          {
            if (40 <= currentSec <= 50)
            {
              alarmStatus[3] = 1;
              break;
            }
          }
        }
      }
    }
  }
  else if (RTC.dow == 5)
  {
    if (numberOfAlarms[4] > 0)
    {
      for (int i = 0; i < numberOfAlarms[4]; i++)
      {
        if (minFri[i] == currentMin)
        {
          if (clocksFri[i] == currentHour)
          {
            if (40 <= currentSec <= 50)
            {
              alarmStatus[4] = 1;
              break;
            }
          }
        }
      }
    }
  }
  else if (RTC.dow == 6)
  {
    if (numberOfAlarms[5] > 0)
    {
      for (int i = 0; i < numberOfAlarms[5]; i++)
      {
        if (minSat[i] == currentMin)
        {
          if (clocksSat[i] == currentHour)
          {
            if (40 <= currentSec <= 50)
            {
              alarmStatus[5] = 1;
              break;
            }
          }
        }
      }
    }
  }
  else if (RTC.dow == 7)
  {
    if (numberOfAlarms[6] > 0)
    {
      for (int i = 0; i < numberOfAlarms[6]; i++)
      {
        if (minSun[i] == currentMin)
        {
          if (clocksSun[i] == currentHour)
          {
            if (40 <= currentSec <= 50)
            {
              alarmStatus[6] = 1;
              break;
            }
          }
        }
      }
    }
  }

  //Rise alarm
  riseAlarm();

  // Add or delete alarm time
  if (stringComplete)
  {
    // TODO: ABILITY TO GET "5" INSTEAD OF "05"
    comMsg = input.charAt(0);
    char d1 = input.charAt(2);
    int d2 = (int)d1 - 48;
    dayOfWeek = d2;

    char c1 = input.charAt(4);
    char c2 = input.charAt(5);
    char m1 = input.charAt(7);
    char m2 = input.charAt(8);

    int c3 = (int)c1 - 48;
    int c4 = (int)c2 - 48;
    clockData = c3 * 10 + c4;

    int m3 = (int)m1 - 48;
    int m4 = (int)m2 - 48;
    minData = m3 * 10 + m4;
    input = "";
//    Serial.println(comMsg);
//    Serial.println(dayOfWeek);
//    Serial.println(clockData);
//    Serial.println(minData);

    if (dayOfWeek == 1)
    {

      exist = false;
      // TODO: ARRAY LIMIT REACHED ERROR v2
      if (comMsg == 'A')
      {

        if (numberOfAlarms[0] == 0) {

          clocksMon[0] = clockData;
          minMon[0] = minData;
          numberOfAlarms[0] = numberOfAlarms[0] + 1;

        }
        else
        {
          for (int i = 0; i < numberOfAlarms[0]; i++)
          {
            if (clocksMon[i] == clockData )
            {
              if (minMon[i] == minData)
              {
                exist = true;
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
                delay(1000);
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer

              }
            }
          }
          if (!exist)
          {
            clocksMon[numberOfAlarms[0]] = clockData;
            minMon[numberOfAlarms[0]] = minData;
            numberOfAlarms[0] = numberOfAlarms[0] + 1;
          }
        }
      }
      else if (comMsg == 'D')
      {
        // TODO: WHEN DELETED MOVE THE ARRAY AND FIX v2
        if (numberOfAlarms[0] != 0)
        {
          for (int i = 0; i < numberOfAlarms[0]; i++)
          {
            if (clocksMon[i] == clockData )
            {
              if (minMon[i] == minData )
              {
                clocksMon[i] = 0;
                minMon[i] = 0;
                numberOfAlarms[0] = numberOfAlarms[0] - 1;
                break;
              }
            }
          }
          // TODO: DELETE NOTHING ERROR v2
        }
        // TODO: NOTHING TO DELETE ERROR v2
      }
    }
    else if (dayOfWeek == 2)
    {

      exist = false;
      // TODO: ARRAY LIMIT REACHED ERROR v2
      if (comMsg == 'A')
      {
        if (numberOfAlarms[1] == 0) {

          clocksTue[0] = clockData;
          minTue[0] = minData;
          numberOfAlarms[1] = numberOfAlarms[1] + 1;

        }
        else
        {
          for (int i = 0; i < numberOfAlarms[1]; i++)
          {
            if (clocksTue[i] == clockData )
            {
              if (minTue[i] == minData)
              {
                exist = true;
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
                delay(1000);
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer

              }
            }
          }
          if (!exist)
          {
            clocksTue[numberOfAlarms[1]] = clockData;
            minTue[numberOfAlarms[1]] = minData;
            numberOfAlarms[1] = numberOfAlarms[1] + 1;
          }
        }
      }
      else if (comMsg == 'D')
      {
        if (numberOfAlarms[1] != 0)
        {
          for (int i = 0; i < numberOfAlarms[1]; i++)
          {
            if (clocksTue[i] == clockData )
            {
              if (minTue[i] == minData )
              {
                clocksTue[i] = 0;
                minTue[i] = 0;
                numberOfAlarms[1] = numberOfAlarms[1] - 1;
                break;
              }
            }
          }
          // TODO: DELETE NOTHING ERROR v2
        }
        // TODO: NOTHING TO DELETE ERROR v2
      }
    }
    else if (dayOfWeek == 3)
    {

      exist = false;
      // TODO: ARRAY LIMIT REACHED ERROR v2
      if (comMsg == 'A')
      {

        if (numberOfAlarms[2] == 0) {

          clocksWed[0] = clockData;
          minWed[0] = minData;
          numberOfAlarms[2] = numberOfAlarms[2] + 1;

        }
        else
        {
          for (int i = 0; i < numberOfAlarms[2]; i++)
          {
            if (clocksWed[i] == clockData )
            {
              if (minWed[i] == minData)
              {
                exist = true;
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
                delay(1000);
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer

              }
            }
          }
          if (!exist)
          {
            clocksWed[numberOfAlarms[2]] = clockData;
            minWed[numberOfAlarms[2]] = minData;
            numberOfAlarms[2] = numberOfAlarms[2] + 1;
          }
        }
      }
      else if (comMsg == 'D')
      {
        if (numberOfAlarms[2] != 0)
        {
          for (int i = 0; i < numberOfAlarms[2]; i++)
          {
            if (clocksWed[i] == clockData )
            {
              if (minWed[i] == minData )
              {
                clocksWed[i] = 0;
                minWed[i] = 0;
                numberOfAlarms[2] = numberOfAlarms[2] - 1;
                break;
              }
            }
          }
          // TODO: DELETE NOTHING ERROR v2
        }
        // TODO: NOTHING TO DELETE ERROR v2
      }
    }
    else if (dayOfWeek == 4)
    {

      exist = false;
      // TODO: ARRAY LIMIT REACHED ERROR v2
      if (comMsg == 'A')
      {

        if (numberOfAlarms[3] == 0) {

          clocksThu[0] = clockData;
          minThu[0] = minData;
          numberOfAlarms[3] = numberOfAlarms[3] + 1;

        }
        else
        {
          for (int i = 0; i < numberOfAlarms[3]; i++)
          {
            if (clocksThu[i] == clockData )
            {
              if (minThu[i] == minData)
              {
                exist = true;
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
                delay(1000);
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer

              }
            }
          }
          if (!exist)
          {
            clocksThu[numberOfAlarms[3]] = clockData;
            minThu[numberOfAlarms[3]] = minData;
            numberOfAlarms[3] = numberOfAlarms[3] + 1;
          }
        }
      }
      else if (comMsg == 'D')
      {
        if (numberOfAlarms[3] != 0)
        {
          for (int i = 0; i < numberOfAlarms[3]; i++)
          {
            if (clocksThu[i] == clockData )
            {
              if (minThu[i] == minData )
              {
                clocksThu[i] = 0;
                minThu[i] = 0;
                numberOfAlarms[3] = numberOfAlarms[3] - 1;
                break;
              }
            }
          }
          // TODO: DELETE NOTHING ERROR v2
        }
        // TODO: NOTHING TO DELETE ERROR v2
      }
    }
    else if (dayOfWeek == 5)
    {

      exist = false;
      // TODO: ARRAY LIMIT REACHED ERROR v2
      if (comMsg == 'A')
      {

        if (numberOfAlarms[4] == 0) {

          clocksFri[0] = clockData;
          minFri[0] = minData;
          numberOfAlarms[4] = numberOfAlarms[4] + 1;

        }
        else
        {
          for (int i = 0; i < numberOfAlarms[4]; i++)
          {
            if (clocksFri[i] == clockData )
            {
              if (minFri[i] == minData)
              {
                exist = true;
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
                delay(1000);
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
              }
            }
          }
          if (!exist)
          {
            clocksFri[numberOfAlarms[4]] = clockData;
            minFri[numberOfAlarms[4]] = minData;
            numberOfAlarms[4] = numberOfAlarms[4] + 1;
          }
        }
      }
      else if (comMsg == 'D')
      {
        if (numberOfAlarms[4] != 0)
        {
          for (int i = 0; i < numberOfAlarms[4]; i++)
          {
            if (clocksFri[i] == clockData )
            {
              if (minFri[i] == minData )
              {
                clocksFri[i] = 0;
                minFri[i] = 0;
                numberOfAlarms[4] = numberOfAlarms[4] - 1;
                break;
              }
            }
          }
          // TODO: DELETE NOTHING ERROR v2
        }
        // TODO: NOTHING TO DELETE ERROR v2
      }
    }
    else if (dayOfWeek == 6)
    {

      exist = false;
      // TODO: ARRAY LIMIT REACHED ERROR v2
      if (comMsg == 'A')
      {

        if (numberOfAlarms[5] == 0) {

          clocksSat[0] = clockData;
          minSat[0] = minData;
          numberOfAlarms[5] = numberOfAlarms[5] + 1;

        }
        else
        {
          for (int i = 0; i < numberOfAlarms[5]; i++)
          {
            if (clocksSat[i] == clockData )
            {
              if (minSat[i] == minData)
              {
                exist = true;
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
                delay(1000);
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
              }
            }
          }
          if (!exist)
          {
            clocksSat[numberOfAlarms[5]] = clockData;
            minSat[numberOfAlarms[5]] = minData;
            numberOfAlarms[5] = numberOfAlarms[5] + 1;
          }
        }
      }
      else if (comMsg == 'D')
      {
        if (numberOfAlarms[5] != 0)
        {
          for (int i = 0; i < numberOfAlarms[5]; i++)
          {
            if (clocksSat[i] == clockData )
            {
              if (minSat[i] == minData )
              {
                clocksSat[i] = 0;
                minSat[i] = 0;
                numberOfAlarms[5] = numberOfAlarms[5] - 1;
                break;
              }
            }
          }
          // TODO: DELETE NOTHING ERROR v2
        }
        // TODO: NOTHING TO DELETE ERROR v2
      }
    }
    else if (dayOfWeek == 7)
    {

      exist = false;
      // TODO: ARRAY LIMIT REACHED ERROR v2
      if (comMsg == 'A')
      {

        if (numberOfAlarms[6] == 0) {

          clocksSun[0] = clockData;
          minSun[0] = minData;
          numberOfAlarms[6] = numberOfAlarms[6] + 1;

        }
        else
        {
          for (int i = 0; i < numberOfAlarms[6]; i++)
          {
            if (clocksSun[i] == clockData )
            {
              if (minSun[i] == minData)
              {
                exist = true;
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer
                delay(1000);
                digitalWrite(buzzer, LOW); // Buzzer
                delay(1000);
                digitalWrite(buzzer, HIGH); // Buzzer

              }
            }
          }
          if (!exist)
          {
            clocksSun[numberOfAlarms[6]] = clockData;
            minSun[numberOfAlarms[6]] = minData;
            numberOfAlarms[6] = numberOfAlarms[6] + 1;
          }
        }
      }
      else if (comMsg == 'D')
      {
        if (numberOfAlarms[6] != 0)
        {
          for (int i = 0; i < numberOfAlarms[6]; i++)
          {
            if (clocksSun[i] == clockData )
            {
              if (minSun[i] == minData )
              {
                clocksSun[i] = 0;
                minSun[i] = 0;
                numberOfAlarms[0] = numberOfAlarms[0] - 1;
                break;
              }
            }
          }
          // TODO: DELETE NOTHING ERROR v2
        }
        // TODO: NOTHING TO DELETE ERROR v2
      }
    }

    comMsg = "";
    dayOfWeek = 0;
    clockData = 0;
    minData = 0;
    stringComplete = false;
//    Serial.println("del");
  }
}

// Reading the Serial Input
void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    input.concat(inChar);
    if (inChar == '!')
    {
      stringComplete = true;
      break;
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
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonMon);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[0] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonMon);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[0] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
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
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonTue);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[1] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonTue);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[1] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
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
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonWed);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[2] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonWed);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[2] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
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
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonThu);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[3] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonThu);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[3] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
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
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonFri);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[4] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonFri);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[4] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
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
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonSat);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[5] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonSat);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[5] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
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
      digitalWrite(buzzer, LOW); // Buzzer
      digitalWrite(lightControl, HIGH); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonSun);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[6] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
      digitalWrite(buzzer, HIGH); // Buzzer
      digitalWrite(lightControl, LOW); // Led
      delay(delayPeriod);
      buttonState = digitalRead(buttonSun);
      if (buttonState == HIGH)
      {
        // Box Opened Close Alarm
        alarmStatus[6] = 0;
        digitalWrite(buzzer, HIGH); // Buzzer
        digitalWrite(lightControl, LOW); // Led
        delay(40000);
        break;
      }
    }
  }
}

// END
