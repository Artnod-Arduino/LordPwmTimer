#ifndef LordPwmTimer_h
#define LordPwmTimer_h

#include <TimeLord.h>
#include <RTClib.h>

#define LORDTIMER_PWM_TIME	0
#define LORDTIMER_PWM_MIN	1
#define LORDTIMER_PWM_MAX	2
#define LORDTIMER_ON		3
#define LORDTIMER_OFF		4

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
		void	run(DateTime now);
		// renvoie un int signifiant le pwm
		int		getPwm(void);
		
		// active ou desactive le timer
		void	enable(bool value = true);		
		//renvoie un booléen s'il est activé
		bool	isEnable(void);
		
		
	private:
		int		_IO_Pin;
		TimeLord _myLord;
		int		_data[5];
		int		_pwm;
		bool	_isEnable;
		int		_lastDay;
		
		void	checkSun(DateTime now);
		void	runPwm(unsigned long timeSec);
};
#endif