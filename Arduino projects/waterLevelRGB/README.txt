Arduino Water Level Monitoring System

This project uses an Arduino UNO to monitor water levels with an RGB LED and a buzzer for visual and auditory alerts. The system reads water levels using a water level sensor and triggers responses based on defined thresholds.
I got the idea from my portable AC, that tends to fill a bucket with condensed water from the air it cools. When the bucket is near full, and alert system would be ideal to warn the user and prevent water damage.

Features

    RGB LED Alerts: Displays water level status:
        White: Empty
        Green: Low
        Yellow: Medium
        Red: High or Critical (S.O.S blink when critical)

    Buzzer Notifications:
        Unique tones for Low, Medium and High levels.
        S.O.S pattern for Critical levels.
    Customizable Thresholds: Adjust water level thresholds to match your sensor's analog values.

Components

    Arduino UNO
    RGB LED
    Buzzer
    Water Level Sensor

How It Works

    The water level sensor reads the current water level.
    Based on the reading:
        The RGB LED changes color.
        The buzzer plays specific tones or patterns.
    Alerts update every second (can be adjusted).

Setup

    Connect the components as specified in the code.
    Upload the code to your Arduino UNO.
    Adjust the thresholds (emptyThreshold, lowerThreshold, etc.) as needed.

Usage

    Power the Arduino to activate the system.
    Monitor LED colors and buzzer alerts for water level changes.

License

This project is open-source and can be modified or redistributed. Credit to the author, Abdurrahman Faig, is appreciated.