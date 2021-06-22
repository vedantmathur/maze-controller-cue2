// Insight Maze - NeoPixel ring lights

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// The Arduino pin connected to the light
// #define LED_PIN     6
int LED_PIN = 0;

// How many NeoPixels are attached to the Arduino
#define LED_COUNT  60

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 35 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare the NeoPixel strip object:
// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

Adafruit_NeoPixel strip[3];
int incomingByte;
bool FFLAG = false;

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.


  
  Serial.begin(9600);

  for (int i = 0; i < 3; i++){
    strip[i] = Adafruit_NeoPixel(LED_COUNT, i + 5, NEO_GRBW + NEO_KHZ800);
    strip[i].begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip[i].show(); 
    // Turn OFF all pixels ASAP
    strip[i].setBrightness(BRIGHTNESS); 
  }

  
}

void loop() {

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

    // say what you got:
    switch(incomingByte){
    case 'q':
      LED_PIN = 0;
      break;
    case 'w':
      LED_PIN = 1;
      break;
    case 'e':
      LED_PIN = 2;
      break;
    case 'f':
      FFLAG = !FFLAG;
    default:
      break;
   }
  }
  
  // Flash green (can change to other colors)
  Serial.print("LED NO: ");
  Serial.println(LED_PIN);
  if(FFLAG){
    flash_led_no(LED_PIN);
  }
  else {
    fade_led_no(LED_PIN);
  }
  
}

void flash_led_no(int i){
  strip[i].fill(strip[i].Color(0, 255, 0));
  strip[i].show();
  delay(700);
  strip[i].fill(strip[i].Color(0, 0, 0));
  strip[i].show();
  delay(300);
}

void fade_led_no(int i){
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip[i].fill(strip[i].Color(0, strip[i].gamma8(j), 0));
    strip[i].show();
    delay(5);
  }
  //delay(300);
  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip[i].fill(strip[i].Color(0, strip[i].gamma8(j), 0));
    strip[i].show();
    delay(5);
  }
  
}
