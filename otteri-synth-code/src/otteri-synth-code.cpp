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
void stopNote();
void changeToPreset(int preset);

// Audio library components
AudioSynthWaveform waveform1;     // Waveform generator
AudioSynthWaveform waveform2;     // Waveform generator
AudioSynthWaveform waveform3;     // Waveform generator
AudioMixer4 mixer1;               // Mixer to combine waveforms
AudioEffectEnvelope envelope1;    // Envelope generator
AudioFilterStateVariable filter1; // Filter to modify the waveform
AudioOutputI2S i2s1;              // Audio output over I2S
AudioConnection patchCord1(waveform1, 0, mixer1, 0);
AudioConnection patchCord2(waveform2, 0, mixer1, 1);
AudioConnection patchCord3(waveform3, 0, mixer1, 2);
AudioConnection patchCord4(mixer1, envelope1);
AudioConnection patchCord5(envelope1, 0, filter1, 0);
AudioConnection patchCord6(filter1, 0, i2s1, 0);
AudioConnection patchCord7(filter1, 0, i2s1, 1);

AudioControlSGTL5000 sgtl5000_1; // Control chip for I2S

// Button setup
const int numButtons = 9;
Bounce buttons[numButtons] = {
    Bounce(0, 5), Bounce(1, 5), Bounce(2, 5), Bounce(3, 5),
    Bounce(4, 5), Bounce(5, 5), Bounce(6, 5), Bounce(7, 5),
    Bounce(21, 5)};

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

    filter1.resonance(0.5); // Slightly resonant

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
    switch (preset) {
    case 0:
        Serial.println("Sine");
        waveform1.begin(WAVEFORM_SINE);
        waveform1.amplitude(0.9);
        waveform2.begin(WAVEFORM_SAWTOOTH);
        waveform2.amplitude(0.9);
        waveform3.begin(WAVEFORM_PULSE);
        waveform3.amplitude(0.9);

        envelope1.attack(50);
        envelope1.decay(120);
        envelope1.sustain(0.7);
        envelope1.release(250);

        break;
    case 1:
        Serial.println("Square");
        waveform1.begin(WAVEFORM_SQUARE);
        waveform1.amplitude(0.9);
        waveform2.begin(WAVEFORM_SQUARE);
        waveform2.amplitude(0.9);
        waveform3.begin(WAVEFORM_TRIANGLE);
        waveform3.amplitude(0.9);
        envelope1.attack(100);
        envelope1.decay(120);
        envelope1.sustain(0.7);
        envelope1.release(500);

        break;
    case 2:
        Serial.println("Sawtooth");
        waveform1.begin(WAVEFORM_SAWTOOTH);
        waveform1.amplitude(0.9);
        waveform2.begin(WAVEFORM_SAWTOOTH);
        waveform2.amplitude(0.9);
        waveform3.begin(WAVEFORM_PULSE);
        waveform3.amplitude(0.9);
        envelope1.attack(0);
        envelope1.decay(4);
        envelope1.sustain(0.5);
        envelope1.release(10);

        break;
    case 3:
        Serial.println("Triangle");
        waveform1.begin(WAVEFORM_TRIANGLE);
        waveform1.amplitude(0.9);
        waveform2.begin(WAVEFORM_SAWTOOTH);
        waveform2.amplitude(0.9);
        waveform3.begin(WAVEFORM_PULSE);
        waveform3.amplitude(0.9);
        break;
    case 4:
        Serial.println("Pulse");
        waveform1.begin(WAVEFORM_PULSE);
        waveform1.amplitude(0.9);
        waveform2.begin(WAVEFORM_PULSE);
        waveform1.amplitude(0.9);
        waveform3.begin(WAVEFORM_PULSE);
        waveform1.amplitude(0.9);
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
    waveform1.frequency(notes[noteIndex] * pow(2, -1 / 1200.0) * pow(2, random(-2, 3) / 1200.0));
    waveform2.frequency(notes[noteIndex] * pow(2, 1 / 1200.0) * pow(2, random(-2, 3) / 1200.0));
    waveform3.frequency(notes[noteIndex] * pow(2, 2 / 1200.0) * pow(2, random(-2, 3) / 1200.0));

    envelope1.noteOn();

    Serial.print("Note played: ");
    Serial.println(notes[noteIndex]);
}

/**
 * @brief Stops the currently playing note.
 * 
 * Deactivates the sound by turning off `envelope1` and logs the action to the Serial console.
 */
void stopNote() {
    envelope1.noteOff();
    Serial.println("Note stopped.");
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
            stopNote();
        }
    }

    // Check for preset change
    buttons[8].update();
    if (buttons[8].fell()) {
        currentPreset = (currentPreset + 1) % 5;
        Serial.print("Preset changed to: ");
        Serial.print(currentPreset);
        changeToPreset(currentPreset);
    }
}
