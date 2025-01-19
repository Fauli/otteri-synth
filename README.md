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

I then adapted the generated mesh to my liking and needs.

# Hardware

## Case

The initial prototype was small, but after confirming its success, I scaled up the design to accommodate robust electronics suitable for a child's enthusiastic use. 

<img src=".attachements/prototype-closed.jpg" alt="Otteri DALL-E Image" width="400" height="300">
<img src=".attachements/prototype-open.jpg" alt="Otteri DALL-E Image" width="400" height="300">

I opted for durable arcade buttons to withstand frequent interaction.

<img src=".attachements/scaled-up-first-time.jpg" alt="Otteri DALL-E Image" width="400" height="300">

## Microcontroller

The core of the Otteri-synth is powered by a [Teensy 3.2](https://www.pjrc.com/store/teensy32.html) with the [Teensy Audio Shield](https://www.pjrc.com/store/teensy3_audio.html) as I still had them laying around at home.

The Teensy is excellent for synthesizer projects due to its high-performance ARM Cortex-M4 processor and real-time audio processing capabilities,

I'm a big fan of the teensy boards!

## Amplifier

Another component I still had laying around at home was an [Adafruit MAX7944](https://www.adafruit.com/product/1752) amplifier. For the speakers size, this will be more then enough.

## Wiring diagram

*Coming soon...*

# Software

The project began with basic sketches in the Arduino IDE, but as the software evolved, I transitioned to [PlatformIO](https://platformio.org/) for enhanced code management and scalability.

The complete codebase is available in the `otteri-synth-code` directory.

# Demo

*Coming soon...*