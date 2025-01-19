# Otteri-synth

<p align="center">
    <img src=".attachements/otteri-logo.png" alt="Otteri DALL-E Image" width="300" height="300">
    <br/>
    Cause otters like to play!
</p>

# Introduction
For fun I wanted to create a synthesizer that looks like an otter. It should be my daughters firsth synth :)

# Design
As the base I generated an image using DALL-E and fed it into [TRELLIS](https://huggingface.co/JeffreyXiang/TRELLIS-image-large).
I was able to create an image in DALL-E that exactly matched what I had in mind.

<img src=".attachements/generated-otter-synth.webp" alt="Otteri DALL-E Image" width="300" height="300">


TRELLIS created an amazing mesh out of it that I could directly use as a starting point in [Blender](https://www.blender.org/).
You can find and download the mesh [here](.attachements/otteri-synth-generated.glb).

I then adopted the generated mesh to my liking and needs.

# Hardware

## Case

Once I had a design that was happy with, I printed a small prototype.
After a successful initial print, I scaled it up and started to add the electronics.

<img src=".attachements/prototype-closed.jpg" alt="Otteri DALL-E Image" width="400" height="300">
<img src=".attachements/prototype-open.jpg" alt="Otteri DALL-E Image" width="400" height="300">

Because my young daughter will likel smash hard on the controls, I decided to go with arcade buttons.

<img src=".attachements/scaled-up-first-time.jpg" alt="Otteri DALL-E Image" width="400" height="300">

## Microcontroller

I'm using a [Teensy 3.2](https://www.pjrc.com/store/teensy32.html) with the [Teensy Audio Shield](https://www.pjrc.com/store/teensy3_audio.html) as I still had them laying around at home.

## Amplifier

Another component I still had laying around at home was an [Adafruit MAX7944](https://www.adafruit.com/product/1752) amplifier. For the speakers size, this will be more then enough.

## Wiring diagram

todo..


# Software
Initially I started with the basic .ino coding in Arduino IDE. But as the code gre, I decided to migrate to [PlatformIO](https://platformio.org/).

The code can be found in the otteri-synth-code folder.


# Demo

todo..