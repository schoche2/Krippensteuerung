#include <SoftPWM.h>

SOFTPWM_DEFINE_CHANNEL(0, DDRD, PORTD, PORTD0);  //Arduino pin 0
SOFTPWM_DEFINE_CHANNEL(1, DDRD, PORTD, PORTD1);  //Arduino pin 1
SOFTPWM_DEFINE_CHANNEL(2, DDRD, PORTD, PORTD2);  //Arduino pin 2
SOFTPWM_DEFINE_CHANNEL(3, DDRD, PORTD, PORTD3);  //Arduino pin 3
SOFTPWM_DEFINE_CHANNEL(4, DDRD, PORTD, PORTD4);  //Arduino pin 4
SOFTPWM_DEFINE_CHANNEL(5, DDRD, PORTD, PORTD5);  //Arduino pin 5
SOFTPWM_DEFINE_CHANNEL(6, DDRD, PORTD, PORTD6);  //Arduino pin 6
SOFTPWM_DEFINE_CHANNEL(7, DDRD, PORTD, PORTD7);  //Arduino pin 7
SOFTPWM_DEFINE_CHANNEL(8, DDRB, PORTB, PORTB0);  //Arduino pin 8
SOFTPWM_DEFINE_CHANNEL(9, DDRB, PORTB, PORTB1);  //Arduino pin 9
SOFTPWM_DEFINE_CHANNEL(10, DDRB, PORTB, PORTB2);  //Arduino pin 10
SOFTPWM_DEFINE_CHANNEL(11, DDRB, PORTB, PORTB3);  //Arduino pin 11
SOFTPWM_DEFINE_CHANNEL(12, DDRB, PORTB, PORTB4);  //Arduino pin 12
SOFTPWM_DEFINE_CHANNEL(13, DDRB, PORTB, PORTB5);  //Arduino pin 13
SOFTPWM_DEFINE_CHANNEL(14, DDRC, PORTC, PORTC0);  //Arduino pin A0
SOFTPWM_DEFINE_CHANNEL(15, DDRC, PORTC, PORTC1);  //Arduino pin A1
SOFTPWM_DEFINE_CHANNEL(16, DDRC, PORTC, PORTC2);  //Arduino pin A2
SOFTPWM_DEFINE_CHANNEL(17, DDRC, PORTC, PORTC3);  //Arduino pin A3
SOFTPWM_DEFINE_CHANNEL(18, DDRC, PORTC, PORTC4);  //Arduino pin A4
SOFTPWM_DEFINE_CHANNEL(19, DDRC, PORTC, PORTC5);  //Arduino pin A5

SOFTPWM_DEFINE_OBJECT(20);

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


void Flame::setup(int pin, int alpha, int delay) {
  constrain(alpha, 0, 100);
  _pin = pin;
  _alpha = alpha;
  _updateDelay = delay;
  // random first upate to prevent multiple flames looking synchronous
  _nextUpdate  = millis() + random(_updateDelay);
}


void Flame::flicker() {
  if (millis() >= _nextUpdate) {
    _nextUpdate += _updateDelay; 
    _brightness = random(0, 255);
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



Flame flames [20];


//Light light1;

void setup()
{
  Palatis::SoftPWM.begin(60);

  for (int i = 0; i <= 19; i++)
  {
    flames[i].setup(i, i, 200 + i);
  }
  //light1.setup(1,123);
}


void loop()
{
  for (int i = 0; i <= 19; i++)
  {
    flames[i].flicker();
  }
}