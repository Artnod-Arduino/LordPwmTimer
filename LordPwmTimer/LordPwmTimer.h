#ifndef LordPwmTimer_h
#define LordPwmTimer_h

#include <TimeLord.h>
#include <RTClib.h>

#define LORDTIMER_ON		0
#define LORDTIMER_OFF		1
#define LORDTIMER_PWM_TIME	2

class LordPwmTimer
{
	public:
		// Déclaration de l'objet
		LordPwmTimer(int IO_Pin);
		
		// renvoie le paramètre
		int		getValue(int type);
		// permet de modifier les valeurs par catégorie de parametrage
		void	setValue(int type, int value);
		// permet de configurer le TimeLord
		void	setLord(int timeZone, float latitude, float logitude);
		// lance l'analyse
		// currentTime = nombre de minute depuis minuit (22:30 -> 22*60 + 30 = 1350)
		void	run(DateTime now);
		// renvoie un booléen signifiant sont état
		bool	isWorking(void);
		// renvoie un int signifiant le pwm
		int		getPwm(void);
		
		// active ou desactive le timer
		void	enable(bool value = true);		
		//renvoie un booléen s'il est activé
		bool	isEnable(void);
		
		
	private:
		int		_IO_Pin;
		TimeLord _myLord;
		int		_data[3];
		bool	_isWorking;
		bool	_isStarted;
		int		_pwm;
		bool	_isEnable;
		int		_lastDay;
		unsigned long _PwmStartTime;
		
		bool	runCycle(int now);
		void	incrementPwm(unsigned long timeSec);
		void	decrementPwm(unsigned long timeSec);
};
#endif