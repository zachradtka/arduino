

#define MIC_PIN A0  // 
#define LED_PIN 13 //
#define NUM_SAMPLES 60  // Number of samples
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define NOISE 0 // Noise in mic signal
#define ACTIVE_VOL 10

const int NUM_BLINKS = 4;
const int OFF_TIME = 200;
const int ON_TIME = 200;

const int OUTPUT_PIN = 13;

const int SENSOR_PIN = 14;
int volume = 0;

int vol[NUM_SAMPLES];
byte volCnt = 0;  // Frame counter

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(MIC_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
int volume =512;
  volume = analogRead(MIC_PIN);

  volume = abs(volume - 512 - DC_OFFSET);
  volume = (volume <= NOISE) ? 0 : (volume - NOISE);      // Remove noise/hum

//  Serial.println(volume);

  vol[volCnt++] = volume;
  if (volCnt >= NUM_SAMPLES) {
    volCnt = 0;
  }

  // Calculate the average
  int sum = 0;
  for (int i = 0; i< NUM_SAMPLES; i++) {
    sum+=vol[i];
  }
  int avg_vol = sum / NUM_SAMPLES;
  Serial.println(avg_vol);

  
  if (avg_vol > ACTIVE_VOL) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
//  delay(10);
  //  for (int i=0; i < NUM_BLINKS; i++) {
  //    digitalWrite(13, HIGH);
  //    delay(ON_TIME);
  //    digitalWrite(13, LOW);
  //    delay(OFF_TIME);
  //  }
  //
  //  delay(OFF_TIME * 4);
}
