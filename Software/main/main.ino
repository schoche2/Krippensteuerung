#include <SoftPWM.h>


#define OUT1    0;
#define OUT2    1;
#define OUT3    2;
#define OUT4    3;
#define OUT5    4;
#define OUT6    5;
#define OUT7    18;
#define OUT8    19;


SOFTPWM_DEFINE_CHANNEL(0,   DDRD, PORTD, PORTD0);  //Arduino pin 0
SOFTPWM_DEFINE_CHANNEL(1,   DDRD, PORTD, PORTD1);  //Arduino pin 1
SOFTPWM_DEFINE_CHANNEL(2,   DDRD, PORTD, PORTD2);  //Arduino pin 2
SOFTPWM_DEFINE_CHANNEL(3,   DDRD, PORTD, PORTD3);  //Arduino pin 3
SOFTPWM_DEFINE_CHANNEL(4,   DDRD, PORTD, PORTD4);  //Arduino pin 4
SOFTPWM_DEFINE_CHANNEL(5,   DDRD, PORTD, PORTD5);  //Arduino pin 5
SOFTPWM_DEFINE_CHANNEL(18,  DDRC, PORTC, PORTC4);  //Arduino pin A4
SOFTPWM_DEFINE_CHANNEL(19,  DDRC, PORTC, PORTC5);  //Arduino pin A5

SOFTPWM_DEFINE_OBJECT(8);



class Flame
{
  public:
    void setup(int pin, int alpha = 20, int delay = 100);
    void flicker();

  private:
    int _pin;
    int _brightness = 0;  // soft start
    int _oldBrightness = 0;
    int _updateDelay;
    unsigned long _nextUpdate;
    int _alpha; // filter coefficient (0..100). Low means slow changes
};


void Flame::setup(int pin, int alpha, int delay)
{
  constrain(alpha, 0, 100);
  _pin = pin;
  _alpha = alpha;
  _updateDelay = delay;
  // random first upate to prevent multiple flames looking synchronous
  _nextUpdate  = millis() + random(_updateDelay);
}


void Flame::flicker(int min, int max)
{
  if (millis() >= _nextUpdate) {
    _nextUpdate += _updateDelay; 
    _brightness = random(min, max);
    // low pass filter the brightness changes
    _brightness = (_alpha * _brightness + (100 - _alpha) * _oldBrightness) / 100;
    _oldBrightness = _brightness;

    Palatis::SoftPWM.set(_pin, _brightness);
  }
}



class Light
{
  public:
    void setup(int pin, int percen = 100);
};


void Light::setup(int pin, int percent)
{
  constrain(percent, 0, 100);
  int pwm = (255) / 100 * percent;

  Palatis::SoftPWM.set(pin, pwm);
}



Light light1;
Light light2;
Light light3;
Light light4;

Flame flame5;
Flame flame6;
Flame flame7;
Flame flame8;


void setup()
{
  Palatis::SoftPWM.begin(60);

  light1.setup(OUT1, 25   );
  light2.setup(OUT2, 50   );
  light3.setup(OUT3, 75   );
  light4.setup(OUT4, 100  );

  flame5.setup(OUT5, 5,   100);
  flame6.setup(OUT6, 10,  150);
  flame7.setup(OUT7, 15,  200);
  flame8.setup(OUT7, 20,  250);
}


void loop()
{
  flame5.flicker(0, 255);
  flame6.flicker(0, 255);
  flame7.flicker(0, 255);
  flame8.flicker(0, 75);
}