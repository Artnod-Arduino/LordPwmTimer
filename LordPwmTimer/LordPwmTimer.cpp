#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif
#include "LordPwmTimer.h"

LordPwmTimer::LordPwmTimer(int IO_Pin)
{
	_IO_Pin		= IO_Pin;
	_isEnable	= false;
	_data[LORDTIMER_PWM_MIN] = 0;
	_data[LORDTIMER_PWM_MAX] = 255;
	_pwm		= _data[LORDTIMER_PWM_MIN];
	_lastDay	= 0;
	pinMode(_IO_Pin,OUTPUT);
	analogWrite(_IO_Pin, _pwm);
}


int LordPwmTimer::getValue(int type)
{
	return _data[type];
}

void LordPwmTimer::setValue(int type, int value)
{
	_data[type] = value;
}

void LordPwmTimer::setLord(int timeZone, float latitude, float longitude)
{
	_myLord.TimeZone(timeZone * 60);
	_myLord.Position(latitude, longitude);
}


void LordPwmTimer::run(DateTime now)
{
	if(_isEnable)
	{
		checkSun(now); // check for new Sun Rise/Set
		unsigned long timeSec = (unsigned long)(now.hour()) * 3600 + (now.minute()*60) + now.second(); // time in sec from 00h00:00
		runPwm(timeSec); // change pwm
	}
}
int LordPwmTimer::getPwm(void)
{
	return _pwm;
}


void LordPwmTimer::enable(bool value)
{
	if(value != _isEnable)
	{
		if(value == false) _pwm = 0;
		digitalWrite(_IO_Pin, _pwm);
		_isEnable = value;
	}
}

bool LordPwmTimer::isEnable(void)
{
	return _isEnable;
}


void LordPwmTimer::checkSun(DateTime now)
{
	if(now.day() != _lastDay)
	{
		byte sunTime[] = {0, 0, 0, now.day(), now.month(), now.year()};
		_myLord.SunRise(sunTime);
		_data[LORDTIMER_ON] = sunTime[2] * 60 + sunTime[1];
		_myLord.SunSet(sunTime);
		_data[LORDTIMER_OFF] = sunTime[2] * 60 + sunTime[1];
		_lastDay = now.day();
	}
}

void LordPwmTimer::runPwm(unsigned long timeSec)
{
	int pwmRange = _data[LORDTIMER_PWM_MAX] - _data[LORDTIMER_PWM_MIN];
	float nbPerSec = (float)(pwmRange) / _data[LORDTIMER_PWM_TIME];
	unsigned long startOn = (unsigned long)(_data[LORDTIMER_ON]) *60;
	unsigned long endOn = startOn + _data[LORDTIMER_PWM_TIME];
	unsigned long startOff = (unsigned long)(_data[LORDTIMER_OFF]) *60;
	unsigned long endOff = startOff + _data[LORDTIMER_PWM_TIME];
	
	if(timeSec < startOn) // off = _data[LORDTIMER_PWM_MIN]
	{
		if(_pwm != _data[LORDTIMER_PWM_MIN])
		{
			_pwm = _data[LORDTIMER_PWM_MIN];
			analogWrite(_IO_Pin, _pwm);
		}
	}
	else if((timeSec >= startOn)  && (timeSec <= endOn)) // switch on = pwm increment
	{
		float pwmVal = _data[LORDTIMER_PWM_MIN] + (nbPerSec * (timeSec - startOn));
		int decimal = 100 * (pwmVal - (int)(pwmVal));
		if(decimal > 50) pwmVal = (int)(pwmVal) + 1;
		else pwmVal = (int)(pwmVal);
		if((int)(pwmVal) != _pwm)
		{
			_pwm = pwmVal;
			analogWrite(_IO_Pin, _pwm);
		}
	}
	else if((timeSec > endOn)    && (timeSec < startOff)) // on = _data[LORDTIMER_PWM_MAX]
	{
		if(_pwm != _data[LORDTIMER_PWM_MAX])
		{
			_pwm = _data[LORDTIMER_PWM_MAX];
			analogWrite(_IO_Pin, _pwm);
		}
	}
	else if((timeSec >= startOff) && (timeSec <= endOff)) // switch off = pwm decrement
	{
		float pwmVal = _data[LORDTIMER_PWM_MAX] - (nbPerSec * (timeSec - startOff));
		int decimal = 100 * (pwmVal - (int)(pwmVal));
		if(decimal > 50) pwmVal = (int)(pwmVal) + 1;
		else pwmVal = (int)(pwmVal);
		if((int)(pwmVal) != _pwm)
		{
			_pwm = pwmVal;
			analogWrite(_IO_Pin, _pwm);
		}
	}
	else if(timeSec > endOff) // off = _data[LORDTIMER_PWM_MIN]
	{
		if(_pwm != _data[LORDTIMER_PWM_MIN])
		{
			_pwm = _data[LORDTIMER_PWM_MIN];
			analogWrite(_IO_Pin, _pwm);
		}
	}
}