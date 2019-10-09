/*
 * RGBLamp by Michael Ploof, September 2019
 * 
 * Fire simulator adapted from FastLED example, Fire2012 by Mark Kriegsman, July 2012
 * as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
 * 
 */

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    7
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    64
CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND  100

const int POT_BRIGHT = A1;
const int POT_RGB = A0;
const int BUTTON = 3;

int brightness = 0;
bool led_on = true;

typedef enum _mode {
  WHITE,
  COLOR,
  FIRE
} MODE;

MODE mode = WHITE;

void setup() {
  Serial.begin(115200);

  //delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(brightness);

  pinMode(POT_RGB, INPUT);
  pinMode(POT_BRIGHT, INPUT);
  pinMode(BUTTON, INPUT);

}

void loop() {

  switch (mode) {
    case COLOR:
      {
        updateColor();
        break;
      }
    case WHITE:
      {
        updateWhite();
        break;
      }
    case FIRE:
      {
        fireSim();
        break;
      }
  }

  FastLED.delay(1000 / FRAMES_PER_SECOND);

  checkButton();

}

void checkButton() {
  static long last_start = millis();
  static bool was_pressed = false;
  bool pressed = digitalRead(BUTTON);
  const int THRESHOLD = 350;

  if (pressed && !was_pressed) {
    last_start = millis();
  }
  else if (!pressed && was_pressed) {
    long press_time = millis() - last_start;
    if (press_time > THRESHOLD && led_on) {
      if (mode == WHITE) {
        mode = COLOR;
      }
      else if (mode == COLOR) {
        mode = FIRE;
      }
      else if (mode == FIRE) {
        mode = WHITE;
      }
    }
    else {
      toggleLEDPower();
    }
  }
  was_pressed = pressed;
}

void toggleLEDPower() {
  led_on = !led_on;
  if (led_on) {
    FastLED.setBrightness(brightness);
  }
  else {
    FastLED.setBrightness(0);
  }
}

void updateColor() {

  int hue = map(analogRead(POT_RGB), 0, 1023, 0, 360);
  CRGB new_color = colorWheel(hue);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = new_color;
  }

  brightness = map(analogRead(POT_BRIGHT), 0, 1023, 0, 255);
  if (led_on) {
    FastLED.setBrightness(brightness);
  }
  FastLED.show();

}


void updateWhite() {

  int raw_val = analogRead(POT_RGB);
  int adjusted_val = sqrt(raw_val * 10);
  int white_temp = map(adjusted_val, 101, 0, 2000, 6000);

  int g = 99.7408025861 * log((float)white_temp / 100.0) - 161.1195681661;
  int b = 138.5177615561 * log((float)white_temp / 100.0 - 10) - 305.0447927307;
  if (white_temp < 2000) {
    b = 0;
  }

  String output = "Raw: " + String(raw_val) + " Adj: " + String(adjusted_val) + " Temp: " + String(white_temp) + "k -- G: " + String(g) + " -- B:" + String(b) + "\n";
  Serial.println(output);

  CRGB new_color = CRGB(255, g, b);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = new_color;
  }

  brightness = map(analogRead(POT_BRIGHT), 0, 1023, 0, 255);
  if (led_on) {
    FastLED.setBrightness(brightness);
  }
  FastLED.show();

}


CRGB colorWheel(int hue) {
  int r, g, b;

  if (hue >= 0 && hue < 60) {
    r = 255;
    b = 0;
    g = (float)hue / 60 * 255;
  }
  else if (hue >= 60 && hue < 120) {
    r = (1.0 - ((float)hue - 60) / 60) * 255;
    b = 0;
    g = 255;
  }
  else if (hue >= 120 && hue < 180) {
    r = 0;
    b = ((float) hue - 120) / 60 * 255;
    g = 255;
  }
  else if (hue >= 180 && hue < 240) {
    r = 0;
    b = 255;
    g = (1.0 - ((float)hue - 180) / 60) * 255;;
  }
  else if (hue >= 240 && hue < 300) {
    r = ((float) hue - 240) / 60 * 255;;
    b = 255;
    g = 0;
  }
  else if (hue >= 300 && hue <= 360) {
    r = 255;
    b = (1.0 - ((float)hue - 300) / 60) * 255;
    g = 0;
  }

  return CRGB(r, g, b);
}

CRGB rainbow(int hue) {
  int r, g, b;

  if (hue >= 0 && hue < 60) {
    r = (float)hue / 60 * 255;
    b = 0;
    g = 0;
  }
  else if (hue >= 60 && hue < 120) {
    r = 255;
    b = 0;
    g = ((float)hue - 60) / 60 * 255;
  }
  else if (hue >= 120 && hue < 180) {
    r = (1.0 - ((float)hue - 120) / 60) * 255;
    b = 0;
    g = 255;
  }
  else if (hue >= 180 && hue < 240) {
    r = 0;
    b = ((float) hue - 180) / 60 * 255;
    g = 255;
  }
  else if (hue >= 240 && hue < 300) {
    r = 0;
    b = 255;
    g = (1.0 - ((float)hue - 240) / 60) * 255;
  }
  else if (hue >= 300 && hue <= 360) {
    r = 0;
    b = (1.0 - ((float)hue - 300) / 60) * 255;
    g = 0;
  }

  return CRGB(r, g, b);
}


// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
int cooling = 55;

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
int sparking = 120;

bool gReverseDirection = false;


void fireSim()
{
  cooling = map(analogRead(POT_BRIGHT), 0, 1023, 20, 100);
  if (led_on) {
    FastLED.setBrightness(brightness);
  }

  sparking = map(analogRead(POT_RGB), 0, 1023, 50, 200);

  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < sparking ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }


  FastLED.show(); // display this frame
}
