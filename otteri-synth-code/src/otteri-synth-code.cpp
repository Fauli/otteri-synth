#include <Audio.h>
#include <Bounce2.h>

// define notes
const double NOTE_C4 = 261.63;
const double NOTE_CS4 = 277.18;
const double NOTE_D4 = 293.66;
const double NOTE_DS4 = 311.13;
const double NOTE_E4 = 329.63;
const double NOTE_F4 = 349.23;
const double NOTE_FS4 = 369.99;
const double NOTE_G4 = 392.00;
const double NOTE_GS4 = 415.30;
const double NOTE_A4 = 440.00;
const double NOTE_AS4 = 466.16;
const double NOTE_B4 = 493.88;
const double NOTE_C5 = 523.25;
const double NOTE_CS5 = 554.37;
const double NOTE_D5 = 587.33;
const double NOTE_DS5 = 622.25;
const double NOTE_E5 = 659.25;
const double NOTE_F5 = 698.46;
const double NOTE_FS5 = 739.99;
const double NOTE_G5 = 783.99;
const double NOTE_GS5 = 830.61;
const double NOTE_A5 = 880.00;
const double NOTE_AS5 = 932.33;
const double NOTE_B5 = 987.77;
const double NOTE_C6 = 1046.50;
const double NOTE_CS6 = 1108.73;
const double NOTE_D6 = 1174.66;
const double NOTE_DS6 = 1244.51;
const double NOTE_E6 = 1318.51;
const double NOTE_F6 = 1396.91;
const double NOTE_FS6 = 1479.98;
const double NOTE_G6 = 1567.98;
const double NOTE_GS6 = 1661.22;
const double NOTE_A6 = 1760.00;
const double NOTE_AS6 = 1864.66;
const double NOTE_B6 = 1975.53;
const double NOTE_C7 = 2093.00;
const double NOTE_CS7 = 2217.46;
const double NOTE_D7 = 2349.32;
const double NOTE_DS7 = 2489.02;
const double NOTE_E7 = 2637.02;
const double NOTE_F7 = 2793.83;
const double NOTE_FS7 = 2959.96;
const double NOTE_G7 = 3135.96;
const double NOTE_GS7 = 3322.44;
const double NOTE_A7 = 3520.00;
const double NOTE_AS7 = 3729.31;
const double NOTE_B7 = 3951.07;
const double NOTE_C8 = 4186.01;
const double NOTE_CS8 = 4434.92;
const double NOTE_D8 = 4698.63;
const double NOTE_DS8 = 4978.03;
const double NOTE_E8 = 5274.04;
const double NOTE_F8 = 5587.65;
const double NOTE_FS8 = 5919.91;
const double NOTE_G8 = 6271.93;
const double NOTE_GS8 = 6644.88;
const double NOTE_A8 = 7040.00;
const double NOTE_AS8 = 7458.62;
const double NOTE_B8 = 7902.13;

// Function declarations
void playNote(int noteIndex);
void stopNote(int note);
void changeToPreset(int preset);

// Audio library components
AudioSynthWaveform waveform1[3];  // Waveform generator
AudioSynthWaveform waveform2[3];  // Waveform generator
AudioSynthWaveform waveform3[3];  // Waveform generator
AudioMixer4 mixer1[3];            // Mixer to combine waveforms
AudioEffectEnvelope envelope1[3]; // Envelope generator

AudioMixer4 outputMixer;

AudioOutputI2S i2s1; // Audio output over I2S

// Somehow this fails, resp. crashes? TODO: find out why?
// AudioConnection patchCord1(waveform1[0], 0, mixer1[0], 0);
// AudioConnection patchCord2(waveform2[0], 0, mixer1[0], 1);
// AudioConnection patchCord3(waveform3[0], 0, mixer1[0], 2);
// AudioConnection patchCord4(mixer1[0], 0, ladder1[0], 0);
// AudioConnection patchCord5(dc1[0], 0, envelopeFilter1[0], 0);
// AudioConnection patchCord6(envelopeFilter1[0], 0, ladder1[0], 1);
// AudioConnection patchCord7(ladder1[0], 0, envelope1[0], 0);
// AudioConnection patchCord8(envelope1[0], 0, outputMixer, 0);

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

AudioControlSGTL5000 sgtl5000_1; // Control chip for I2S

int numVoices = 3;
int noteArray[8];
int activeNote = 0;

int cents[3] = {0, 0, 0};
int semitones[3] = {0, 0, 0};
int octaves = 1;

// Button setup
const int numButtons = 9;
Bounce buttons[numButtons] = {
    Bounce(0, 5), Bounce(1, 5), Bounce(2, 5), Bounce(3, 5),
    Bounce(4, 5), Bounce(5, 5), Bounce(8, 5), Bounce(21, 5),
    Bounce(20, 5)};
float notes[numButtons - 1] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int currentPreset = 0;

/**
 * @brief Setup function for the synthesizer.
 *
 * Initializes the serial communication, audio memory, audio control, and buttons.
 * Sets up the default waveform and envelope settings.
 */
void setup() {
    Serial.begin(9600);
    AudioMemory(15);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.4);

    // Initialize buttons
    for (int i = 0; i < numButtons; ++i) {
        pinMode(i, INPUT_PULLUP);
        buttons[i].attach(i);
        buttons[i].interval(5);
    }

    changeToPreset(currentPreset);

    Serial.println("Setup complete. Default waveform set.");
}

/**
 * @brief Changes the waveform and envelope settings based on the selected preset.
 *
 * Configures `waveform1`, `waveform2`, and `waveform3` to specific waveforms and amplitude settings.
 * Adjusts the `envelope1` parameters such as attack, decay, sustain, and release based on the chosen preset.
 * Each preset corresponds to a unique configuration and is logged to the Serial console.
 *
 * @param preset The index of the preset configuration to apply.
 */
void changeToPreset(int preset) {
    cents[0] = 0;
    cents[1] = 0;
    cents[2] = 0;
    switch (preset) {
    case 0:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Sine");
            waveform1[i].begin(WAVEFORM_SINE);
            waveform1[i].amplitude(0.9);
            waveform2[i].begin(WAVEFORM_SAWTOOTH);
            waveform2[i].amplitude(0.9);
            waveform3[i].begin(WAVEFORM_PULSE);
            waveform3[i].amplitude(0.9);

            envelope1[i].attack(50);
            envelope1[i].decay(120);
            envelope1[i].sustain(0.7);
            envelope1[i].release(250);
            cents[0] = -10;
            cents[1] = 9;
            cents[2] = 0;
        }
        break;
    case 1:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Ambient Pad");
            waveform1[i].begin(WAVEFORM_SINE);
            waveform1[i].amplitude(0.8);
            waveform2[i].begin(WAVEFORM_TRIANGLE);
            waveform2[i].amplitude(0.5);
            waveform3[i].begin(WAVEFORM_SAWTOOTH);
            waveform3[i].amplitude(0.3);

            envelope1[i].attack(800);
            envelope1[i].decay(1000);
            envelope1[i].sustain(0.6);
            envelope1[i].release(1500);
            cents[0] = -4;
            cents[1] = 4;
            cents[2] = 0;
        }
        break;
    case 2:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Percussive Stab");
            waveform1[i].begin(WAVEFORM_SQUARE);
            waveform1[i].amplitude(0.9);
            waveform2[i].begin(WAVEFORM_PULSE);
            waveform2[i].amplitude(0.9);
            waveform3[i].begin(WAVEFORM_SAMPLE_HOLD);
            waveform3[i].amplitude(0.4);

            envelope1[i].attack(0);
            envelope1[i].decay(200);
            envelope1[i].sustain(0);
            envelope1[i].release(100);
        }
        break;
    case 3:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Deep Bass");
            waveform1[i].begin(WAVEFORM_SINE);
            waveform1[i].amplitude(1.0);
            waveform2[i].begin(WAVEFORM_SAWTOOTH);
            waveform2[i].amplitude(0.7);
            waveform3[i].begin(WAVEFORM_SQUARE);
            waveform3[i].amplitude(0.5);

            envelope1[i].attack(5);
            envelope1[i].decay(400);
            envelope1[i].sustain(0.8);
            envelope1[i].release(200);
        }
        break;
    case 4:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Lead Synth");
            waveform1[i].begin(WAVEFORM_SAWTOOTH);
            waveform1[i].amplitude(0.8);
            waveform2[i].begin(WAVEFORM_PULSE);
            waveform2[i].amplitude(0.6);
            waveform3[i].begin(WAVEFORM_TRIANGLE);
            waveform3[i].amplitude(0.5);

            envelope1[i].attack(50);
            envelope1[i].decay(300);
            envelope1[i].sustain(0.7);
            envelope1[i].release(400);
        }
        break;
    case 5:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Space Whispers");
            waveform1[i].begin(WAVEFORM_SINE);
            waveform1[i].amplitude(0.3);
            waveform2[i].begin(WAVEFORM_TRIANGLE);
            waveform2[i].amplitude(0.3);
            waveform3[i].begin(WAVEFORM_TRIANGLE_VARIABLE);
            waveform3[i].amplitude(0.3);
            waveform3[i].pulseWidth(0.75); // Variable pulse width for a dynamic texture

            envelope1[i].attack(500);
            envelope1[i].decay(2000);
            envelope1[i].sustain(0.5);
            envelope1[i].release(3000);
        }
        break;
    case 6:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Ethereal Voices");
            waveform1[i].begin(WAVEFORM_SINE);
            waveform1[i].amplitude(0.8);
            waveform2[i].begin(WAVEFORM_TRIANGLE);
            waveform2[i].amplitude(0.5);
            waveform3[i].begin(WAVEFORM_SAWTOOTH);
            waveform3[i].amplitude(0.3);

            envelope1[i].delay(100);    // Delay before starting
            envelope1[i].attack(1200);  // Very slow attack for a smooth onset
            envelope1[i].hold(500);     // Hold to sustain the sound
            envelope1[i].decay(800);    // Slow decay
            envelope1[i].sustain(0.6);  // Sustained level
            envelope1[i].release(1500); // Very long release for a gradual fade
        }
        break;
    case 7:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Glitch Machine");
            waveform1[i].begin(WAVEFORM_SQUARE);
            waveform1[i].amplitude(0.8);
            waveform2[i].begin(WAVEFORM_TRIANGLE);
            waveform2[i].amplitude(0.6);
            waveform3[i].begin(WAVEFORM_SAMPLE_HOLD);
            waveform3[i].amplitude(0.5);

            envelope1[i].attack(10);
            envelope1[i].decay(50);
            envelope1[i].sustain(0.1);
            envelope1[i].release(10);
        }
        break;
    case 8:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Retro Arcade");
            waveform1[i].begin(WAVEFORM_BANDLIMIT_SQUARE);
            waveform1[i].amplitude(1.0);
            waveform2[i].begin(WAVEFORM_SQUARE);
            waveform2[i].amplitude(0.7);
            waveform3[i].begin(WAVEFORM_SINE);
            waveform3[i].amplitude(0.4);

            envelope1[i].attack(5);
            envelope1[i].decay(300);
            envelope1[i].sustain(0.8);
            envelope1[i].release(150);
        }
        break;
    case 9:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Cyber Drone");
            waveform1[i].begin(WAVEFORM_SAWTOOTH);
            waveform1[i].amplitude(0.9);
            waveform2[i].begin(WAVEFORM_BANDLIMIT_PULSE);
            waveform2[i].amplitude(0.9);
            waveform3[i].begin(WAVEFORM_SAWTOOTH_REVERSE);
            waveform3[i].amplitude(0.9);

            envelope1[i].attack(100);
            envelope1[i].decay(1200);
            envelope1[i].sustain(0.6);
            envelope1[i].release(2500);
        }
        break;
    case 10:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Cinematic Sweep");
            waveform1[i].begin(WAVEFORM_SAWTOOTH);
            waveform1[i].amplitude(0.7);
            waveform2[i].begin(WAVEFORM_TRIANGLE);
            waveform2[i].amplitude(0.3);
            waveform3[i].begin(WAVEFORM_PULSE);
            waveform3[i].amplitude(0.5);
            waveform3[i].pulseWidth(0.25);

            envelope1[i].delay(50);    // Start with a delay
            envelope1[i].attack(500);  // Slow attack for a sweeping effect
            envelope1[i].hold(100);    // Short hold
            envelope1[i].decay(400);   // Gradual decay
            envelope1[i].sustain(0.4); // Lower sustain level for a fading tail
            envelope1[i].release(800); // Long release for a smooth tail-off
        }
        break;
    case 11:
        for (int i = 0; i < numVoices; i++) {
            Serial.println("Staccato Plucks");
            waveform1[i].begin(WAVEFORM_TRIANGLE);
            waveform1[i].amplitude(1.0);
            waveform2[i].begin(WAVEFORM_SQUARE);
            waveform2[i].amplitude(0.5);
            waveform3[i].begin(WAVEFORM_SINE);
            waveform3[i].amplitude(0.3);

            envelope1[i].delay(0);    // No delay
            envelope1[i].attack(10);  // Very quick attack
            envelope1[i].hold(5);     // Minimal hold
            envelope1[i].decay(20);   // Quick decay
            envelope1[i].sustain(0);  // No sustain
            envelope1[i].release(50); // Quick release for a sharp pluck
        }
        break;
    }
}

/**
 * @brief Plays a musical note with slight detuning.
 *
 * Sets frequencies for three waveform generators based on a note index.
 *
 * @param noteIndex Index of the note in the `notes` array, determining the base frequency.
 */
void playNote(int noteIndex) {
    stopNote(noteIndex);
    activeNote += 1;
    if (activeNote >= numVoices) {
        activeNote = 0;
    }
    noteArray[activeNote] = noteIndex;
    Serial.println("Note: " + String(notes[noteIndex]) + " Playing: " + String(activeNote));

    waveform1[activeNote].frequency(notes[noteIndex] * pow(2, -1 / 1200.0) * pow(2, cents[0] / 1200.0) * pow(2, random(-2, 3) / 1200.0));
    waveform2[activeNote].frequency(notes[noteIndex] * pow(2, 1 / 1200.0) * pow(2, cents[1] / 1200.0) * pow(2, random(-2, 3) / 1200.0));
    waveform3[activeNote].frequency(notes[noteIndex] * pow(2, 2 / 1200.0) * pow(2, cents[2] / 1200.0) * pow(2, random(-2, 3) / 1200.0));

    envelope1[activeNote].noteOn();

    Serial.print("Note played: ");
    Serial.println(notes[noteIndex]);
}

/**
 * @brief Stops the currently playing note.
 *
 * Deactivates the sound by turning off `envelope1` and logs the action to the Serial console.
 */
void stopNote(int note) {
    for (int j = 0; j < numVoices; j++) {
        if (noteArray[j] == note) {
            envelope1[j].noteOff();
            noteArray[j] = 0;
        }
    }
    Serial.println("Note: " + String(note) + " Stopped");
}

/**
 * @brief Main program loop.
 *
 * Runs the main program infinitely, checking for button presses and releases.
 */
void loop() {
    for (int i = 0; i < numButtons - 1; ++i) {
        buttons[i].update();
        if (buttons[i].fell()) {
            playNote(i);
        }
        if (buttons[i].rose()) {
            stopNote(i);
        }
    }

    // Check for preset change
    buttons[8].update();
    if (buttons[8].fell()) {
        currentPreset = (currentPreset + 1) % 11;
        Serial.print("Preset changed to: ");
        Serial.print(currentPreset);
        changeToPreset(currentPreset);
    }
}
