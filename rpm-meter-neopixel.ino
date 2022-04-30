#include <Adafruit_NeoPixel.h>
#define PIN            6  //LED Data Pin
#define NUMPIXELS      8  //number of leds connected
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//Calculate the frequency of the tacho signal you are using and adjust the following values accordingly
//example: for a standard tacho signal on a 4 cylinder engine the frequeny is equal to the RPM divided by 30
const unsigned int onFrequency = 5;  //startup sequence, value represents engine speed higher than cranking and lower than idle
const unsigned int minFrequency = 10; // minimum frequency to begin turning on LEDs
const unsigned int maxFrequency = 15; // maximum frequency in normal range, after this value shift flash will occur
const unsigned int shiftFrequency = 20; // frequency range from max to shift when shifting should happen, after this value overrev flash will occur


//Set the color for each LED in the neo pixel strip
//Colors are set using an RGB value ranging from (0,0,0) to (255,255,255)
const uint32_t tachColor[NUMPIXELS] = {
  Adafruit_NeoPixel::Color(0, 120, 0),//green
  Adafruit_NeoPixel::Color(0, 120, 0),//green
  Adafruit_NeoPixel::Color(0, 120, 0),//green
  Adafruit_NeoPixel::Color(60, 60, 0),//orange
  Adafruit_NeoPixel::Color(60, 60, 0),//orange
  Adafruit_NeoPixel::Color(120, 0, 0),//red
  Adafruit_NeoPixel::Color(120, 0, 0),//red
  Adafruit_NeoPixel::Color(0, 0, 120),//blue
};


//Set the frequency when each LED should turn on
//First LED turns on at minFrequency
const unsigned int lightShiftFreq[NUMPIXELS] = {
  minFrequency,
  11,
  11,
  13,
  16,
  16,
  18,
  18,
};

bool hasStartupSequenceRun = false; // only run startup sequence one time
const byte tachPin = 5;
unsigned long igfreq;

void setup() {
    pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
    
    float ighigh, iglow;
    unsigned long igcal1, igcal2;

    //measure period of tach signal
    /*ighigh = pulseIn(tachPin, HIGH);
    iglow = pulseIn(tachPin, LOW);

    igcal1 = 1000 / ((ighigh / 1000) + (iglow / 1000));

    //do it again
    ighigh = pulseIn(tachPin, HIGH);
    iglow = pulseIn(tachPin, LOW);

    igcal2 = 1000 / ((ighigh / 1000) + (iglow / 1000));*/
    
    if (hasStartupSequenceRun == false) {
        for (int rpm = 0; rpm < 100; rpm++)
        {
            igfreq += rpm;
            if (igfreq >= 20)
            {
                igfreq = 0;
            }
            else
            {
                if (igfreq > onFrequency) {
                    //run start sequence
                    //LEDs will light up, flash and light out upon starting the engine.
                    for (int i = 0; i < NUMPIXELS; ++i) {
                        pixels.setPixelColor(i, tachColor[i]);
                        pixels.show();
                        delay(50);
                    }
                    for (int a = 0; a < 10; a++) {
                        pixels.fill(pixels.Color(0, 0, 120));
                        pixels.show();
                        delay(20);
                        pixels.fill(pixels.Color(0, 0, 0));
                        pixels.show();
                        delay(20);
                    }
                    for (int i = 0; i < NUMPIXELS; ++i) {
                        pixels.setPixelColor(i, tachColor[i]);
                        pixels.show();
                    }
                    for (int i = NUMPIXELS - 1; i >= 0; --i) {
                        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
                        pixels.show();
                        delay(50);
                    }
                    hasStartupSequenceRun = true;
                    pixels.fill(pixels.Color(0, 0, 0));
                    pixels.show();
                }
                /*if (igfreq < onFrequency) {
                    //resets hasStartupSequenceRun to false if engine stops but Arduino remains powered, 
                    //startupsequence will rerun upon restarting the engine
                    hasStartupSequenceRun = false;
                }*/
            }
        }
    }
        
    /*
    if (igfreq < maxFrequency) {

        // normal operating range
        for (int i = 0; i < NUMPIXELS; ++i) {
            if (igfreq > lightShiftFreq[i]) {
                pixels.setPixelColor(i, tachColor[i]);
            }
            else {
                pixels.setPixelColor(i, pixels.Color(0, 0, 0));
            }
        }
        pixels.show();
    }
    else if (igfreq >= maxFrequency && igfreq < shiftFrequency) {
        //shift flash
        //default color=blue
        //to change shift flash color, edit RGB value in following line
        pixels.fill(pixels.Color(0, 0, 120));
        pixels.show();
        delay(20);
        pixels.fill(pixels.Color(0, 0, 0));
        pixels.show();
        delay(20);
    }

    else if (igfreq >= shiftFrequency) {
        //overrev flash
        //default color=red
        //to change overrev flash color, edit RGB value in following line
        pixels.fill(pixels.Color(120, 0, 0));
        pixels.show();
        delay(20);
        pixels.fill(pixels.Color(0, 0, 0));
        pixels.show();
        delay(20);
    }*/
}