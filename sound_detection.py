import serial
import sounddevice as sd
import numpy as np
import tensorflow as tf

# Set up serial communication with the Arduino
arduino = serial.Serial('/dev/cu.usbmodem142101', 9600)  # Adjust port name if needed
arduino.write(b'0')  # Ensure white LED is on immediately

# Load the TensorFlow Lite model
interpreter = tf.lite.Interpreter(model_path="/Users/timmyseriki/soundclassifier_with_metadata.tflite")
interpreter.allocate_tensors()

# Get input and output details
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Function to process audio for model
def preprocess_audio(audio):
    audio = np.squeeze(audio)  # Remove extra dimensions
    audio = audio.astype(np.float32)  # Convert to float32 (required for TF Lite)
    
    # Assuming the expected shape is (44032,)
    expected_length = 44032
    if audio.shape[0] < expected_length:
        # If the audio is shorter than expected, pad it
        audio = np.pad(audio, (0, expected_length - audio.shape[0]))
    elif audio.shape[0] > expected_length:
        # If the audio is longer than expected, trim it
        audio = audio[:expected_length]
    
    return np.expand_dims(audio, axis=0)  # Add batch dimension

# Record a short audio clip (5 sec)
duration = 10  # seconds
sample_rate = 44100
audio_data = sd.rec(int(duration * sample_rate), samplerate=sample_rate, channels=1)
sd.wait()

# Preprocess audio and run inference
audio_input = preprocess_audio(audio_data)
interpreter.set_tensor(input_details[0]['index'], audio_input)
interpreter.invoke()
output_data = interpreter.get_tensor(output_details[0]['index'])

# Load labels
labels = ["Background Noise", "Glass Breaking"]
prediction = np.argmax(output_data)

# Send command to Arduino based on prediction
if labels[prediction] == "Glass Breaking":
    print("Glass breaking detected!")
    arduino.write(b'1')  # Send signal to Arduino to blink yellow LED
else:
    print("No glass breaking detected.")
    arduino.write(b'0')  # Send signal to Arduino to keep white LED on
