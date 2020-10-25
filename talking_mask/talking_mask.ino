#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define MATRIX_PIN 7  // Output pin to matrix
#define NUMPIXELS  64 // Number of pixel in matrix

Adafruit_NeoPixel pixels(NUMPIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

#define PIXEL_BRIGHTNESS 25 // A value from 0 to 255, where 0 is max
#define GRID_ROWS 8
#define GRID_COLS 8

#define MIC_PIN A0      // Input pin to read microphone
#define NUM_SAMPLES 5   // Number of samples
#define DC_OFFSET   0   // DC offset in mic signal - if unusure, leave 0
#define NOISE       0   // Noise in mic signal
#define ACTIVE_VOL  10

#define VOL_0 20
#define VOL_1 40
#define VOL_2 60
#define VOL_3 80

const uint32_t C_LIPS   = pixels.Color(0, 255, 0);
const uint32_t C_WHITE = pixels.Color(255, 255, 255);
const uint32_t C_BLACK = pixels.Color(0, 0, 0);

const uint32_t TALK_0[GRID_ROWS][GRID_COLS] = {
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS },
  {C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS },
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK}
};

const uint32_t TALK_1[GRID_ROWS][GRID_COLS] = {
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_BLACK, C_BLACK},
  {C_LIPS,  C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS,  C_LIPS },
  {C_LIPS,  C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS,  C_LIPS },
  {C_BLACK, C_BLACK, C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK}
};

const uint32_t TALK_2[GRID_ROWS][GRID_COLS] = {
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_BLACK, C_BLACK},
  {C_BLACK, C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK , C_LIPS,  C_BLACK},
  {C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS },
  {C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS },
  {C_BLACK, C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS,  C_BLACK},
  {C_BLACK, C_BLACK, C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_BLACK, C_BLACK},
  {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK}
};

const uint32_t TALK_3[GRID_ROWS][GRID_COLS] = {
  {C_BLACK, C_BLACK, C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_BLACK, C_BLACK},
  {C_BLACK, C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK , C_LIPS,  C_BLACK},
  {C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS },
  {C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS },
  {C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS },
  {C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_LIPS },
  {C_BLACK, C_LIPS,  C_BLACK, C_BLACK, C_BLACK, C_BLACK , C_LIPS,  C_BLACK},
  {C_BLACK, C_BLACK, C_LIPS,  C_LIPS,  C_LIPS,  C_LIPS,  C_BLACK, C_BLACK},
};


int vol[NUM_SAMPLES];
byte volCnt = 0;  // Frame counter
void setup() {
  pinMode(MIC_PIN, INPUT);
  pixels.begin();
  pixels.show();
  pixels.setBrightness(25);
}

void loop() {
  int volume = analogRead(MIC_PIN);

  volume = abs(volume - 512 - DC_OFFSET);
  volume = (volume <= NOISE) ? 0 : (volume - NOISE);      // Remove noise/hum


  // Get a window average
  vol[volCnt++] = volume;
  if (volCnt >= NUM_SAMPLES) {
    volCnt = 0;
  }

  // Calculate the average
  int sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += vol[i];
  }
  int avg_vol = sum / NUM_SAMPLES;
//  Serial.println(avg_vol);

  if (avg_vol > VOL_3) {
    displayMouth(TALK_3);
  } else if (avg_vol > VOL_2) {
    displayMouth(TALK_2);
  } else if (avg_vol > VOL_1) {
    displayMouth(TALK_1);
  } else {
    displayMouth(TALK_0);
  }

}


void displayMouth( uint32_t mouth[8][8]) {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      pixels.setPixelColor(i * GRID_COLS + j, mouth[i][j]);
    }
  }
  pixels.show();
}
