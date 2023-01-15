#include <SoftPWM.h>


#define OUT1    0        // Lagerfeuer
#define OUT2    1        // Ofen
#define OUT3    2        // Kellerlicht
#define OUT4    3        // Küche
#define OUT5    4        // Stall links
#define OUT6    5        // Stall rechts
#define OUT7    19       // 
#define OUT8    18       // Laterne


SOFTPWM_DEFINE_CHANNEL(0,   DDRD, PORTD, PORTD0);  //Arduino pin 0
SOFTPWM_DEFINE_CHANNEL(1,   DDRD, PORTD, PORTD1);  //Arduino pin 1
SOFTPWM_DEFINE_CHANNEL(2,   DDRD, PORTD, PORTD2);  //Arduino pin 2
SOFTPWM_DEFINE_CHANNEL(3,   DDRD, PORTD, PORTD3);  //Arduino pin 3
SOFTPWM_DEFINE_CHANNEL(4,   DDRD, PORTD, PORTD4);  //Arduino pin 4
SOFTPWM_DEFINE_CHANNEL(5,   DDRD, PORTD, PORTD5);  //Arduino pin 5
SOFTPWM_DEFINE_CHANNEL(18,  DDRC, PORTC, PORTC4);  //Arduino pin A4
SOFTPWM_DEFINE_CHANNEL(19,  DDRC, PORTC, PORTC5);  //Arduino pin A5

SOFTPWM_DEFINE_OBJECT(20);



class Flame
{
  public:
    void setup(int pin, int alpha = 20, int delay = 100);
    void flicker(int min, int max);

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



Flame flame1;
Flame flame2;
Light light3;
Light light4;
Light light5;
Light light6;
Light light7;
Flame flame8;


void setup()
{
  Palatis::SoftPWM.begin(60);

  flame1.setup(OUT1, 70,  200);
  flame2.setup(OUT2, 70,  200);
  light3.setup(OUT3, 50);
  light4.setup(OUT4, 60);
  light5.setup(OUT5, 80);
  light6.setup(OUT6, 80);
  light7.setup(OUT7, 80);
  flame8.setup(OUT8, 20, 400);
}


void loop()
{
  flame1.flicker(0, 155);
  flame2.flicker(0, 150);
  flame8.flicker(0, 155);
}