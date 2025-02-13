#include <Audio.h>
#include <Bounce2.h>
#include <math.h>

//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------
void playNote(int noteIndex);
void stopNote(int noteIndex);

//------------------------------------------------------------------------------
// Helper Function: Compute Detuned Frequency
//------------------------------------------------------------------------------
/**
 * @brief Computes a detuned frequency from a base frequency.
 *
 * Uses preset-specific random detune and vibrato values.
 *
 * @param baseFreq       The base frequency in Hz.
 * @param fixedDetune    The fixed per-oscillator offset (in cents).
 * @param presetDetune   The preset-specified offset (in cents).
 * @param randomDepth    Maximum random detune depth (in cents).
 * @param vibDepth       Vibrato depth (in cents).
 * @param vibFrequency   Vibrato frequency (Hz).
 * @return The computed frequency in Hz.
 */
double computeDetunedFrequency(double baseFreq, int fixedDetune, int presetDetune,
                               double randomDepth, double vibDepth, double vibFrequency) {
  // Generate a fractional random detune between -randomDepth and +randomDepth cents.
  double randomDetune = random(-1000 * randomDepth, 1000 * randomDepth + 1) / 1000.0;
  
  // Vibrato component using current time.
  double vibrato = vibDepth * sin((millis() / 1000.0) * 2 * PI * vibFrequency);
  
  double totalDetune = fixedDetune + presetDetune + randomDetune + vibrato;
  return baseFreq * pow(2.0, totalDetune / 1200.0);
}

//------------------------------------------------------------------------------
// Global Constants and Structures
//------------------------------------------------------------------------------
const int numVoices = 3;
const int numPresets = 12;

// Base fixed detune values (in cents) for each oscillator.
const int baseDetune[3] = { -1, 1, 2 };

/**
 * @brief Holds configuration settings for a preset, including waveform,
 * envelope, detuning, and vibrato parameters.
 */
struct Preset {
  const char* name;
  // Oscillator settings:
  uint8_t waveType[3];    // Waveform type for each oscillator.
  float amplitude[3];     // Amplitude for each oscillator.
  
  // Envelope parameters (in milliseconds):
  unsigned int delay;
  unsigned int attack;
  unsigned int hold;
  unsigned int decay;
  float sustain;
  unsigned int release;
  
  // Preset-specific detune offsets (in cents) for each oscillator.
  int detune[3];
  
  // New parameters for detuning:
  double randomDetuneDepth; // Maximum random detune (cents).
  double vibratoDepth;      // Vibrato depth (cents).
  double vibratoFrequency;  // Vibrato frequency (Hz).
  
  // Optional pulse width settings for each oscillator (-1 if unused)
  float pulseWidth[3];
};

//------------------------------------------------------------------------------
// Preset Definitions (Each with Unique Detune/Vibrato Settings)
//------------------------------------------------------------------------------
Preset presets[numPresets] = {
  // Preset 0: Cosmic Bell
  { "Cosmic Bell",
    { WAVEFORM_SINE, WAVEFORM_SINE, WAVEFORM_SINE },
    { 0.8, 0.3, 0.2 },
    0, 50, 0, 500, 0.2, 1000,
    { 0, 0, 0 },
    1.0,   // randomDetuneDepth: ±1.0 cent
    0.5,   // vibratoDepth: 0.5 cents
    0.5,   // vibratoFrequency: 0.5 Hz
    { -1, -1, -1 }
  },
  // Preset 1: Vibrant Pad
  { "Vibrant Pad",
    { WAVEFORM_SAWTOOTH, WAVEFORM_TRIANGLE, WAVEFORM_SINE },
    { 0.8, 0.6, 0.4 },
    0, 800, 0, 1500, 0.7, 2500,
    { -5, 5, 0 },
    2.0,   // ±2.0 cents random detune
    0.7,   // 0.7 cents vibrato depth
    0.3,   // 0.3 Hz vibrato frequency
    { -1, -1, -1 }
  },
  // Preset 2: Aggressive Bass
  { "Aggressive Bass",
    { WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH, WAVEFORM_PULSE },
    { 1.0, 0.8, 0.6 },
    0, 5, 0, 200, 0.0, 100,
    { 0, 0, 0 },
    0.5,   // Very subtle random detune
    0.2,   // Minimal vibrato
    0.1,   // Slow vibrato frequency
    { -1, -1, 0.5 }
  },
  // Preset 3: Digital Drone
  { "Digital Drone",
    { WAVEFORM_SAWTOOTH_REVERSE, WAVEFORM_TRIANGLE, WAVEFORM_SINE },
    { 0.8, 0.7, 0.5 },
    0, 1000, 0, 2000, 0.9, 3000,
    { -3, 3, 0 },
    1.5,   // Moderate random detune
    1.0,   // Noticeable vibrato
    0.4,   // 0.4 Hz vibrato frequency
    { -1, -1, -1 }
  },
  // Preset 4: Glitchy FX
  { "Glitchy FX",
    { WAVEFORM_SAMPLE_HOLD, WAVEFORM_SAMPLE_HOLD, WAVEFORM_SQUARE },
    { 0.7, 0.7, 0.6 },
    0, 10, 20, 30, 0.0, 20,
    { 0, 0, 0 },
    3.0,   // Wide random detune for chaotic effect
    0.0,   // No vibrato
    0.0,   // --
    { -1, -1, -1 }
  },
  // Preset 5: Analog Lead
  { "Analog Lead",
    { WAVEFORM_SAWTOOTH, WAVEFORM_PULSE, WAVEFORM_TRIANGLE },
    { 0.9, 0.8, 0.7 },
    0, 50, 0, 400, 0.6, 500,
    { 2, -2, 0 },
    1.5,   // Moderate random detune
    0.8,   // Noticeable vibrato
    0.8,   // 0.8 Hz vibrato frequency
    { -1, 0.4, -1 }
  },
  // Preset 6: Ethereal Voices
  { "Ethereal Voices",
    { WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH },
    { 0.7, 0.5, 0.3 },
    100, 1200, 500, 800, 0.6, 1500,
    { 0, 0, 0 },
    1.0,   // Subtle random detune
    0.6,   // Smooth vibrato
    0.5,   // 0.5 Hz vibrato frequency
    { -1, -1, -1 }
  },
  // Preset 7: Crystal Organ
  { "Crystal Organ",
    { WAVEFORM_SINE, WAVEFORM_SINE, WAVEFORM_TRIANGLE },
    { 0.8, 0.8, 0.6 },
    0, 200, 0, 300, 0.8, 600,
    { 0, 0, 0 },
    0.5,   // Minimal random detune
    0.2,   // Very subtle vibrato
    0.2,   // 0.2 Hz vibrato frequency
    { -1, -1, -1 }
  },
  // Preset 8: Electric Pluck
  { "Electric Pluck",
    { WAVEFORM_TRIANGLE, WAVEFORM_SQUARE, WAVEFORM_SINE },
    { 1.0, 0.5, 0.3 },
    0, 5, 10, 50, 0.0, 80,
    { 0, 0, 0 },
    0.2,   // Very subtle random detune
    0.1,   // Minimal vibrato
    1.0,   // Faster vibrato for pluckiness
    { -1, -1, -1 }
  },
  // Preset 9: Warm Rhodes
  { "Warm Rhodes",
    { WAVEFORM_SAWTOOTH, WAVEFORM_SAWTOOTH, WAVEFORM_TRIANGLE },
    { 0.8, 0.6, 0.5 },
    0, 100, 0, 500, 0.7, 800,
    { -3, 3, 0 },
    0.8,   // Moderate random detune
    0.5,   // Smooth vibrato
    0.4,   // 0.4 Hz vibrato frequency
    { -1, -1, -1 }
  },
  // Preset 10: Spacey Arp
  { "Spacey Arp",
    { WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH, WAVEFORM_SINE },
    { 0.7, 0.7, 0.5 },
    0, 20, 0, 300, 0.5, 400,
    { 5, -5, 0 },
    1.2,   // Slightly wider random detune
    0.7,   // Moderate vibrato
    0.6,   // 0.6 Hz vibrato frequency
    { -1, -1, -1 }
  },
  // Preset 11: Experimental Noise
  { "Experimental Noise",
    { WAVEFORM_SAMPLE_HOLD, WAVEFORM_SINE, WAVEFORM_TRIANGLE },
    { 0.8, 0.5, 0.6 },
    0, 50, 0, 400, 0.3, 800,
    { 0, 0, 0 },
    2.0,   // Wide random detune for chaos
    1.0,   // Noticeable vibrato
    1.5,   // Faster vibrato frequency
    { -1, -1, -1 }
  }
};

// Global preset state
int currentPresetIndex = 0;
int currentPresetDetune[3] = { 0, 0, 0 };

//------------------------------------------------------------------------------
// Audio Objects and Patch Connections
//------------------------------------------------------------------------------
AudioSynthWaveform waveform1[numVoices];  // Oscillator 1
AudioSynthWaveform waveform2[numVoices];  // Oscillator 2
AudioSynthWaveform waveform3[numVoices];  // Oscillator 3
AudioMixer4 mixer1[numVoices];            // Mixer for each voice's oscillators
AudioEffectEnvelope envelope1[numVoices]; // Envelope for each voice

AudioMixer4 outputMixer;
AudioOutputI2S i2s1;                 // I2S output
AudioControlSGTL5000 sgtl5000_1;       // Audio control chip

// Patch cords for each voice:
AudioConnection patchCord1(waveform1[0], 0, mixer1[0], 0);
AudioConnection patchCord2(waveform2[0], 0, mixer1[0], 1);
AudioConnection patchCord3(waveform3[0], 0, mixer1[0], 2);
AudioConnection patchCord4(mixer1[0], envelope1[0]);
AudioConnection patchCord5(envelope1[0], 0, outputMixer, 0);

AudioConnection patchCord7(waveform1[1], 0, mixer1[1], 0);
AudioConnection patchCord8(waveform2[1], 0, mixer1[1], 1);
AudioConnection patchCord9(waveform3[1], 0, mixer1[1], 2);
AudioConnection patchCord10(mixer1[1], envelope1[1]);
AudioConnection patchCord11(envelope1[1], 0, outputMixer, 1);

AudioConnection patchCord13(waveform1[2], 0, mixer1[2], 0);
AudioConnection patchCord14(waveform2[2], 0, mixer1[2], 1);
AudioConnection patchCord15(waveform3[2], 0, mixer1[2], 2);
AudioConnection patchCord16(mixer1[2], envelope1[2]);
AudioConnection patchCord17(envelope1[2], 0, outputMixer, 2);

AudioConnection patchCord192(outputMixer, 0, i2s1, 0);
AudioConnection patchCord20(outputMixer, 0, i2s1, 1);

//------------------------------------------------------------------------------
// Button Setup
//------------------------------------------------------------------------------
// 10 buttons:
//  - Buttons 0-7 (pins 0,1,2,3,4,5,8,21): Note play.
//  - Button 8 (pin 20): Preset selection.
//  - Button 9 (pin 17): Octave cycling.
// When buttons 8 and 9 are held, note buttons 0 and 1 adjust volume.
const int numButtons = 10;
const int buttonPins[numButtons] = { 0, 1, 2, 3, 4, 5, 8, 21, 20, 17 };
Bounce buttons[numButtons];

//------------------------------------------------------------------------------
// Note Frequencies and Voice Management
//------------------------------------------------------------------------------
// Eight note frequencies for octave 5 (C5, D5, E5, F5, G5, A5, B5, C6).
float noteFrequencies[8] = { 523.25, 587.33, 659.25, 698.46, 783.99, 880.00, 987.77, 1046.50 };

/*
  voiceNoteIndices[] tracks which note (by index) is playing on each voice.
  A value of -1 indicates an inactive voice.
*/
int voiceNoteIndices[numVoices];
int currentVoice = 0; // Round-robin voice allocation

// Current octave (default 5). Frequencies scale by 2^(currentOctave - 5).
int currentOctave = 5;

//------------------------------------------------------------------------------
// Global Volume Control Variables
//------------------------------------------------------------------------------
float currentVolume = 0.65;  // Default volume (0.0 to 1.0)
bool volumeMode = false;     // Flag for volume mode

//------------------------------------------------------------------------------
// Preset and Voice Functions
//------------------------------------------------------------------------------
/**
 * @brief Loads a preset and applies its settings to all voices.
 */
void changeToPreset(int presetIndex) {
  if (presetIndex < 0 || presetIndex >= numPresets)
    return;
  
  Preset preset = presets[presetIndex];
  Serial.print("Changing preset to: ");
  Serial.println(preset.name);
  
  // Update global preset detune offsets.
  for (int i = 0; i < 3; i++) {
    currentPresetDetune[i] = preset.detune[i];
  }
  
  // Apply oscillator and envelope settings.
  for (int i = 0; i < numVoices; i++) {
    waveform1[i].begin(preset.waveType[0]);
    waveform1[i].amplitude(preset.amplitude[0]);
    
    waveform2[i].begin(preset.waveType[1]);
    waveform2[i].amplitude(preset.amplitude[1]);
    
    waveform3[i].begin(preset.waveType[2]);
    waveform3[i].amplitude(preset.amplitude[2]);
    
    if (preset.pulseWidth[0] >= 0)
      waveform1[i].pulseWidth(preset.pulseWidth[0]);
    if (preset.pulseWidth[1] >= 0)
      waveform2[i].pulseWidth(preset.pulseWidth[1]);
    if (preset.pulseWidth[2] >= 0)
      waveform3[i].pulseWidth(preset.pulseWidth[2]);
    
    envelope1[i].delay(preset.delay);
    envelope1[i].attack(preset.attack);
    envelope1[i].hold(preset.hold);
    envelope1[i].decay(preset.decay);
    envelope1[i].sustain(preset.sustain);
    envelope1[i].release(preset.release);
  }
}

/**
 * @brief Plays a note using round-robin voice allocation, applying octave and detuning.
 */
void playNote(int noteIndex) {
  stopNote(noteIndex);
  currentVoice = (currentVoice + 1) % numVoices;
  voiceNoteIndices[currentVoice] = noteIndex;
  
  double baseFreq = noteFrequencies[noteIndex] * pow(2, currentOctave - 5);
  
  // Get current preset detune parameters.
  double randomDepth = presets[currentPresetIndex].randomDetuneDepth;
  double vibDepth     = presets[currentPresetIndex].vibratoDepth;
  double vibFrequency = presets[currentPresetIndex].vibratoFrequency;
  
  waveform1[currentVoice].frequency(
    computeDetunedFrequency(baseFreq, baseDetune[0], currentPresetDetune[0],
                            randomDepth, vibDepth, vibFrequency)
  );
  waveform2[currentVoice].frequency(
    computeDetunedFrequency(baseFreq, baseDetune[1], currentPresetDetune[1],
                            randomDepth, vibDepth, vibFrequency)
  );
  waveform3[currentVoice].frequency(
    computeDetunedFrequency(baseFreq, baseDetune[2], currentPresetDetune[2],
                            randomDepth, vibDepth, vibFrequency)
  );
  
  envelope1[currentVoice].noteOn();
  
  Serial.print("Playing note ");
  Serial.print(baseFreq);
  Serial.print(" Hz on voice ");
  Serial.println(currentVoice);
}

/**
 * @brief Stops a note if it is currently playing.
 */
void stopNote(int noteIndex) {
  for (int i = 0; i < numVoices; i++) {
    if (voiceNoteIndices[i] == noteIndex) {
      envelope1[i].noteOff();
      voiceNoteIndices[i] = -1;
      Serial.print("Stopped note ");
      Serial.println(noteFrequencies[noteIndex]);
    }
  }
}

//------------------------------------------------------------------------------
// Setup and Main Loop
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  AudioMemory(15);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(currentVolume);
  
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttons[i].attach(buttonPins[i]);
    buttons[i].interval(5);
  }
  
  for (int i = 0; i < numVoices; i++) {
    voiceNoteIndices[i] = -1;
  }
  
  changeToPreset(currentPresetIndex);
  Serial.println("Setup complete.");
}

void loop() {
  for (int i = 0; i < numButtons; i++) {
    buttons[i].update();
  }
  
  // Check if preset (button 8) and octave (button 9) buttons are held.
  bool presetHeld = (buttons[8].read() == LOW);
  bool octaveHeld  = (buttons[9].read() == LOW);
  
  if (presetHeld && octaveHeld) {
    if (!volumeMode) {
      volumeMode = true;
      Serial.println("Volume mode activated.");
    }
    if (buttons[0].fell()) {
      currentVolume -= 0.05;
      if (currentVolume < 0.0) currentVolume = 0.0;
      sgtl5000_1.volume(currentVolume);
      Serial.print("Volume decreased to ");
      Serial.println(currentVolume);
    }
    if (buttons[1].fell()) {
      currentVolume += 0.05;
      if (currentVolume > 1.0) currentVolume = 1.0;
      sgtl5000_1.volume(currentVolume);
      Serial.print("Volume increased to ");
      Serial.println(currentVolume);
    }
  } else {
    if (volumeMode) {
      volumeMode = false;
      Serial.println("Exiting volume mode.");
    }
    for (int i = 0; i < 8; i++) {
      if (buttons[i].fell()) {
        Serial.print("Button on pin ");
        Serial.print(buttonPins[i]);
        Serial.println(" activated (note button).");
        playNote(i);
      }
      if (buttons[i].rose()) {
        Serial.print("Button on pin ");
        Serial.print(buttonPins[i]);
        Serial.println(" released (note button).");
        stopNote(i);
      }
    }
    if (buttons[8].fell()) {
      Serial.print("Button on pin ");
      Serial.print(buttonPins[8]);
      Serial.println(" activated (instrument selection).");
      currentPresetIndex = (currentPresetIndex + 1) % numPresets;
      changeToPreset(currentPresetIndex);
    }
    if (buttons[9].fell()) {
      Serial.print("Button on pin ");
      Serial.print(buttonPins[9]);
      Serial.println(" activated (octave cycle).");
      currentOctave++;
      if (currentOctave > 6) currentOctave = 3;
      Serial.print("Octave set to ");
      Serial.println(currentOctave);
    }
  }
}