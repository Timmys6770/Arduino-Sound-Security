#include <Keypad.h>

const int redLED = 22;
const int greenLED = 23;
const int yellowLED = 24;
const int whiteLED = 25;

// Define pin for keypad
const byte ROW_NUM = 4; // four rows
const byte COLUMN_NUM = 3; // three columns
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String password = "1738"; // Set the password
String enteredPassword = "";  // String to store entered password
int wrongAttempts = 0;

unsigned long yellowStartTime = 0;
unsigned long pinEntryTimeout = 20000;  // 20 seconds timeout for pin entry
unsigned long yellowBlinkInterval = 500; // 500ms for blinking yellow LED
unsigned long yellowBlinkDuration = 20000; // 20 seconds for yellow blinking
bool yellowBlinking = false;
bool greenLEDOn = false;

void setup() {
  // Set LED pins as outputs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);

  // Start with all LEDs off
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(whiteLED, HIGH);  

  // Initialize serial communication
  Serial.begin(9600);

  // Turn on white LED immediately without flickering
}

void loop() {
  // Check if glass breaking signal is received from Python
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (incomingByte == '1') {
      startYellowBlinking();
    }
  }

  // Handle keypad input for pin entry
  char key = keypad.getKey();
  if (key) {
    enteredPassword += key;
    Serial.println(key);
  }

  // Check if entered password matches the stored password
  if (enteredPassword.length() == password.length()) {
    if (enteredPassword == password) {
      correctPasswordEntered();
    } else {
      wrongAttempts++;
      if (wrongAttempts >= 3) {
        incorrectPassword();
      }
      enteredPassword = ""; // Reset entered password
    }
  }

  // If no pin entered in the allowed time, go to red LED
  if (yellowBlinking && millis() - yellowStartTime >= pinEntryTimeout) {
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);
    delay(1000000);
    digitalWrite(redLED, LOW);
    digitalWrite(whiteLED, HIGH);
  }

  // Manage the blinking of yellow LED
  if (yellowBlinking && millis() - yellowStartTime < yellowBlinkDuration) {
    if (millis() % yellowBlinkInterval < yellowBlinkInterval / 2) {
      digitalWrite(yellowLED, HIGH);
    } else {
      digitalWrite(yellowLED, LOW);
    }
  }

  // Ensure green LED turns off after 10 seconds
  if (greenLEDOn && millis() - yellowStartTime >= 1000000) {
      digitalWrite(greenLED, LOW);
      digitalWrite(whiteLED, HIGH);;
  }
}

void startYellowBlinking() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(whiteLED, LOW);

  if (greenLEDOn) {
    digitalWrite(yellowLED, LOW);
    return;
  }

  yellowBlinking = true;
  yellowStartTime = millis();
}

void correctPasswordEntered() {
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(whiteLED, LOW);

  digitalWrite(greenLED, HIGH);
  delay(1000000);

  digitalWrite(greenLED, LOW);
  digitalWrite(whiteLED, HIGH);;
}

void incorrectPassword() {
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(whiteLED, LOW);

  digitalWrite(redLED, HIGH);
  delay(1000000);
  
  digitalWrite(redLED, LOW);
  digitalWrite(whiteLED, HIGH);
}
