#include <Bounce2.h>
#include <Audio.h>

// Define notes
#define NOTE_C4  261.63
#define NOTE_D4  293.66
#define NOTE_E4  329.63
#define NOTE_F4  349.23
#define NOTE_G4  392.00
#define NOTE_A4  440.00
#define NOTE_B4  493.88
#define NOTE_C5  523.25

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

float notes[numButtons - 1] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};
int currentPreset = 0;

void setup() {
  Serial.begin(9600);
  AudioMemory(15);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  
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
      Serial.println("Note stopped.");
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
