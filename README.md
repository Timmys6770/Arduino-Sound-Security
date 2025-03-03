# Arduino Sound-Based Security System  

## Overview  
This project detects glass breaking sounds using a TensorFlow Lite model and triggers an Arduino-based security response with LEDs.  

## Requirements  
- **Hardware:** Arduino, LED lights, keypad, microphone  
- **Software:** Arduino IDE, Python 3.9, TensorFlow Lite, PySerial, Sounddevice  

## How to Run  
1. Upload `security_system.ino` to your Arduino.  
2. Run `sound_detection.py` in Python.  
3. If glass breaking is detected, the system will activate security LEDs.  

## Video Demonstration  
*[Link to be added after recording]*  
# Arduino Sound Security System

This project detects glass-breaking sounds using a TensorFlow Lite model and an Arduino-based LED alert system.

## How It Works
- A Python script processes microphone input to detect glass-breaking sounds.
- If a break is detected, the Python script signals the Arduino via serial communication.
- The Arduino blinks a yellow LED and prompts the user to enter a PIN.
- If the correct PIN is entered, the green LED lights up; otherwise, the red LED signals an alarm.

## Setup Instructions
1. **Clone the Repository**
2. **Upload the Arduino Code**
- Open `security_system.ino` in the Arduino IDE.
- Select your board and port, then upload the code.

3. **Run the Python Script**
4. **Test the System**
- Play a glass-breaking sound to trigger the system.
- Enter the correct PIN when prompted.

## Video Demonstration
🔗 **[Watch the Demo Here](YOUR_VIDEO_LINK_HERE)**

