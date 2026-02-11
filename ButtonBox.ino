#include <HID-Project.h>

#define BUTTON_PIN_1 2  // Button 1 (Ctrl+C)
#define BUTTON_PIN_2 16 // Button 2 (Ctrl+V)
#define BUTTON_PIN_3 4  // Button 3 (Ctrl+S)
#define BUTTON_PIN_4 5  // Button 4 (Open Chrome)
#define BUTTON_PIN_5 6  // Button 5 (Ctrl+2)
#define BUTTON_PIN_6 7  // Button 6 (Mute/Unmute)
#define BUTTON_PIN_7 15 // Button 7 (Always On, LED on at all times)

#define LED_PIN_1 9     // LED control pin for other buttons
#define LED_PIN_6 10    // LED control pin for mute button
#define LED_PIN_7 14     // LED control pin for Button 7

unsigned long lastDebounceTime = 0;   // The last time the button was toggled
unsigned long debounceDelay = 50;     // Debounce time; increase if necessary

bool buttonState1 = HIGH;
bool lastButtonState1 = HIGH;

bool buttonState2 = HIGH;
bool lastButtonState2 = HIGH;

bool buttonState3 = HIGH;
bool lastButtonState3 = HIGH;

bool buttonState4 = HIGH;
bool lastButtonState4 = HIGH;

bool buttonState5 = HIGH;
bool lastButtonState5 = HIGH;

bool buttonState6 = HIGH;           // Button 6 state for mute/unmute
bool lastButtonState6 = HIGH;       // Last state of Button 6

bool buttonState7 = HIGH;           // Button 7 state (Always On, LED on at all times)
bool lastButtonState7 = HIGH;       // Last state of Button 7

bool ledState6 = LOW;               // LED state for mute button

// Pins
#define POT_PIN A0                  // Potentiometer connected to A0
#define VU_METER_PIN 3              // VU meter connected to PWM Pin 6

// Variables
int prevVolume = -1;                // To store the last volume level

void setup() {
  Consumer.begin();

  // Set button pins as input with pull-up resistors
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(BUTTON_PIN_4, INPUT_PULLUP); // Setup Button 4 for opening Chrome
  pinMode(BUTTON_PIN_5, INPUT_PULLUP);
  pinMode(BUTTON_PIN_6, INPUT_PULLUP);
  pinMode(BUTTON_PIN_7, INPUT_PULLUP);

  pinMode(LED_PIN_1, OUTPUT);       // LED pin for other buttons
  pinMode(LED_PIN_6, OUTPUT);       // LED pin for mute button
  pinMode(LED_PIN_7, OUTPUT);       // LED pin for Button 7

  digitalWrite(LED_PIN_1, HIGH);    // LED for other buttons on
  digitalWrite(LED_PIN_6, LOW);     // Mute LED off initially
  digitalWrite(LED_PIN_7, HIGH);    // Button 7 LED stays on at all times

  Keyboard.begin();                 // Initialize HID-Project Keyboard
  pinMode(VU_METER_PIN, OUTPUT);    // Set VU meter pin as output
}

void loop() {
  unsigned long currentMillis = millis();  // Get the current time

  // Button 1 (Ctrl+C)
  buttonState1 = digitalRead(BUTTON_PIN_1);
  if (buttonState1 == LOW && lastButtonState1 == HIGH && (currentMillis - lastDebounceTime) > debounceDelay) {
    digitalWrite(LED_PIN_1, LOW);          // Turn LED for this button off while pressed
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('c');
    delay(100);
    Keyboard.releaseAll();
    lastDebounceTime = currentMillis;
  }
  lastButtonState1 = buttonState1;

  // Button 2 (Ctrl+V)
  buttonState2 = digitalRead(BUTTON_PIN_2);
  if (buttonState2 == LOW && lastButtonState2 == HIGH && (currentMillis - lastDebounceTime) > debounceDelay) {
    digitalWrite(LED_PIN_1, LOW);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('v');
    delay(100);
    Keyboard.releaseAll();
    lastDebounceTime = currentMillis;
  }
  lastButtonState2 = buttonState2;

  // Button 3 (Ctrl+S)
  buttonState3 = digitalRead(BUTTON_PIN_3);
  if (buttonState3 == LOW && lastButtonState3 == HIGH && (currentMillis - lastDebounceTime) > debounceDelay) {
    digitalWrite(LED_PIN_1, LOW);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('s');
    delay(100);
    Keyboard.releaseAll();
    lastDebounceTime = currentMillis;
  }
  lastButtonState3 = buttonState3;

  // Button 4 (Open Chrome)
  buttonState4 = digitalRead(BUTTON_PIN_4);
  if (buttonState4 == LOW && lastButtonState4 == HIGH && (currentMillis - lastDebounceTime) > debounceDelay) {
    digitalWrite(LED_PIN_1, LOW);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('1');
    delay(100);
    Keyboard.releaseAll();
    lastDebounceTime = currentMillis;
  }
  lastButtonState4 = buttonState4;

  // Button 5 (Ctrl+2)
  buttonState5 = digitalRead(BUTTON_PIN_5);
  if (buttonState5 == LOW && lastButtonState5 == HIGH && (currentMillis - lastDebounceTime) > debounceDelay) {
    digitalWrite(LED_PIN_1, LOW);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('2');
    delay(100);
    Keyboard.releaseAll();
    lastDebounceTime = currentMillis;
  }
  lastButtonState5 = buttonState5;

  // Button 6 (Mute/Unmute)
  buttonState6 = digitalRead(BUTTON_PIN_6);
  if (buttonState6 == LOW && lastButtonState6 == HIGH && (currentMillis - lastDebounceTime) > debounceDelay) {
    ledState6 = !ledState6;                                // Toggle LED state for mute button
    digitalWrite(LED_PIN_6, ledState6 ? HIGH : LOW);       // Update the LED for mute button
    Consumer.write(MEDIA_VOLUME_MUTE);                    // Mute/Unmute the system volume
    lastDebounceTime = currentMillis;                     // Update the debounce time
  }
  lastButtonState6 = buttonState6;

  // Button 7 (Always on, LED stays on)
  buttonState7 = digitalRead(BUTTON_PIN_7);
  if (buttonState7 == LOW && lastButtonState7 == HIGH && (currentMillis - lastDebounceTime) > debounceDelay) {
    digitalWrite(LED_PIN_7, HIGH);  // LED stays on permanently
    lastDebounceTime = currentMillis;
  }
  lastButtonState7 = buttonState7;

  // Keep LEDs for other buttons on
  if (buttonState1 == HIGH && buttonState2 == HIGH && buttonState3 == HIGH && buttonState4 == HIGH && buttonState5 == HIGH) {
    digitalWrite(LED_PIN_1, HIGH);
  }

  // Volume control
  int potValue = readSmoothedPotValue();
  int volume = map(potValue, 0, 1023, 0, 100);

  if (volume != prevVolume) {
    adjustVolume(volume);
    prevVolume = volume;
  }

  int vuMeterValue = map(volume, 0, 100, 0, 255);
  analogWrite(VU_METER_PIN, vuMeterValue);

  delay(50);
}

int readSmoothedPotValue() {
  const int numReadings = 10;
  long total = 0;

  for (int i = 0; i < numReadings; i++) {
    total += analogRead(POT_PIN);
    delay(5);
  }

  return total / numReadings;
}

void adjustVolume(int targetVolume) {
  static int currentVolume = 0;

  while (currentVolume < targetVolume) {
    Consumer.write(MEDIA_VOLUME_UP);
    currentVolume++;
    delay(5);
  }
  while (currentVolume > targetVolume) {
    Consumer.write(MEDIA_VOLUME_DOWN);
    currentVolume--;
    delay(5);
  }
}
