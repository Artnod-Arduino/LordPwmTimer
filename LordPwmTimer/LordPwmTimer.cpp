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
		if(now.day() != _lastDay)
		{
			byte sunTime[] = {0, 0, 0, now.day(), now.month(), now.year()};
			_myLord.SunRise(sunTime);
			_data[LORDTIMER_ON] = sunTime[2] * 60 + sunTime[1];
			_myLord.SunSet(sunTime);
			_data[LORDTIMER_OFF] = sunTime[2] * 60 + sunTime[1];
			_lastDay = now.day();
		}
		// change pwm
		bool new_state = runCycle((now.hour()*60)+now.minute());	
		if(new_state != _isWorking)
		{
			if(new_state) incrementPwm(now.unixtime());
			else decrementPwm(now.unixtime());
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


bool LordPwmTimer::runCycle(int now)
{
	bool working = false;
	if(_data[LORDTIMER_ON] < _data[LORDTIMER_OFF] ) // operation le jour meme
	{
		if((now >= _data[LORDTIMER_ON]) && (now < _data[LORDTIMER_OFF])) // jour
		{
			working = true;
		}
		else // nuit
		{
			working = false;
		}
	}  
	else if(_data[LORDTIMER_ON] > _data[LORDTIMER_OFF])  // opération chevauche 2 jours 
	{
		if((now < _data[LORDTIMER_ON]) && (now >= _data[LORDTIMER_OFF])) // nuit
		{
			working = false;
		}
		else // jour
		{
			working = true;
		}
	}
	return working;
}

void LordPwmTimer::incrementPwm(unsigned long unixTime)
{
	float nbPerSec = 255.0 / _data[LORDTIMER_PWM_TIME];
	if(_isStarted == false)
	{
		_PwmStartTime = unixTime;
		_isStarted = true;
	}
	else
	{
		float pmwVal = nbPerSec * (unixTime - _PwmStartTime);
		if(pmwVal <= 255)
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
}

void LordPwmTimer::decrementPwm(unsigned long unixTime)
{
	float nbPerSec = 255.0 / _data[LORDTIMER_PWM_TIME];
	if(_isStarted == false)
	{
		_PwmStartTime = unixTime;
		_isStarted = true;
	}
	else
	{
		float pmwVal = nbPerSec * (unixTime - _PwmStartTime);
		if(pmwVal >= 0)
		{
			_pwm = pmwVal;
			digitalWrite(_IO_Pin, _pwm);
		}
		else
		{
			_isWorking = true;
			_isStarted = false;
		}
	}
}