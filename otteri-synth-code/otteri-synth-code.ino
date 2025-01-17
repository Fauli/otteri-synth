#include <Audio.h>
#include <Bounce.h>

AudioSynthWaveform waveform1, waveform2;
AudioMixer4 mixer;                 // Add a mixer to handle both waveforms
AudioOutputI2S i2s1;
AudioConnection patchCord1(waveform1, 0, mixer, 0);
AudioConnection patchCord2(waveform2, 0, mixer, 1);
AudioConnection patchCord3(mixer, 0, i2s1, 0); // Connect mixer to I2S output left channel
AudioConnection patchCord4(mixer, 0, i2s1, 1); // Connect mixer to I2S output right channel
AudioControlSGTL5000 sgtl5000_1;

Bounce button1 = Bounce(0, 15);
Bounce button2 = Bounce(1, 15);

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);

  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);

  waveform1.begin(WAVEFORM_SINE);
  waveform1.frequency(440);
  waveform1.amplitude(0);

  waveform2.begin(WAVEFORM_SINE);
  waveform2.frequency(493.88);
  waveform2.amplitude(0);

  mixer.gain(0, 0.5); 
  mixer.gain(1, 0.5);

  Serial.println("Setup complete with mixer");
}

void loop() {
  button1.update();
  button2.update();

  if (button1.fallingEdge()) {
    waveform1.amplitude(0.75);
  }
  if (button1.risingEdge()) {
    waveform1.amplitude(0);
  }
  if (button2.fallingEdge()) {
    waveform2.amplitude(0.75);
  }
  if (button2.risingEdge()) {
    waveform2.amplitude(0);
  }
}
