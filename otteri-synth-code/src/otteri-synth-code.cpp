#include <Audio.h>
#include <Bounce2.h>

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

// Sample header definition
// void someFunction(int num);
void presetSelect();
void updateVals();
void playSound(int note);
void stopSound(int note);
void checkNotes();


// Audio library components
AudioSynthWaveform waveform1[3];
AudioSynthWaveform waveform2[3];
AudioSynthWaveform waveform3[3];
AudioSynthWaveformDc dc1[3];
AudioEffectEnvelope envelopeFilter1[3];
AudioMixer4 mixer2[3];
AudioMixer4 mixer1[3];
AudioFilterLadder ladder2[3];
AudioFilterLadder ladder1[3];
AudioEffectEnvelope envelope1[3];
AudioEffectEnvelope envelope2[3];
AudioMixer4 mixerL;
AudioMixer4 mixerR;
AudioOutputI2S i2s1;

AudioConnection patchCord1(waveform1[0], 0, mixer1[0], 0);
AudioConnection patchCord2(waveform1[0], 0, mixer2[0], 0);
AudioConnection patchCord3(waveform2[0], 0, mixer1[0], 1);
AudioConnection patchCord4(waveform2[0], 0, mixer2[0], 1);
AudioConnection patchCord5(waveform3[0], 0, mixer1[0], 2);
AudioConnection patchCord6(waveform3[0], 0, mixer2[0], 2);
AudioConnection patchCord7(dc1[0], envelopeFilter1[0]);
AudioConnection patchCord8(envelopeFilter1[0], 0, ladder1[0], 1);
AudioConnection patchCord9(envelopeFilter1[0], 0, ladder2[0], 1);
AudioConnection patchCord10(mixer2[0], 0, ladder2[0], 0);
AudioConnection patchCord11(mixer1[0], 0, ladder1[0], 0);
AudioConnection patchCord12(ladder2[0], envelope2[0]);
AudioConnection patchCord13(ladder1[0], envelope1[0]);
AudioConnection patchCord14(envelope1[0], 0, mixerL, 0);
AudioConnection patchCord15(envelope2[0], 0, mixerR, 0);

AudioConnection patchCord16(waveform1[1], 0, mixer1[1], 0);
AudioConnection patchCord17(waveform1[1], 0, mixer2[1], 0);
AudioConnection patchCord18(waveform2[1], 0, mixer1[1], 1);
AudioConnection patchCord19(waveform2[1], 0, mixer2[1], 1);
AudioConnection patchCord20(waveform3[1], 0, mixer1[1], 2);
AudioConnection patchCord21(waveform3[1], 0, mixer2[1], 2);
AudioConnection patchCord22(dc1[1], envelopeFilter1[1]);
AudioConnection patchCord23(envelopeFilter1[1], 0, ladder1[1], 1);
AudioConnection patchCord24(envelopeFilter1[1], 0, ladder2[1], 1);
AudioConnection patchCord25(mixer2[1], 0, ladder2[1], 0);
AudioConnection patchCord26(mixer1[1], 0, ladder1[1], 0);
AudioConnection patchCord27(ladder2[1], envelope2[1]);
AudioConnection patchCord28(ladder1[1], envelope1[1]);
AudioConnection patchCord29(envelope1[1], 0, mixerL, 1);
AudioConnection patchCord30(envelope2[1], 0, mixerR, 1);

AudioConnection patchCord31(waveform1[2], 0, mixer1[2], 0);
AudioConnection patchCord32(waveform1[2], 0, mixer2[2], 0);
AudioConnection patchCord33(waveform2[2], 0, mixer1[2], 1);
AudioConnection patchCord34(waveform2[2], 0, mixer2[2], 1);
AudioConnection patchCord35(waveform3[2], 0, mixer1[2], 2);
AudioConnection patchCord36(waveform3[2], 0, mixer2[2], 2);
AudioConnection patchCord37(dc1[2], envelopeFilter1[2]);
AudioConnection patchCord38(envelopeFilter1[2], 0, ladder1[2], 1);
AudioConnection patchCord39(envelopeFilter1[2], 0, ladder2[2], 1);
AudioConnection patchCord40(mixer2[2], 0, ladder2[2], 0);
AudioConnection patchCord41(mixer1[2], 0, ladder1[2], 0);
AudioConnection patchCord42(ladder2[2], envelope2[2]);
AudioConnection patchCord43(ladder1[2], envelope1[2]);
AudioConnection patchCord44(envelope1[2], 0, mixerL, 2);
AudioConnection patchCord45(envelope2[2], 0, mixerR, 2);

AudioConnection patchCord125(mixerL, 0, i2s1, 0);
AudioConnection patchCord126(mixerR, 0, i2s1, 1);

AudioControlSGTL5000 sgtl5000_1;

// music variables
float myFreq = 80;
int voices = 3;
float freqValue = 1000;
float resValue = 0.1;
float octValue = 2.0;
bool notePlay1 = false;
bool noteInterupt = false;
bool notePlay2 = false;
bool notePlay3 = false;
int octave = 4;
int transpose = 0;
int scale = 0;
int presetSelected = 0;
float gainValue = 2.0;

int cents[3] = {0, 0, 0};
int semitones[3] = {0, 0, 0};
int pan[3] = {0, 0, 0};
int volume[3] = {100, 100, 100};

int attackAmp[3] = {100, 100, 100};
int decayAmp[3] = {100, 100, 100};
float sustainAmp[3] = {1, 1, 1};
int releaseAmp[3] = {100, 100, 100};

int attackFilter[3] = {100, 100, 100};
int decayFilter[3] = {100, 100, 100};
float sustainFilter[3] = {1, 1, 1};
int releaseFilter[3] = {100, 100, 100};

int cutoff[3] = {1000, 1000, 1000};
float resonance[3] = {2.0, 2.0, 2.0};
float filterAmount[3] = {2.0, 2.0, 2.0};

int waveforms[3] = {WAVEFORM_SAWTOOTH, WAVEFORM_SAWTOOTH, WAVEFORM_SAWTOOTH};
int waveformsArray[5] = {WAVEFORM_SAWTOOTH, WAVEFORM_PULSE, WAVEFORM_SQUARE, WAVEFORM_TRIANGLE, WAVEFORM_SINE};
float pulseWidth[3] = {0.25, 0.25, 0.25};

int noteArray[8];
int notePlaying = 0;

// Button setup
const int numButtons = 9;
Bounce buttons[numButtons] = {
    Bounce(0, 5), Bounce(1, 5), Bounce(2, 5), Bounce(3, 5),
    Bounce(4, 5), Bounce(5, 5), Bounce(6, 5), Bounce(7, 5),
    Bounce(8, 5)};

int currentOsc = 1;

float notesArray[60] = {
    NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
    NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
    NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
    NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8, NOTE_E8, NOTE_F8, NOTE_FS8, NOTE_G8, NOTE_GS8, NOTE_A8, NOTE_AS8, NOTE_B8};
int currentPreset = 0;

void setup() {
    Serial.begin(11520);
    AudioMemory(15);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.75);

    Serial.println("Setup started");

    // Initialize buttons
    for (int i = 0; i < numButtons; ++i) {
        pinMode(i, INPUT_PULLUP);
        buttons[i].attach(i);
        buttons[i].interval(5);
    }

    for (int i = 0; i < 3; i++) {
        dc1[i].amplitude(1.0);
    }

    presetSelect();
    Serial.println("Preset selected: " + String(presetSelected));
    Serial.println("Octave: " + String(octave));
    Serial.println("Setup complete");


}

// void updateVals() {
//     for (int i = 0; i < 3; i++) {
//         envelope1[i].attack(attackAmp[i]);
//         envelope1[i].decay(decayAmp[i]);
//         envelope1[i].sustain(sustainAmp[i]);
//         envelope1[i].release(releaseAmp[i]);
//         envelope2[i].attack(attackAmp[i]);
//         envelope2[i].decay(decayAmp[i]);
//         envelope2[i].sustain(sustainAmp[i]);
//         envelope2[i].release(releaseAmp[i]);
//         envelopeFilter1[i].attack(attackFilter[i]);
//         envelopeFilter1[i].decay(decayFilter[i]);
//         envelopeFilter1[i].sustain(sustainFilter[i]);
//         envelopeFilter1[i].release(releaseFilter[i]);
//         if (waveforms[0] == 0) {
//             waveform1[i].begin(waveformsArray[0]);
//         } else if (waveforms[0] == 1) {
//             waveform1[i].begin(waveformsArray[1]);
//         } else if (waveforms[0] == 2) {
//             waveform1[i].begin(waveformsArray[2]);
//         } else if (waveforms[0] == 3) {
//             waveform1[i].begin(waveformsArray[3]);
//         } else if (waveforms[0] == 4) {
//             waveform1[i].begin(waveformsArray[4]);
//         }
//         waveform1[i].amplitude(1.0);
//         waveform1[i].pulseWidth(pulseWidth[0]);
//         if (waveforms[1] == 0) {
//             waveform2[i].begin(waveformsArray[0]);
//         } else if (waveforms[1] == 1) {
//             waveform2[i].begin(waveformsArray[1]);
//         } else if (waveforms[1] == 2) {
//             waveform2[i].begin(waveformsArray[2]);
//         } else if (waveforms[1] == 3) {
//             waveform2[i].begin(waveformsArray[3]);
//         } else if (waveforms[1] == 4) {
//             waveform2[i].begin(waveformsArray[4]);
//         }
//         waveform2[i].amplitude(1.0);
//         waveform2[i].pulseWidth(pulseWidth[1]);
//         if (waveforms[2] == 0) {
//             waveform3[i].begin(waveformsArray[0]);
//         } else if (waveforms[2] == 1) {
//             waveform3[i].begin(waveformsArray[1]);
//         } else if (waveforms[2] == 2) {
//             waveform3[i].begin(waveformsArray[2]);
//         } else if (waveforms[2] == 3) {
//             waveform3[i].begin(waveformsArray[3]);
//         } else if (waveforms[2] == 4) {
//             waveform3[i].begin(waveformsArray[4]);
//         }
//         waveform3[i].amplitude(1.0);
//         waveform3[i].pulseWidth(pulseWidth[2]);
//         ladder1[i].frequency(cutoff[i]);
//         ladder1[i].octaveControl(filterAmount[i]);
//         ladder1[i].resonance(resonance[i]);
//         ladder2[i].frequency(cutoff[i]);
//         ladder2[i].octaveControl(filterAmount[i]);
//         ladder2[i].resonance(resonance[i]);
//         mixer1[i].gain(0, max(0, (-pan[0] + 100.0) * 0.005) * (volume[0] / 100.0) * 0.33);
//         mixer1[i].gain(1, max(0, (-pan[1] + 100.0) * 0.005) * (volume[1] / 100.0) * 0.33);
//         mixer1[i].gain(2, max(0, (-pan[2] + 100.0) * 0.005) * (volume[2] / 100.0) * 0.33);
//         mixer2[i].gain(0, max(0, (pan[0] + 100.0) * 0.005) * (volume[0] / 100.0) * 0.33);
//         mixer2[i].gain(1, max(0, (pan[1] + 100.0) * 0.005) * (volume[1] / 100.0) * 0.33);
//         mixer2[i].gain(2, max(0, (pan[2] + 100.0) * 0.005) * (volume[2] / 100.0) * 0.33);
//     }
// }


void presetSelect() {
    if (presetSelected < 0) {
        presetSelected = 9;
    }
    if (presetSelected > 9) {
        presetSelected = 0;
    }

    if (presetSelected == 0) {
        // Bass
        octave = 2.0;

        cents[0] = 5;
        cents[1] = -5;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = 0;
        semitones[2] = -12;

        pan[0] = -100;
        pan[1] = 100;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 100;

        attackAmp[0] = 0;
        attackAmp[1] = 0;
        attackAmp[2] = 0;
        decayAmp[0] = 400;
        decayAmp[1] = 400;
        decayAmp[2] = 400;
        sustainAmp[0] = 0.0;
        sustainAmp[1] = 0.0;
        sustainAmp[2] = 0.0;
        releaseAmp[0] = 10;
        releaseAmp[1] = 10;
        releaseAmp[2] = 10;

        attackFilter[0] = 0;
        attackFilter[1] = 0;
        attackFilter[2] = 0;
        decayFilter[0] = 250;
        decayFilter[1] = 250;
        decayFilter[2] = 250;
        sustainFilter[0] = 0.0;
        sustainFilter[1] = 0.0;
        sustainFilter[2] = 0.0;
        releaseFilter[0] = 10;
        releaseFilter[1] = 10;
        releaseFilter[2] = 10;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.2;
        resonance[1] = 0.2;
        resonance[2] = 0.2;
        filterAmount[0] = 3.0;
        filterAmount[1] = 3.0;
        filterAmount[2] = 3.0;

        waveforms[0] = 0;
        waveforms[1] = 0;
        waveforms[2] = 1;
    } else if (presetSelected == 1) {
        // Keys
        octave = 4.0;

        cents[0] = 2;
        cents[1] = -2;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = 0;
        semitones[2] = 12;

        pan[0] = -100;
        pan[1] = 100;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 100;

        attackAmp[0] = 0;
        attackAmp[1] = 0;
        attackAmp[2] = 0;
        decayAmp[0] = 2500;
        decayAmp[1] = 2500;
        decayAmp[2] = 2500;
        sustainAmp[0] = 0;
        sustainAmp[1] = 0;
        sustainAmp[2] = 0;
        releaseAmp[0] = 300;
        releaseAmp[1] = 300;
        releaseAmp[2] = 300;

        attackFilter[0] = 0;
        attackFilter[1] = 0;
        attackFilter[2] = 0;
        decayFilter[0] = 2500;
        decayFilter[1] = 2500;
        decayFilter[2] = 2500;
        sustainFilter[0] = 0;
        sustainFilter[1] = 0;
        sustainFilter[2] = 0;
        releaseFilter[0] = 300;
        releaseFilter[1] = 300;
        releaseFilter[2] = 300;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.2;
        resonance[1] = 0.2;
        resonance[2] = 0.2;
        filterAmount[0] = 3.0;
        filterAmount[1] = 3.0;
        filterAmount[2] = 3.0;

        waveforms[0] = 0;
        waveforms[1] = 0;
        waveforms[2] = 0;
    } else if (presetSelected == 2) {
        // Pad
        octave = 4.0;

        cents[0] = 15;
        cents[1] = -15;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = 0;
        semitones[2] = 0;

        pan[0] = -100;
        pan[1] = 100;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 0;

        attackAmp[0] = 1500;
        attackAmp[1] = 1500;
        attackAmp[2] = 1500;
        decayAmp[0] = 6000;
        decayAmp[1] = 6000;
        decayAmp[2] = 6000;
        sustainAmp[0] = 0.2;
        sustainAmp[1] = 0.2;
        sustainAmp[2] = 0.2;
        releaseAmp[0] = 1500;
        releaseAmp[1] = 1500;
        releaseAmp[2] = 1500;

        attackFilter[0] = 1500;
        attackFilter[1] = 1500;
        attackFilter[2] = 1500;
        decayFilter[0] = 6000;
        decayFilter[1] = 6000;
        decayFilter[2] = 6000;
        sustainFilter[0] = 0.2;
        sustainFilter[1] = 0.2;
        sustainFilter[2] = 0.2;
        releaseFilter[0] = 1500;
        releaseFilter[1] = 1500;
        releaseFilter[2] = 1500;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.5;
        resonance[1] = 0.5;
        resonance[2] = 0.5;
        filterAmount[0] = 2.0;
        filterAmount[1] = 2.0;
        filterAmount[2] = 2.0;

        waveforms[0] = 4;
        waveforms[1] = 4;
        waveforms[2] = 4;
    } else if (presetSelected == 3) {
        // Lead
        octave = 5.0;

        cents[0] = 10;
        cents[1] = -10;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = 0;
        semitones[2] = 0;

        pan[0] = -100;
        pan[1] = 100;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 100;

        attackAmp[0] = 0;
        attackAmp[1] = 0;
        attackAmp[2] = 0;
        decayAmp[0] = 0;
        decayAmp[1] = 0;
        decayAmp[2] = 0;
        sustainAmp[0] = 1.0;
        sustainAmp[1] = 1.0;
        sustainAmp[2] = 1.0;
        releaseAmp[0] = 2;
        releaseAmp[1] = 2;
        releaseAmp[2] = 2;

        attackFilter[0] = 0;
        attackFilter[1] = 0;
        attackFilter[2] = 0;
        decayFilter[0] = 0;
        decayFilter[1] = 0;
        decayFilter[2] = 0;
        sustainFilter[0] = 1.0;
        sustainFilter[1] = 1.0;
        sustainFilter[2] = 1.0;
        releaseFilter[0] = 2;
        releaseFilter[1] = 2;
        releaseFilter[2] = 2;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.2;
        resonance[1] = 0.2;
        resonance[2] = 0.2;
        filterAmount[0] = 2.5;
        filterAmount[1] = 2.5;
        filterAmount[2] = 2.5;

        waveforms[0] = 0;
        waveforms[1] = 0;
        waveforms[2] = 0;
    } else if (presetSelected == 4) {
        // Arp
        octave = 4.0;

        cents[0] = 0;
        cents[1] = 0;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = 0;
        semitones[2] = 0;

        pan[0] = 0;
        pan[1] = 0;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 0;
        volume[2] = 0;

        attackAmp[0] = 0;
        attackAmp[1] = 0;
        attackAmp[2] = 0;
        decayAmp[0] = 250;
        decayAmp[1] = 250;
        decayAmp[2] = 250;
        sustainAmp[0] = 0;
        sustainAmp[1] = 0;
        sustainAmp[2] = 0;
        releaseAmp[0] = 250;
        releaseAmp[1] = 250;
        releaseAmp[2] = 250;

        attackFilter[0] = 0;
        attackFilter[1] = 0;
        attackFilter[2] = 0;
        decayFilter[0] = 250;
        decayFilter[1] = 250;
        decayFilter[2] = 250;
        sustainFilter[0] = 0;
        sustainFilter[1] = 0;
        sustainFilter[2] = 0;
        releaseFilter[0] = 250;
        releaseFilter[1] = 250;
        releaseFilter[2] = 250;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.2;
        resonance[1] = 0.2;
        resonance[2] = 0.2;
        filterAmount[0] = 3.0;
        filterAmount[1] = 3.0;
        filterAmount[2] = 3.0;

        waveforms[0] = 2;
        waveforms[1] = 2;
        waveforms[2] = 2;
    } else if (presetSelected == 5) {
        // Brass
        octave = 4.0;

        cents[0] = -4;
        cents[1] = 4;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = 0;
        semitones[2] = 0;

        pan[0] = -100;
        pan[1] = 100;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 100;

        attackAmp[0] = 0;
        attackAmp[1] = 0;
        attackAmp[2] = 0;
        decayAmp[0] = 2500;
        decayAmp[1] = 2500;
        decayAmp[2] = 2500;
        sustainAmp[0] = 0.8;
        sustainAmp[1] = 0.8;
        sustainAmp[2] = 0.8;
        releaseAmp[0] = 250;
        releaseAmp[1] = 250;
        releaseAmp[2] = 250;

        attackFilter[0] = 100;
        attackFilter[1] = 100;
        attackFilter[2] = 100;
        decayFilter[0] = 2500;
        decayFilter[1] = 2500;
        decayFilter[2] = 2500;
        sustainFilter[0] = 0.8;
        sustainFilter[1] = 0.8;
        sustainFilter[2] = 0.8;
        releaseFilter[0] = 250;
        releaseFilter[1] = 250;
        releaseFilter[2] = 250;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.2;
        resonance[1] = 0.2;
        resonance[2] = 0.2;
        filterAmount[0] = 2.0;
        filterAmount[1] = 2.0;
        filterAmount[2] = 2.0;

        waveforms[0] = 0;
        waveforms[1] = 0;
        waveforms[2] = 0;
    } else if (presetSelected == 6) {
        // Organ
        octave = 5.0;

        cents[0] = 0;
        cents[1] = 0;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = -12;
        semitones[2] = 7;

        pan[0] = 0;
        pan[1] = 0;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 100;

        attackAmp[0] = 0;
        attackAmp[1] = 0;
        attackAmp[2] = 0;
        decayAmp[0] = 100;
        decayAmp[1] = 100;
        decayAmp[2] = 100;
        sustainAmp[0] = 0.6;
        sustainAmp[1] = 0.6;
        sustainAmp[2] = 0.6;
        releaseAmp[0] = 100;
        releaseAmp[1] = 100;
        releaseAmp[2] = 100;

        attackFilter[0] = 0;
        attackFilter[1] = 0;
        attackFilter[2] = 0;
        decayFilter[0] = 100;
        decayFilter[1] = 100;
        decayFilter[2] = 100;
        sustainFilter[0] = 0.6;
        sustainFilter[1] = 0.6;
        sustainFilter[2] = 0.6;
        releaseFilter[0] = 100;
        releaseFilter[1] = 100;
        releaseFilter[2] = 100;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.5;
        resonance[1] = 0.5;
        resonance[2] = 0.5;
        filterAmount[0] = 2.0;
        filterAmount[1] = 2.0;
        filterAmount[2] = 2.0;

        waveforms[0] = 4;
        waveforms[1] = 4;
        waveforms[2] = 4;
    } else if (presetSelected == 7) {
        // Flute
        octave = 5.0;

        cents[0] = -1;
        cents[1] = 1;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = 0;
        semitones[2] = 12;

        pan[0] = -100;
        pan[1] = 100;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 10;

        attackAmp[0] = 500;
        attackAmp[1] = 500;
        attackAmp[2] = 500;
        decayAmp[0] = 0;
        decayAmp[1] = 0;
        decayAmp[2] = 0;
        sustainAmp[0] = 1.0;
        sustainAmp[1] = 1.0;
        sustainAmp[2] = 1.0;
        releaseAmp[0] = 400;
        releaseAmp[1] = 400;
        releaseAmp[2] = 400;

        attackFilter[0] = 500;
        attackFilter[1] = 500;
        attackFilter[2] = 500;
        decayFilter[0] = 0;
        decayFilter[1] = 0;
        decayFilter[2] = 0;
        sustainFilter[0] = 1.0;
        sustainFilter[1] = 1.0;
        sustainFilter[2] = 1.0;
        releaseFilter[0] = 500;
        releaseFilter[1] = 500;
        releaseFilter[2] = 500;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.5;
        resonance[1] = 0.5;
        resonance[2] = 0.5;
        filterAmount[0] = 2.6;
        filterAmount[1] = 2.6;
        filterAmount[2] = 2.6;

        waveforms[0] = 3;
        waveforms[1] = 3;
        waveforms[2] = 4;
    } else if (presetSelected == 8) {
        // Chiptune
        octave = 4.0;

        cents[0] = 0;
        cents[1] = 0;
        cents[2] = 0;

        semitones[0] = 0;
        semitones[1] = 0;
        semitones[2] = 12;

        pan[0] = -100;
        pan[1] = 100;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 10;

        attackAmp[0] = 0;
        attackAmp[1] = 0;
        attackAmp[2] = 0;
        decayAmp[0] = 0;
        decayAmp[1] = 0;
        decayAmp[2] = 0;
        sustainAmp[0] = 1.0;
        sustainAmp[1] = 1.0;
        sustainAmp[2] = 1.0;
        releaseAmp[0] = 100;
        releaseAmp[1] = 100;
        releaseAmp[2] = 100;

        attackFilter[0] = 0;
        attackFilter[1] = 0;
        attackFilter[2] = 0;
        decayFilter[0] = 0;
        decayFilter[1] = 0;
        decayFilter[2] = 0;
        sustainFilter[0] = 1.0;
        sustainFilter[1] = 1.0;
        sustainFilter[2] = 1.0;
        releaseFilter[0] = 100;
        releaseFilter[1] = 100;
        releaseFilter[2] = 100;

        cutoff[0] = 2000;
        cutoff[1] = 2000;
        cutoff[2] = 2000;
        resonance[0] = 0.0;
        resonance[1] = 0.0;
        resonance[2] = 0.0;
        filterAmount[0] = 2.5;
        filterAmount[1] = 2.5;
        filterAmount[2] = 2.5;

        waveforms[0] = 1;
        waveforms[1] = 1;
        waveforms[2] = 1;
    } else if (presetSelected == 9) {
        // Wow
        octave = 4.0;
        cents[0] = 5;
        cents[1] = -5;
        cents[2] = 0;

        semitones[0] = 7;
        semitones[1] = 0;
        semitones[2] = -12;

        pan[0] = -50;
        pan[1] = 50;
        pan[2] = 0;

        volume[0] = 100;
        volume[1] = 100;
        volume[2] = 100;

        attackAmp[0] = 400;
        attackAmp[1] = 400;
        attackAmp[2] = 400;
        decayAmp[0] = 4000;
        decayAmp[1] = 4000;
        decayAmp[2] = 4000;
        sustainAmp[0] = 0.8;
        sustainAmp[1] = 0.8;
        sustainAmp[2] = 0.8;
        releaseAmp[0] = 10;
        releaseAmp[1] = 10;
        releaseAmp[2] = 10;

        attackFilter[0] = 800;
        attackFilter[1] = 800;
        attackFilter[2] = 800;
        decayFilter[0] = 6000;
        decayFilter[1] = 6000;
        decayFilter[2] = 6000;
        sustainFilter[0] = 0.2;
        sustainFilter[1] = 0.2;
        sustainFilter[2] = 0.2;
        releaseFilter[0] = 10;
        releaseFilter[1] = 10;
        releaseFilter[2] = 10;

        cutoff[0] = 1000;
        cutoff[1] = 1000;
        cutoff[2] = 1000;
        resonance[0] = 0.8;
        resonance[1] = 0.8;
        resonance[2] = 0.8;
        filterAmount[0] = 1.5;
        filterAmount[1] = 1.5;
        filterAmount[2] = 1.5;

        waveforms[0] = 0;
        waveforms[1] = 0;
        waveforms[2] = 0;
    }
    // updateVals();
}

void playSound(int note) {
    stopSound(note);
    myFreq = notesArray[note];
    notePlaying += 1;
    if (notePlaying >= voices) {
        notePlaying = 0;
    }
    noteArray[notePlaying] = note;
    Serial.println("Note: " + String(note) + " Playing: " + String(notePlaying));

    // Play Notes with intial pitch, semitones, detune, and random drift offset
    waveform1[notePlaying].frequency(notesArray[note + semitones[0]] * pow(2, cents[0] / 1200.0) * pow(2, random(-2, 3) / 1200.0));
    waveform2[notePlaying].frequency(notesArray[note + semitones[1]] * pow(2, cents[1] / 1200.0) * pow(2, random(-2, 3) / 1200.0));
    waveform3[notePlaying].frequency(notesArray[note + semitones[2]] * pow(2, cents[2] / 1200.0) * pow(2, random(-2, 3) / 1200.0));
    envelope1[notePlaying].noteOn();
    envelope2[notePlaying].noteOn();
    envelopeFilter1[notePlaying].noteOn();
}

void stopSound(int note) {
    for (int num = 0; num < voices; num++) {
        if (noteArray[num] == note) {
            envelope1[num].noteOff();
            envelope2[num].noteOff();
            envelopeFilter1[num].noteOff();
            noteArray[num] = 0;
        }
    }
    Serial.println("Note: " + String(note) + " Stopped");
}

void checkNotes() {
    // update all buttons
    for (int i = 0; i < numButtons; ++i) {
        buttons[i].update();
    }
    

    // // then act on them
    if (buttons[0].fallingEdge()) {
        playSound(12 * octave + transpose);
        Serial.println("Button 1: " + String(buttons[0].fallingEdge()));
    } else if (buttons[0].risingEdge()) {
        stopSound(12 * octave + transpose);
        Serial.println("Button 1: " + String(buttons[0].risingEdge()));
    }

    if (buttons[1].fallingEdge()) {
        playSound(12 * octave + transpose + 2);
    } else if (buttons[1].risingEdge()) {
        stopSound(12 * octave + transpose + 2);
    }

    if (scale == 0) {
        if (buttons[2].fallingEdge()) {
            playSound(12 * octave + transpose + 4);
        } else if (buttons[2].risingEdge()) {
            stopSound(12 * octave + transpose + 4);
        }
    } else {
        if (buttons[2].fallingEdge()) {
            playSound(12 * octave + transpose + 3);
        } else if (buttons[2].risingEdge()) {
            stopSound(12 * octave + transpose + 3);
        }
    }

    if (buttons[3].fallingEdge()) {
        playSound(12 * octave + transpose + 5);
    } else if (buttons[3].risingEdge()) {
        stopSound(12 * octave + transpose + 5);
    }

    if (buttons[4].fallingEdge()) {
        playSound(12 * octave + transpose + 7);
    } else if (buttons[4].risingEdge()) {
        stopSound(12 * octave + transpose + 7);
    }

    if (scale == 0) {
        if (buttons[5].fallingEdge()) {
            playSound(12 * octave + transpose + 9);
        } else if (buttons[5].risingEdge()) {
            stopSound(12 * octave + transpose + 9);
        }
    } else {
        if (buttons[5].fallingEdge()) {
            playSound(12 * octave + transpose + 8);
        } else if (buttons[5].risingEdge()) {
            stopSound(12 * octave + transpose + 8);
        }
    }

    if (scale == 0) {
        if (buttons[6].fallingEdge()) {
            playSound(12 * octave + transpose + 11);
        } else if (buttons[6].risingEdge()) {
            stopSound(12 * octave + transpose + 11);
        }
    } else {
        if (buttons[6].fallingEdge()) {
            playSound(12 * octave + transpose + 10);
        } else if (buttons[6].risingEdge()) {
            stopSound(12 * octave + transpose + 10);
        }
    }

    if (buttons[7].fallingEdge()) {
        playSound(12 * octave + transpose + 12);
    } else if (buttons[7].risingEdge()) {
        stopSound(12 * octave + transpose + 12);
    }
}

void loop() {
    checkNotes();
}
