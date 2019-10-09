# RGBLamp

This code is intended for use with a programmable LED lamp using any LED type
supported by the [FastLED library](https://github.com/FastLED/FastLED), two
potentiometers, and a momentary switch (capacitive touch sensor works to great
effect here). It has three modes:

1) White w/ color temperature and brightness adjustment  
2) Full wheel w/ color and brightness adjustment  
3) Fire simulation w/ sparking and airflow adjustment

The switch is used to turn the lamp on and off with a short press or to switch
modes with a long press.

## Dependencies
[FastLED](https://github.com/FastLED/FastLED)

## Board compatibility
Any microcontroller supported by the [FastLED library](https://github.com/FastLED/FastLED)
that also has at least two analog inputs and two digital GPIO. (e.g. Arduino Uno,
    Nano, Mega, etc.)
