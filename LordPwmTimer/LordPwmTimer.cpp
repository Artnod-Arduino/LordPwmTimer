#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif
#include "LordPwmTimer.h"

LordPwmTimer::LordPwmTimer(int IO_Pin)
{
	_IO_Pin		= IO_Pin;
	_isWorking	= false;
	_isStarted	= false;
	_isEnable	= false;
	_pwm		= 0;
	_lastDay	= 0;
	pinMode(_IO_Pin,OUTPUT);
	analogWrite(_IO_Pin, 0);
	_data[LORDTIMER_PWM_MIN] = 0;
	_data[LORDTIMER_PWM_MAX] = 255;
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
		// check for new Sun Rise/Set
		checkSun(now);
		// change pwm
		int timeMin = (now.hour()*60) + now.minute();
		bool new_state = runCycle(timeMin);
		if(new_state != _isWorking)
		{
			unsigned long timeSec = (now.hour()*3600) + (now.minute()*60) + now.second();
			if(new_state) incrementPwm(timeSec);
			else decrementPwm(timeSec);
		}
	}
}

bool LordPwmTimer::isWorking(void)
{
	return (_isWorking || _isStarted);
}

int LordPwmTimer::getPwm(void)
{
	return _pwm;
}


void LordPwmTimer::enable(bool value)
{
	if(value != _isEnable)
	{
		if(_isWorking)
		{
			_isWorking = false;
			_pwm = 0;
			digitalWrite(_IO_Pin, _pwm);
		}
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

bool LordPwmTimer::runCycle(int timeMin)
{
	if((timeMin >= _data[LORDTIMER_ON]) && (timeMin < _data[LORDTIMER_OFF])) return true;
	return false;
}

void LordPwmTimer::incrementPwm(unsigned long timeSec)
{
	int pwmRange = _data[LORDTIMER_PWM_MAX] - _data[LORDTIMER_PWM_MIN];
	float nbPerSec = pwmRange / _data[LORDTIMER_PWM_TIME];
	unsigned long startOn = _data[LORDTIMER_ON] *60;
	unsigned long endOn = startOn + _data[LORDTIMER_PWM_TIME];
	
	if(timeSec >= startOn && timeSec <= endOn)
	{
		if(_isStarted == false) _isStarted = true;
		float pmwVal = _data[LORDTIMER_PWM_MIN] + (nbPerSec * (timeSec - startOn));
		if(pmwVal <= _data[LORDTIMER_PWM_MAX])
		{
			_pwm = pmwVal;
			analogWrite(_IO_Pin, _pwm);
		}
		else
		{
			_isWorking = true;
			_isStarted = false;
		}
	}
	else 
	{
		_pwm = _data[LORDTIMER_PWM_MAX];
		analogWrite(_IO_Pin, _pwm);
		_isWorking = true;
	}
}

void LordPwmTimer::decrementPwm(unsigned long timeSec)
{
	int pwmRange = _data[LORDTIMER_PWM_MAX] - _data[LORDTIMER_PWM_MIN];
	float nbPerSec = pwmRange / _data[LORDTIMER_PWM_TIME];
	unsigned long endOff = _data[LORDTIMER_OFF] *60;
	unsigned long startOff = endOff - _data[LORDTIMER_PWM_TIME];
	
	if(timeSec >= startOff && timeSec <= endOff)
	{
		if(_isStarted == false) _isStarted = true;
		float pmwVal = _data[LORDTIMER_PWM_MAX] - (nbPerSec * (timeSec - startOff));
		if(pmwVal >= _data[LORDTIMER_PWM_MIN])
		{
			_pwm = pmwVal;
			analogWrite(_IO_Pin, _pwm);
		}
		else
		{
			_isWorking = false;
			_isStarted = false;
		}
	}
	else 
	{
		_pwm = _data[LORDTIMER_PWM_MIN];
		analogWrite(_IO_Pin, _pwm);
		_isWorking = false;
	}
}
