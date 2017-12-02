//test capacitive sensor with neopixel

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

#define PIXEL_PIN1   6    // Digital IO pin connected to the NeoPixels.
#define PIXEL_PIN2   7
//#define PIXEL_PIN3   8
//#define PIXEL_PIN4   9
//#define PIXEL_PIN5   10
//#define PIXEL_PIN6   11

#define PIXEL_COUNT 24

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN1, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN2, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN3, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN4, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN5, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN6, NEO_GRBW + NEO_KHZ800);

// R,G,B,W
const uint32_t RED = strip1.Color(255, 0, 0, 0);//strip1
const uint32_t BLUE = strip1.Color(0, 72, 130, 0);//strip2.strip6(last)
const uint32_t GREEN = strip1.Color(0, 127, 0, 0);//strip3
const uint32_t PURPLE = strip1.Color(158, 8, 148, 0);//strip4
const uint32_t YELLOW = strip1.Color(255, 100, 0, 0);//strip5
const uint32_t ORANGE = strip1.Color(150, 7, 0, 0);//strip6

const int Days = 7;

boolean oldState = LOW;
unsigned int showType = 0;


void setup() {
  Serial.begin( 9600);
  while (!Serial);        // needed to keep leonardo/micro from starting too fast!

  Serial.println("Start");

  //pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip1.begin();
  strip1.show();
  // strip2.begin();
  // strip2.show();
  // strip3.begin();
  // strip3.show();
  // strip4.begin();
  // strip4.show();
  // strip5.begin();
  // strip5.show();
  // strip6.begin();
  // strip6.show();// Initialize all pixels to 'off'
  Serial.println("Neopixels setup");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  showType = 0;

  Serial.println("Begin");
}

void loop() {
  // debug
  if (Serial.available() > 0) {
    int val = Serial.read() ;
    if (val >= '0' && val <= '9') {
      showType = val - '0';
      Serial.print("MAN "); Serial.println(showType);
      startShow(showType  );
    }
  }

  // Get the currently touched pads
  currtouched = cap.touched(); // bitmap

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
      startShow( showType + 1  );
      showType = (showType + 1) % Days; // 0..6
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
    }
  }
  // reset our state, i.e. dont' trigger again till untouch
  lasttouched = currtouched;

  // comment out this line for detailed data from the sensor!
  return;

  // debugging info, raw values
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();

  // put a delay so it isn't overwhelming
  delay(100);
}


void startShow(int i) {
  Serial.print("Show "); Serial.println(i);

  switch (i) {

    case 1:
      theaterChase(strip1, RED, 50);
      off(strip1);
      break;

    case 2:
      theaterChase(strip1, RED, 50);
      off(strip1);    // Black/off
      theaterChase(strip4, PURPLE, 50);
      off(strip4);    // Black/off
      theaterChase(strip2, BLUE, 50);
      off(strip2);
      theaterChase(strip5, YELLOW, 50);
      off(strip5);
      break;

    case 3:
      theaterChase(strip3, GREEN, 50);
      off(strip3);    // Black/off
      theaterChase(strip2, BLUE, 50);  // Blue
      off(strip2);    // Black/off
      theaterChase(strip4, PURPLE, 50);    // purple
      off(strip4);    // Black/off
      break;

    case 4:
      theaterChase(strip2, BLUE, 50);
      off(strip2);
      theaterChase(strip1, RED, 50);
      off(strip1);
      theaterChase(strip5, YELLOW, 50);
      off(strip5);
      theaterChase(strip3, GREEN, 50);
      off(strip3);
      break;


    case 5:
      theaterChase(strip4, PURPLE, 50);
      off(strip4);
      theaterChase(strip3, GREEN, 50);
      off(strip3);
      theaterChase(strip2, BLUE, 50);
      off(strip2);
      theaterChase(strip1, RED, 50);
      off(strip1);
      break;


    case 6:
      theaterChase(strip5, YELLOW, 50);
      off(strip5);
      theaterChase(strip1, RED, 50);
      off(strip1);
      theaterChase(strip4, PURPLE, 50);
      off(strip4);
      theaterChase(strip3, GREEN, 50);
      off(strip3);
      break;


    case 7:
      theaterChase(strip1, RED, 50);
      off(strip1);
      theaterChase(strip3, GREEN, 50);
      off(strip3);
      theaterChase(strip2, BLUE, 50);
      off(strip2);
      break;

    case 8:
      theaterChase(strip6, ORANGE, 50);
      off(strip6);
      break;

    case 9:
      // to check wiring
      Serial. println("Type something to stop");
      while (Serial.available() > 0) {
        Serial.read();   // eol
      }
      while (Serial.available() <= 0) {
        theaterChase(strip1, RED, 20);
        theaterChase(strip1, GREEN, 20);
        theaterChase(strip1, strip1.Color(0, 0, 127, 0), 20);
      }
      break;

  }
}
//Fill the dots one after the other with a color
void off(Adafruit_NeoPixel & strip) {
  uint32_t c = strip1.Color(0, 0, 0, 0);
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel & strip, uint32_t color, uint8_t wait) {
  for (int j = 0; j < 7; j++) { //do 7 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip1.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, color);
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip1.numPixels(); i = i + 3)
        for (int i = 0; i < strip2.numPixels(); i = i + 3) {
          strip.setPixelColor(i + q, 0);
        }
    }
  }
}

