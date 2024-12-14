The goal of this project was to create a voice controlled lamp. The lamp would turn on or off if user claps (or taps the table) twice within one second.
In order to detect the changing sound elevels I had to extract the middle average of detected analog sound signals within certain time window, otherwise the sound level detection was not reliable.
Originally I also wanted to attach the lamp to a servo motor and implement a frequency detection system wherein user could control the orientation of the lamp by whistling.
Unfortunately the only "microphone" module I had access to was KY-038 sound sensor module, which is not capable of detecting frequency changes with any real accuracy.

Despite this, I wanted to challenge myself. After scouring the internet for different freequency sampling methods, I decided to try an FFT (Fast Fourier Transform) algorithm using an FFT library in Arduino.
This didn't really work, so I decided to try using autocorrelation. This did not yield any satisfactory results either. However, I am going to try my program again when I have access to a better microphone module.


A video demonstration of the project can be found here:
https://files.catbox.moe/iz0hc9.mp4
