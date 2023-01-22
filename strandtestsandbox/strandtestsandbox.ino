#include <Adafruit_TCS34725.h>

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
//int sensorPin = A0;
//int sensorValue = 0;

#define trigPin 13
#define echoPin 12

int RED;
int GREEN;
int BLUE;
long duration;
long distance;
int mappedDistance;
byte gammatable[256];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

   if (tcs.begin()) {
      Serial.println("Found sensor");
    } else {
      Serial.println("No TCS34725 found ... check your connections");
      while (1); // halt!
    }
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

   // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
}

void loop() {

//  Serial.println(sensorValue);
//  instructableFunction();
  colorRead();
  setLed();
  talkBack();

}

void colorRead() {
  uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED
  if(red >= 4000) red = 4000;
  red = map(red, 0, 4000, 0, 255);
  if(green >= 4000) green = 4000;
  green = map(green, 0, 4000, 0, 255);
  if(blue >= 4000) blue = 4000;
  blue = map(blue, 0, 4000, 0, 255);
  
  
  RED = red;
  GREEN = green;
  BLUE = blue;
}

void instructableFunction() {
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.println(distance);
 
  if (distance >= 400) distance = 400;

  mappedDistance = map(distance, 0, 400, 0, 255);
  
  delay(500);
}

void talkBack(){
  Serial.print("R: ");
  Serial.println(RED);
  Serial.print("G: ");
  Serial.println(GREEN);
  Serial.print("B: ");
  Serial.println(BLUE);
  Serial.println();
}

void setLed(){
//  colorWipe(strip.Color(mappedDistance, (255 - mappedDistance), 0), 50); // Red
  colorWipe(strip.Color(RED, GREEN, BLUE), 50); // otro
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

