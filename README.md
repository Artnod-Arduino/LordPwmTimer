# A METTRE A JOUR
# DailyTimer 
Minuterie journalière avec chevauchement sur 2 jours. Active ou désactive une broche selon l'heure.

## Constantes
 * `LORDTIMER_PWM_TIME` = 0
 * `LORDTIMER_PWM_MIN` = 1
 * `LORDTIMER_PWM_MAX` = 2
 * `LORDTIMER_ON`  = 3
 * `LORDTIMER_OFF`  = 4

## Méthodes
* [LordPwmTimer()](#lordpwmtimerio_pin)
* [getValue()](#getvaluetype)
* [setValue(val)](#setvaluetype-value)
* [setLord(val)](#setlordtimezone-latitude-longitude)
* [run()](#runnow)
* [getPwm()](#getpwm)
* [enable()](#enablevalue)
* [isEnable()](#isenable)

-------------

### LordPwmTimer(IO_Pin)
 Instancie l'objet LordPwmTimer.
 
Paramètres	  | Type | Requis | Description
------------- | ---- | ------ | -----------
IO_Pin		  | int  | Oui	  | Numéro de la broche utilisé.


-------------

### getValue(type)
Renvoie le paramètre souhaité sous la fourme d'un entier, représentant un nombre de minute depuis minuit.

Paramètres | Type | Requis | Description
---------- | ---- | ------ | -----------
type	   | int  | Oui	   | Utiliser les constantes.


-------------

### setValue(type, value)
Permet de configurer un paramètre.

Paramètres | Type | Requis | Description
---------- | ---- | ------ | -----------
type	   | int  | Oui	   | Utiliser les constantes.
value	   | int  | Oui	   | Nombre de minute depuis minuit. Exemple pour *10:27* `minute = (10*60) + 27`.


-------------

### setLord(timeZone, latitude, longitude)
Permet de configurer un paramètre.

Paramètres | Type | Requis | Description
---------- | ---- | ------ | -----------
timeZone	   | int  | Oui	   | +2 pour la France
latitude	   	   | float  | Oui	   | Nice = latitude = 43.70
longitude	       | float  | Oui	   | Nice = longitude = 7.25


-------------

### run(now)
Lance l'analyse.

Paramètres	  | Type | Requis | Description
------------- | ---- | ------ | -----------
currentTime	  | DateTime  | Oui	  | Heure actuelle.


-------------

### getPwm()
Renvoie la valeur du pwm de la broche. Entre 0 et 255.


-------------

### enable(value)
Active ou désactive le timer.

Paramètres	  | Type | Requis | Description
------------- | ---- | ------ | -----------
value		  | bool | Non	  | Par défaut égal `true`. Active `true` ou désactive `false` le timer.


-------------

### isEnable()
Renvoie un booléen si le timer est actif ou non.


-------------
## Exemple
  * [Simple.ino](https://github.com/Artnod-Arduino/LordPwmTimer/blob/master/LordPwmTimer/examples/Simple/Simple.ino)
