#include <Wire.h>
#include <RTClib.h>
#include <LordPwmTimer.h>

#define TIMEZONE 2 // UTC france = +2
const float LATITUDE = 43.70; // Nice = latitude = 43.7000000
const float LONGITUDE = 7.25; // Nice = longitude = 7.250000

#define LIGHT_PIN 11
#define PWM_TIME 900

RTC_DS1307 RTC; // declaration de l'horloge
LordPwmTimer light(LIGHT_PIN); // declaration de la minuterie

void setup()
{
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) RTC.adjust(DateTime(__DATE__, __TIME__));
  light.setLord(TIMEZONE, LATITUDE, LONGITUDE);
  light.setValue(LORDTIMER_PWM_TIME, PWM_TIME);
  light.enable();
}

void loop()
{
  DateTime now = RTC.now();
  light.run(now);
}
