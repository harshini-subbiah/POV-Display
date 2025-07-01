// 7x7 Without IR Sensor
// Pin definitions
const int ledPins[7] = {2, 3, 4, 5, 6, 7, 8};

// Timing
const int columnDelay = 2; // milliseconds per column (adjust as needed)

// Bitmap font for "SUCCESS"
const byte SUCCESS[][7] = {
  // S
  {B0111110, B1000001, B1000000, B0111110, B0000001, B1000001, B0111110},
  // U
  {B1000001, B1000001, B1000001, B1000001, B1000001, B1000001, B0111110},
  // C
  {B0111110, B1000001, B1000000, B1000000, B1000000, B1000001, B0111110},
  // C
  {B0111110, B1000001, B1000000, B1000000, B1000000, B1000001, B0111110},
  // E
  {B1111111, B1000000, B1000000, B1111100, B1000000, B1000000, B1111111},
  // S
  {B0111110, B1000001, B1000000, B0111110, B0000001, B1000001, B0111110},
  // S
  {B0111110, B1000001, B1000000, B0111110, B0000001, B1000001, B0111110}
};

const int LETTER_WIDTH = 7;
const int SPACE = 1;

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}
void loop() {
  // Continuously display "SUCCESS"
  for (int letter = 0; letter < 7; letter++) {
    for (int col = 0; col < LETTER_WIDTH; col++) {
      displayColumn(SUCCESS[letter][col]);
      delay(columnDelay);
    }
    // Space between letters
    displayColumn(0b0000000);
    delay(columnDelay * SPACE);
  }

  // Optional: blank column at end
  displayColumn(0b0000000);
}

// Display one column pattern
void displayColumn(byte colData) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(ledPins[i], bitRead(colData, 6 - i)); // top-to-bottom mapping
  }
}
