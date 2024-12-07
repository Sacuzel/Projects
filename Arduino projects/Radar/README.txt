This is a simple radar project that I made as a proof of concept. The wiring diagram can be found in the project folder.
If the radar is turned on without power bring on, it will give a warning melody. This same melody is also played if the radar finishes its scan cycle, and the power is off. When the power is on, the radar makes a "happy" sound and starts scanning.
During the scan the buzzer makes clicks if an obstacle has been detected. If the number of detected obstacles is over 4, the radar will play a melody. The radar will also zero this number and play a melody again if the detected obstacles exceed 4, basically playing every fifth detection.
The range of the radar has been set to 50 cm and can scan 300 degrees around itself. Additionally, the radar will always finish its cycle even if the power is cut off. Radars are generally never “turned off” and left mounted, so it makes sense for the radar to warn the user before shutting down.

I followed this tutorial by sealsterr1 and improved upon it:

https://projecthub.arduino.cc/sealsterr1/radar-system-using-arduino-uno-0bd12c
Feel free to use the project for your own needs.
Made by Abdurrahman Faig

A video demonstration of the project can be found here:

https://files.catbox.moe/vt6kny.mp4
