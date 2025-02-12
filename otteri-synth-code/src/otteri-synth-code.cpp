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
 * Applies a fixed detune (per oscillator), a preset-specific offset,
 * and a random detune (from -2 to +2 cents).
 *
 * @param baseFreq     The base frequency in Hz.
 * @param fixedDetune  The fixed per-oscillator offset (in cents).
 * @param presetDetune The preset-specified offset (in cents).
 * @return The computed frequency in Hz.
 */
double computeDetunedFrequency(double baseFreq, int fixedDetune, int presetDetune) {
  int randomDetune = random(-2, 3); // random value between -2 and 2
  return baseFreq * pow(2.0, (fixedDetune + presetDetune + randomDetune) / 1200.0);
}

//------------------------------------------------------------------------------
// Global Constants and Structures
//------------------------------------------------------------------------------
const int numVoices = 3;
const int numPresets = 12;

// Base fixed detune values (in cents) for each oscillator.
const int baseDetune[3] = { -1, 1, 2 };

/**
 * @brief Holds all configuration settings for a preset.
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
  
  // Optional pulse width settings for each oscillator (-1 if unused)
  float pulseWidth[3];
};

//------------------------------------------------------------------------------
// Preset Definitions
//------------------------------------------------------------------------------
Preset presets[numPresets] = {
  // Preset 0: Sine
  { "Sine",
    { WAVEFORM_SINE, WAVEFORM_SAWTOOTH, WAVEFORM_PULSE },
    { 0.9, 0.9, 0.9 },
    0, 50, 0, 120, 0.7, 250,
    { -10, 9, 0 },
    { -1, -1, -1 }
  },
  // Preset 1: Ambient Pad
  { "Ambient Pad",
    { WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH },
    { 0.8, 0.5, 0.3 },
    0, 800, 0, 1000, 0.6, 1500,
    { -4, 4, 0 },
    { -1, -1, -1 }
  },
  // Preset 2: Percussive Stab
  { "Percussive Stab",
    { WAVEFORM_SQUARE, WAVEFORM_PULSE, WAVEFORM_SAMPLE_HOLD },
    { 0.9, 0.9, 0.4 },
    0, 0, 0, 200, 0.0, 100,
    { 0, 0, 0 },
    { -1, -1, -1 }
  },
  // Preset 3: Deep Bass
  { "Deep Bass",
    { WAVEFORM_SINE, WAVEFORM_SAWTOOTH, WAVEFORM_SQUARE },
    { 1.0, 0.7, 0.5 },
    0, 5, 0, 400, 0.8, 200,
    { 0, 0, 0 },
    { -1, -1, -1 }
  },
  // Preset 4: Lead Synth
  { "Lead Synth",
    { WAVEFORM_SAWTOOTH, WAVEFORM_PULSE, WAVEFORM_TRIANGLE },
    { 0.8, 0.6, 0.5 },
    0, 50, 0, 300, 0.7, 400,
    { 0, 0, 0 },
    { -1, -1, -1 }
  },
  // Preset 5: Space Whispers
  { "Space Whispers",
    { WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_TRIANGLE_VARIABLE },
    { 0.3, 0.3, 0.3 },
    0, 500, 0, 2000, 0.5, 3000,
    { 0, 0, 0 },
    { -1, -1, 0.75 } // Only oscillator 3 uses a custom pulse width.
  },
  // Preset 6: Ethereal Voices
  { "Ethereal Voices",
    { WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH },
    { 0.8, 0.5, 0.3 },
    100, 1200, 500, 800, 0.6, 1500,
    { 0, 0, 0 },
    { -1, -1, -1 }
  },
  // Preset 7: Glitch Machine
  { "Glitch Machine",
    { WAVEFORM_SQUARE, WAVEFORM_TRIANGLE, WAVEFORM_SAMPLE_HOLD },
    { 0.8, 0.6, 0.5 },
    0, 10, 0, 50, 0.1, 10,
    { 0, 0, 0 },
    { -1, -1, -1 }
  },
  // Preset 8: Retro Arcade
  { "Retro Arcade",
    { WAVEFORM_BANDLIMIT_SQUARE, WAVEFORM_SQUARE, WAVEFORM_SINE },
    { 1.0, 0.7, 0.4 },
    0, 5, 0, 300, 0.8, 150,
    { 0, 0, 0 },
    { -1, -1, -1 }
  },
  // Preset 9: Cyber Drone
  { "Cyber Drone",
    { WAVEFORM_SAWTOOTH, WAVEFORM_BANDLIMIT_PULSE, WAVEFORM_SAWTOOTH_REVERSE },
    { 0.9, 0.9, 0.9 },
    0, 100, 0, 1200, 0.6, 2500,
    { 0, 0, 0 },
    { -1, -1, -1 }
  },
  // Preset 10: Cinematic Sweep
  { "Cinematic Sweep",
    { WAVEFORM_SAWTOOTH, WAVEFORM_TRIANGLE, WAVEFORM_PULSE },
    { 0.7, 0.3, 0.5 },
    50, 500, 100, 400, 0.4, 800,
    { 0, 0, 0 },
    { -1, -1, 0.25 }
  },
  // Preset 11: Staccato Plucks
  { "Staccato Plucks",
    { WAVEFORM_TRIANGLE, WAVEFORM_SQUARE, WAVEFORM_SINE },
    { 1.0, 0.5, 0.3 },
    0, 10, 5, 20, 0.0, 50,
    { 0, 0, 0 },
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
// We'll have 10 buttons:
//   - Buttons 0-7 (pins 0,1,2,3,4,5,8,21) play notes.
//   - Button 8 (pin 20) changes the instrument preset.
//   - Button 9 (pin 17) cycles the octave.
const int numButtons = 10;
const int buttonPins[numButtons] = { 0, 1, 2, 3, 4, 5, 8, 21, 20, 17 };
Bounce buttons[numButtons];

//------------------------------------------------------------------------------
// Note Frequencies and Voice Management
//------------------------------------------------------------------------------
// Define eight note frequencies corresponding to C5, D5, E5, F5, G5, A5, B5, C6.
// (These values match your #defines for NOTE_C5, NOTE_D5, etc.)
float noteFrequencies[8] = { 523.25, 587.33, 659.25, 698.46, 783.99, 880.00, 987.77, 1046.50 };

/*
  voiceNoteIndices[] tracks which note (by index into noteFrequencies) is
  currently playing on each voice. A value of -1 indicates an inactive voice.
*/
int voiceNoteIndices[numVoices];
int currentVoice = 0; // round-robin voice allocation

// Current octave (default is 5). In playNote, the frequency is adjusted by:
//   frequency = baseFrequency * 2^(currentOctave - 5)
int currentOctave = 5;

//------------------------------------------------------------------------------
// Preset and Voice Functions
//------------------------------------------------------------------------------
/**
 * @brief Loads a preset configuration and applies it to all voices.
 *
 * @param presetIndex The index of the preset to load.
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
  
  // Apply settings to each voice.
  for (int i = 0; i < numVoices; i++) {
    waveform1[i].begin(preset.waveType[0]);
    waveform1[i].amplitude(preset.amplitude[0]);
    
    waveform2[i].begin(preset.waveType[1]);
    waveform2[i].amplitude(preset.amplitude[1]);
    
    waveform3[i].begin(preset.waveType[2]);
    waveform3[i].amplitude(preset.amplitude[2]);
    
    // Apply pulse width if specified (>= 0).
    if (preset.pulseWidth[0] >= 0)
      waveform1[i].pulseWidth(preset.pulseWidth[0]);
    if (preset.pulseWidth[1] >= 0)
      waveform2[i].pulseWidth(preset.pulseWidth[1]);
    if (preset.pulseWidth[2] >= 0)
      waveform3[i].pulseWidth(preset.pulseWidth[2]);
    
    // Set envelope parameters.
    envelope1[i].delay(preset.delay);
    envelope1[i].attack(preset.attack);
    envelope1[i].hold(preset.hold);
    envelope1[i].decay(preset.decay);
    envelope1[i].sustain(preset.sustain);
    envelope1[i].release(preset.release);
  }
}

/**
 * @brief Plays a note on an available voice.
 *
 * Uses round-robin allocation and adjusts the note frequency by the current octave.
 *
 * @param noteIndex The index into noteFrequencies for the note to play.
 */
void playNote(int noteIndex) {
  // Stop the note if it's already playing.
  stopNote(noteIndex);
  
  // Allocate the next voice in round-robin fashion.
  currentVoice = (currentVoice + 1) % numVoices;
  voiceNoteIndices[currentVoice] = noteIndex;
  
  // Adjust the base frequency by the octave.
  // (noteFrequencies are defined for octave 5, so multiply by 2^(currentOctave - 5))
  double baseFreq = noteFrequencies[noteIndex] * pow(2, currentOctave - 5);
  
  // Set oscillator frequencies with detuning.
  waveform1[currentVoice].frequency(
    computeDetunedFrequency(baseFreq, baseDetune[0], currentPresetDetune[0])
  );
  waveform2[currentVoice].frequency(
    computeDetunedFrequency(baseFreq, baseDetune[1], currentPresetDetune[1])
  );
  waveform3[currentVoice].frequency(
    computeDetunedFrequency(baseFreq, baseDetune[2], currentPresetDetune[2])
  );
  
  envelope1[currentVoice].noteOn();
  
  Serial.print("Playing note ");
  Serial.print(baseFreq);
  Serial.print(" Hz on voice ");
  Serial.println(currentVoice);
}

/**
 * @brief Stops a note if it is currently playing.
 *
 * @param noteIndex The index into noteFrequencies of the note to stop.
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
  sgtl5000_1.volume(0.65);
  
  // Initialize button pins and attach Bounce objects.
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttons[i].attach(buttonPins[i]);
    buttons[i].interval(5);
  }
  
  // Initialize all voices as inactive.
  for (int i = 0; i < numVoices; i++) {
    voiceNoteIndices[i] = -1;
  }
  
  // Set the default preset.
  changeToPreset(currentPresetIndex);
  Serial.println("Setup complete.");
}

void loop() {
  // --- Process Note Buttons (indices 0-7) ---
  for (int i = 0; i < 8; i++) {
    buttons[i].update();
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
  
  // --- Process Instrument Selection Button (index 8) ---
  buttons[8].update();
  if (buttons[8].fell()) {
    Serial.print("Button on pin ");
    Serial.print(buttonPins[8]);
    Serial.println(" activated (instrument selection).");
    currentPresetIndex = (currentPresetIndex + 1) % numPresets;
    changeToPreset(currentPresetIndex);
  }
  
  // --- Process Octave Cycling Button (index 9) ---
  buttons[9].update();
  if (buttons[9].fell()) {
    Serial.print("Button on pin ");
    Serial.print(buttonPins[9]);
    Serial.println(" activated (octave cycle).");
    // Cycle the octave between 3 and 6.
    currentOctave++;
    if (currentOctave > 6) {
      currentOctave = 3;
    }
    Serial.print("Octave set to ");
    Serial.println(currentOctave);
  }
}
