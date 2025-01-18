#include <Bounce2.h>
#include <Audio.h>

// Define notes
#define NOTE_C4 (261.63)
#define NOTE_CS4 (277.18)
#define NOTE_D4 (293.66)
#define NOTE_DS4 (311.13)
#define NOTE_E4 (329.63)
#define NOTE_F4 (349.23)
#define NOTE_FS4 (369.99)
#define NOTE_G4 (392.00)
#define NOTE_GS4 (415.30)
#define NOTE_A4 (440.00)
#define NOTE_AS4 (466.16)
#define NOTE_B4 (493.88)
#define NOTE_C5 (523.25)
#define NOTE_CS5 (554.37)
#define NOTE_D5 (587.33)
#define NOTE_DS5 (622.25)
#define NOTE_E5 (659.25)
#define NOTE_F5 (698.46)
#define NOTE_FS5 (739.99)
#define NOTE_G5 (783.99)
#define NOTE_GS5 (830.61)
#define NOTE_A5 (880.00)
#define NOTE_AS5 (932.33)
#define NOTE_B5 (987.77)
#define NOTE_C6 (1046.50)
#define NOTE_CS6 (1108.73)
#define NOTE_D6 (1174.66)
#define NOTE_DS6 (1244.51)
#define NOTE_E6 (1318.51)
#define NOTE_F6 (1396.91)
#define NOTE_FS6 (1479.98)
#define NOTE_G6 (1567.98)
#define NOTE_GS6 (1661.22)
#define NOTE_A6 (1760.00)
#define NOTE_AS6 (1864.66)
#define NOTE_B6 (1975.53)
#define NOTE_C7 (2093.00)
#define NOTE_CS7 (2217.46)
#define NOTE_D7 (2349.32)
#define NOTE_DS7 (2489.02)
#define NOTE_E7 (2637.02)
#define NOTE_F7 (2793.83)
#define NOTE_FS7 (2959.96)
#define NOTE_G7 (3135.96)
#define NOTE_GS7 (3322.44)
#define NOTE_A7 (3520.00)
#define NOTE_AS7 (3729.31)
#define NOTE_B7 (3951.07)
#define NOTE_C8 (4186.01)
#define NOTE_CS8 (4434.92)
#define NOTE_D8 (4698.63)
#define NOTE_DS8 (4978.03)
#define NOTE_E8 (5274.04)
#define NOTE_F8 (5587.65)
#define NOTE_FS8 (5919.91)
#define NOTE_G8 (6271.93)
#define NOTE_GS8 (6644.88)
#define NOTE_A8 (7040.00)
#define NOTE_AS8 (7458.62)
#define NOTE_B8 (7902.13)

// Audio library components
AudioSynthWaveform       waveform1;           // Waveform generator
AudioEffectEnvelope      envelope1;           // Envelope generator
AudioFilterStateVariable filter1;             // Filter to modify the waveform
AudioOutputI2S           i2s1;                // Audio output over I2S
AudioConnection          patchCord1(waveform1, envelope1);
AudioConnection          patchCord2(envelope1, 0, filter1, 0);
AudioConnection          patchCord3(envelope1, 0, i2s1, 0);
AudioConnection          patchCord4(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;          // Control chip for I2S

// Button setup
const int numButtons = 9;
Bounce buttons[numButtons] = {
  Bounce(0, 5), Bounce(1, 5), Bounce(2, 5), Bounce(3, 5),
  Bounce(4, 5), Bounce(5, 5), Bounce(6, 5), Bounce(7, 5),
  Bounce(8, 5)
};

float notes[numButtons - 1] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int currentPreset = 0;

void setup() {
  Serial.begin(9600);
  AudioMemory(15);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);
  
  // Initialize buttons
  for (int i = 0; i < numButtons; ++i) {
    pinMode(i, INPUT_PULLUP);
    buttons[i].attach(i);
    buttons[i].interval(5);
  }
  
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.9);

  envelope1.attack(50);
  envelope1.decay(120);
  envelope1.sustain(0.7);
  envelope1.release(250);
  
  filter1.resonance(0.5); // Slightly resonant

  Serial.println("Setup complete. Default waveform: Sine.");
}

void loop() {
  for (int i = 0; i < numButtons - 1; ++i) {
    buttons[i].update();
    if (buttons[i].fell()) {
      waveform1.frequency(notes[i]);
      envelope1.noteOn();
      
      Serial.print("Note played: ");
      Serial.println(notes[i]);
    }
    if (buttons[i].rose()) {
      envelope1.noteOff();
      Serial.print("Note stopped: ");
      Serial.println(notes[i]);
    }
  }
  
  // Check for preset change
  buttons[8].update();
  if (buttons[8].fell()) {
    currentPreset = (currentPreset + 1) % 5;
    Serial.print("Preset changed to: ");
    switch (currentPreset) {
      case 0: waveform1.begin(WAVEFORM_SINE); Serial.println("Sine"); break;
      case 1: waveform1.begin(WAVEFORM_SQUARE); Serial.println("Square"); break;
      case 2: waveform1.begin(WAVEFORM_SAWTOOTH); Serial.println("Sawtooth"); break;
      case 3: waveform1.begin(WAVEFORM_TRIANGLE); Serial.println("Triangle"); break;
      case 4: waveform1.begin(WAVEFORM_ARBITRARY); Serial.println("Arbitrary"); break;
    }
    // Make sure the filter frequency is set appropriately for the preset
    filter1.frequency(5000 - currentPreset * 1000);
  }
}
