//CODE WORKS! Tested at 2am by Nikhil on 5/3/14

//#include "Waveforms.h"

// declaring all I/O 
int pin_c = 52;
int pin_csharp = 53;
int pin_d = 51;
int pin_dsharp = 48;
int pin_e = 46;
int pin_f = 44;
int pin_fsharp = 42;
int pin_g = 30;
int pin_gsharp = 28;
int pin_a = 26;
int pin_asharp = 24;
int pin_b = 22;

int test = 6;

int buttons[12]={pin_c, pin_csharp, pin_d, pin_dsharp, pin_e, pin_f, pin_fsharp, pin_g, pin_gsharp, pin_a, pin_asharp, pin_b};

#define maxWaveform 4
#define maxSamplesNum 120

// Sin wave
static int waveformsTable[maxSamplesNum] = 
  {
    0x7ff, 0x86a, 0x8d5, 0x93f, 0x9a9, 0xa11, 0xa78, 0xadd, 0xb40, 0xba1,
    0xbff, 0xc5a, 0xcb2, 0xd08, 0xd59, 0xda7, 0xdf1, 0xe36, 0xe77, 0xeb4,
    0xeec, 0xf1f, 0xf4d, 0xf77, 0xf9a, 0xfb9, 0xfd2, 0xfe5, 0xff3, 0xffc,
    0xfff, 0xffc, 0xff3, 0xfe5, 0xfd2, 0xfb9, 0xf9a, 0xf77, 0xf4d, 0xf1f,
    0xeec, 0xeb4, 0xe77, 0xe36, 0xdf1, 0xda7, 0xd59, 0xd08, 0xcb2, 0xc5a,
    0xbff, 0xba1, 0xb40, 0xadd, 0xa78, 0xa11, 0x9a9, 0x93f, 0x8d5, 0x86a,
    0x7ff, 0x794, 0x729, 0x6bf, 0x655, 0x5ed, 0x586, 0x521, 0x4be, 0x45d,
    0x3ff, 0x3a4, 0x34c, 0x2f6, 0x2a5, 0x257, 0x20d, 0x1c8, 0x187, 0x14a,
    0x112, 0xdf, 0xb1, 0x87, 0x64, 0x45, 0x2c, 0x19, 0xb, 0x2,
    0x0, 0x2, 0xb, 0x19, 0x2c, 0x45, 0x64, 0x87, 0xb1, 0xdf,
    0x112, 0x14a, 0x187, 0x1c8, 0x20d, 0x257, 0x2a5, 0x2f6, 0x34c, 0x3a4,
    0x3ff, 0x45d, 0x4be, 0x521, 0x586, 0x5ed, 0x655, 0x6bf, 0x729, 0x794
  };

// will need to set pin for octave control

float sampletime_one;
//float sampletime_two;

// setting the frequency of each note, if we decide to change octaves, these can easily be changed by multiplying
// the frequency by 2^(octave) 

float c = 523.25*pow(2,7);
float csharp = 554.37*pow(2,7);
float d = 587.33*pow(2,7);
float dsharp = 622.25*pow(2,7);
float e = 659.25*pow(2,7);
float f = 698.46*pow(2,7);
float fsharp = 739.99*pow(2,7);
float g = 783.99*pow(2,7);
float gsharp = 830.61*pow(2,7);
float a = 880.00*pow(2,7);
float asharp = 932.33*pow(2,7);
float b = 987.77*pow(2,7);

float frequencies[12] = { c, csharp, d, dsharp, e, f, fsharp, g, gsharp, a, asharp, b};

//int maxSamplesNum = 120;

void setup () {
 
  analogWriteResolution(12);  // set the analog output resolution to 12 bit 4096 levels
  analogReadResolution(12);   // set the analog input resolution to 12 bit 
  
  // to eliminate the need for external pullup resistors for each key, buttons are declared this way
  pinMode(pin_c,INPUT_PULLUP); 
  pinMode(pin_csharp,INPUT_PULLUP);
  pinMode(pin_d,INPUT_PULLUP);
  pinMode(pin_dsharp,INPUT_PULLUP);
  pinMode(pin_e,INPUT_PULLUP);
  pinMode(pin_f,INPUT_PULLUP);
  pinMode(pin_fsharp,INPUT_PULLUP);
  pinMode(pin_g,INPUT_PULLUP);
  pinMode(pin_gsharp,INPUT_PULLUP);
  pinMode(pin_a,INPUT_PULLUP);
  pinMode(pin_asharp,INPUT_PULLUP);
  pinMode(pin_b,INPUT_PULLUP);
  
  // analog pin for octave controller
  
  Serial.begin(9600);
 
}

void loop () {
  while(1) {
    checkButton_one();
    //checkButton_two();
    waveform_one();
    }
  }

// we could implement 2 note polyphony, by having one function that outputs the waveform for to the one DAC pin, and a second function that writes to the // other pin
  
    
// function checks which button is being pressed and sets the appropriate frequency
void checkButton_one() {
  int x = 6;  
  
    for (int i = 0; i < 12; i++) {
        
        if (digitalRead(buttons[i]) == LOW) {
            sampletime_one = 1000000.00 / frequencies[i];
            
            //Serial.println(frequencies[i]);
        }
    }
}

// set global variables for incrementation within the checkbutton for loops, then pass note value from checkbutton functions to waveform functions
// checkbutton won't send anything if it detects the same button as the first function
/*
void checkButton_two() {
      for (int i = 0; i < 12; i++) {
        if (buttons[i] == LOW) {
            sampletime_two = 1000000.00 / frequencies[i];
        }
    }
}
*/

// if any buttons are low (activated), write output wave form as long as button is held
void waveform_one () {
    for (int i = 0; i < 12; i++) {
        if (digitalRead(buttons[i]) == LOW) {
            int j = 0;
            while (digitalRead(buttons[i]) == LOW) {
                int output_table = waveformsTable[j];
                analogWrite (DAC1, output_table); // write the selected waveform on DAC0
                //Serial.println(output_table);
                j++;
                    if(j == maxSamplesNum) { // Reset the counter to repeat the wave
                        j = 0;
                    } 
                
                delayMicroseconds(sampletime_one);
            }
        }
    }
}
    
/*
void waveform_two () {
  for (int i = 0; i < 13; i++) {
    if (digitalRead(buttons[i]) == LOW) {
            int j = 0;
            while (digitalRead(buttons[i]) == LOW) {
                analogWrite (DAC1, waveformsTable[j]); // write the selected waveform on DAC1
                j++;
                    if(j == maxSamplesNum) { // Reset the counter to repeat the wave
                        j = 0;
                    } 
                delayMicroseconds(sampletime_two);
                }
            }
        }
    }
    */


